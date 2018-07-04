// Copyright (c) 2015-2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef BLUETOOTH_SIL_PAN_H_
#define BLUETOOTH_SIL_PAN_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

const std::string BLUETOOTH_PROFILE_ID_PAN = "PAN";

/**
 * @brief Interface to abstract the operations for the PAN bluetooth profile.
 */
class BluetoothPanProfile
{
public:
	BluetoothPanProfile() { }

	virtual ~BluetoothPanProfile() { }

	/**
	 * @brief Enable or disable NAP for Bluetooth tethering
	 *
	 * @param enable true or false to enable or disable NAP
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void setTethering(const bool enable, BluetoothResultCallback callback) = 0;

};

#endif
