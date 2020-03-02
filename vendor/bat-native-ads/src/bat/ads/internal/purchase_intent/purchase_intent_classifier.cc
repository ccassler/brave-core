/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdint.h>
#include <functional>
#include <utility>
#include <algorithm>

#include "bat/ads/internal/purchase_intent/purchase_intent_classifier.h"
#include "bat/ads/internal/purchase_intent/funnel_sites.h"
#include "bat/ads/internal/purchase_intent/keywords.h"
#include "bat/ads/internal/time.h"

namespace ads {

PurchaseIntentClassifier::PurchaseIntentClassifier(
    const uint16_t signal_level,
    const uint16_t classification_threshold,
    const uint64_t signal_decay_time_window)
    : signal_level_(signal_level),
      classification_threshold_(classification_threshold),
      signal_decay_time_window_(signal_decay_time_window) {
}

PurchaseIntentClassifier::~PurchaseIntentClassifier() = default;

PurchaseIntentSignalInfo PurchaseIntentClassifier::ExtractIntentSignal(
    const std::string& url) {
  PurchaseIntentSignalInfo signal_info;
  std::string search_query = SearchProviders::ExtractSearchQueryKeywords(url);

  if (!search_query.empty()) {
    auto kw_segments = Keywords::GetSegments(search_query);

    if (!kw_segments.empty()) {
      uint8_t kw_weight = Keywords::GetFunnelWeight(search_query);

      signal_info.timestamp_in_seconds = Time::NowInSeconds();
      signal_info.segments = kw_segments;
      signal_info.weight = kw_weight;
      return signal_info;
    }
  } else {
    FunnelSiteInfo funnel_site = FunnelSites::MatchFunnelSite(url);

    if (!funnel_site.url_netloc.empty()) {
      signal_info.timestamp_in_seconds = Time::NowInSeconds();
      signal_info.segments = funnel_site.segments;
      signal_info.weight = funnel_site.weight;
      return signal_info;
    }
  }

  return signal_info;
}

WinningPurchaseIntentCategories PurchaseIntentClassifier::GetWinningCategories(  // NOLINT
    const PurchaseIntentSignalHistoriesPerSegment& history,
    uint8_t max_segments) {
  WinningPurchaseIntentCategories winning_categories;
  if (history.empty()) {
    return winning_categories;
  }

  if (max_segments < history.size()) {
    max_segments = history.size();
  }

  std::vector<std::pair<std::string, uint8_t>> scores;
  for (const auto& segment_history : history) {
    uint8_t score = GetIntentScoreForSegment(segment_history.second);
    scores.push_back(std::make_pair(segment_history.first, score));
  }

  std::sort(scores.begin(), scores.end(), [](auto &lhs, auto &rhs) {
    return lhs.second > rhs.second;
  });

  const std::vector<std::pair<std::string, uint8_t>> top_scores(
      scores.begin(), scores.begin() + scores.size());
  for (const auto& scores_el : top_scores) {
    if (scores_el.second > classification_threshold_) {
      winning_categories.push_back(scores_el.first);
    }
  }

  return winning_categories;
}

uint8_t PurchaseIntentClassifier::GetIntentScoreForSegment(
    const std::deque<PurchaseIntentSignalHistory>& segment_history) {
  uint8_t intent_score = 0;

  for (const auto& signal : segment_history) {
    const base::Time signal_decayed_at =
        Time::FromDoubleT(signal.timestamp_in_seconds) +
        base::TimeDelta::FromSeconds(signal_decay_time_window_);
    const base::Time now = base::Time::Now();

    if (now > signal_decayed_at) {
      continue;
    }

    intent_score += signal_level_ * signal.weight;
  }

  return intent_score;
}

}  // namespace ads
