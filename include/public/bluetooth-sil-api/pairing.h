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

#ifndef BLUETOOTH_SIL_PAIRING_H_
#define BLUETOOTH_SIL_PAIRING_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

/**
 * @brief Device IO capabilities used for pairing
 */
enum BluetoothPairingIOCapability
{
	BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT,
	BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_ONLY,
	BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_YES_NO,
	BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_ONLY,
	BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_DISPLAY,
};

/**
 * @brief The type of secret used for Bluetooth pairing
 *
 * When using Bluetooth we have to deal with two different kinds of pairing. One
 * is the legacy pairing used with devices supporting Bluetooth version < 2.1 and
 * the other Secure Simple Pairing (SSP) for devices supporting version >= 2.1.
 *
 * The two different types here specify the different formats used for the secret.
 * The PIN code is used for legacy pairing devices and the passkey for devices
 * supporting SSP.
 */
enum BluetoothPairingSecretType
{
	/** alphanumeric string with 1-16 characters */
	BLUETOOTH_PAIRING_SECRET_TYPE_PIN,
	/** Numeric value between 0 and 999999 */
	BLUETOOTH_PAIRING_SECRET_TYPE_PASSKEY
};

/**
 * @brief Bluetooth passkey
 *
 * Numeric value between 0 and 999999.
 */
typedef uint32_t BluetoothPasskey;

#endif
