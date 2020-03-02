/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdint.h>
#include <algorithm>
#include <sstream>

#include "url/gurl.h"
#include "third_party/re2/src/re2/re2.h"
#include "bat/ads/internal/purchase_intent/keywords.h"

namespace ads {

Keywords::Keywords() = default;
Keywords::~Keywords() = default;

std::vector<std::string> Keywords::GetSegments(
    const std::string& search_query) {
  auto search_query_kwset = TransformIntoSetOfWords(search_query);

  for (const auto& kw_info : _automotive_segment_keywords) {
    auto list_kwset = TransformIntoSetOfWords(kw_info.keywords);

    if (Keywords::IsSubset(search_query_kwset, list_kwset)) {
      return kw_info.segments;
    }
  }

  return {};
}

uint8_t Keywords::GetFunnelWeight(
    const std::string& search_query) {
  auto search_query_kwset = TransformIntoSetOfWords(search_query);

  for (const auto& kw_info : _automotive_funnel_keywords) {
    auto list_kwset = TransformIntoSetOfWords(kw_info.keywords);

    if (Keywords::IsSubset(search_query_kwset, list_kwset)) {
      return kw_info.weight;
    }
  }

  return _default_signal_weight;
}

// TODO(Moritz Haller): Rename to make explicit that method checks set_a is
// subset of set_b but not vice versa
bool Keywords::IsSubset(
    std::vector<std::string> kwset_a,
    std::vector<std::string> kwset_b) {
  std::sort(kwset_a.begin(), kwset_a.end());
  std::sort(kwset_b.begin(), kwset_b.end());

  return std::includes(kwset_a.begin(), kwset_a.end(),
      kwset_b.begin(), kwset_b.end());
}

// TODO(Moritz Haller): Implement as std::set?
std::vector<std::string> Keywords::TransformIntoSetOfWords(
    const std::string& text) {
  std::string data = text;
  // Remove every character that is not a word/whitespace/underscore character
  RE2::GlobalReplace(&data, "[^\\w\\s]|_", "");
  // Strip subsequent white space characters
  RE2::GlobalReplace(&data, "\\s+", " ");

  std::for_each(data.begin(), data.end(), [](char & c) {
    c = base::ToLowerASCII(c);
  });

  std::stringstream sstream(data);
  std::vector<std::string> set_of_words;
  std::string word;
  uint16_t word_count = 0;

  while (sstream >> word && word_count < _word_count_limit) {
    set_of_words.push_back(word);
    word_count++;
  }

  return set_of_words;
}

}  // namespace ads
