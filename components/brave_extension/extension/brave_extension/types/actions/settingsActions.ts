/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as types from '../../constants/settingsTypes'
import { Settings, SettingsData } from '../other/settingsTypes'
import { Dispatch } from 'redux'
import { State } from '../state/mainState'

interface SetStoreSettingsChangeReturn {
  type: typeof types.SET_STORE_SETTINGS_CHANGE,
  settingsData: Partial<SettingsData>
}

export interface SetStoreSettingsChange {
  (settingsData: Partial<SettingsData>): SetStoreSettingsChangeReturn
}

export interface SettingsDidChange {
  (settings: Settings): SetStoreSettingsChangeReturn
}

export interface SettingsDataShouldUpdate {
  (): (dispatch: Dispatch, getState: () => State) => void
}

export type settingsActions =
  SetStoreSettingsChangeReturn
