// Copyright (c) 2015-2020 LG Electronics, Inc.
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

#ifndef BLUETOOTH_SIL_ERRORS_H_
#define BLUETOOTH_SIL_ERRORS_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

/**
 * Allowed error codes which may be returned by any SIL operation
 *
 * Any error code referred to in this API is guaranteed to come from
 * the following enumeration.
 */
enum BluetoothError
{
	/** No error (successful completion) */
	BLUETOOTH_ERROR_NONE,
	/** The operation failed for an unspecified or generic reason */
	BLUETOOTH_ERROR_FAIL,
	/** The device is not ready to perform the requested operation */
	BLUETOOTH_ERROR_NOT_READY,
	/** The SIL failed to allocated memory */
	BLUETOOTH_ERROR_NOMEM,
	/** The operation can not be performed at this time */
	BLUETOOTH_ERROR_BUSY,
	/** the requested operation is not supported by the stack or device */
	BLUETOOTH_ERROR_UNSUPPORTED,
	/** An invalid value was passed for one of the parameters */
	BLUETOOTH_ERROR_PARAM_INVALID,
	/** An unhandled error was encountered */
	BLUETOOTH_ERROR_UNHANDLED,
	/** Unknown device address */
	BLUETOOTH_ERROR_UNKNOWN_DEVICE_ADDR,
	/** Authentication with a remote device was canceled */
	BLUETOOTH_ERROR_AUTHENTICATION_CANCELED,
	/** Authentication with a remote device failed */
	BLUETOOTH_ERROR_AUTHENTICATION_FAILED,
	/** Authentication with a remote was rejected*/
	BLUETOOTH_ERROR_AUTHENTICATION_REJECTED,
	/** Authentication with a remote device timed out */
	BLUETOOTH_ERROR_AUTHENTICATION_TIMEOUT,
	/** Device is already paired */
	BLUETOOTH_ERROR_DEVICE_ALREADY_PAIRED,
	/** Device is not paired */
	BLUETOOTH_ERROR_DEVICE_NOT_PAIRED,
	/** Device is already connected */
	BLUETOOTH_ERROR_DEVICE_ALREADY_CONNECTED,
	/** Device is not connected */
	BLUETOOTH_ERROR_DEVICE_NOT_CONNECTED,
	/** Performed operation is not allowed **/
	BLUETOOTH_ERROR_NOT_ALLOWED,
	/** Operation was aborted **/
	BLUETOOTH_ERROR_ABORTED,
	/** Tethering is already enabled **/
	BLUETOOTH_ERROR_TETHERING_ALREADY_ENABLED,
	/** Tethering is already disabled **/
	BLUETOOTH_ERROR_TETHERING_ALREADY_DISABLED,
	/** Firmware update is in process **/
	BLUETOOTH_ERROR_FIRMWARE_UPDATING,
	/** Bluetooth device is already connected to one device **/
	BLUETOOTH_ERROR_ALREADY_CONNECTED_ONE_DEVICE,
	/** Select folder to perform operation **/
	BLUETOOTH_ERROR_PBAP_CALL_SELECT_FOLDER_TYPE,
	/** Item corresponding to the path passed is not playable */
	BLUETOOTH_ERROR_AVRCP_ITEM_NOT_PLAYABLE,
	/** Item is not a folder */
	BLUETOOTH_ERROR_AVRCP_NOT_A_FOLDER,
	/** Requested folder is not available */
	BLUETOOTH_ERROR_MAP_FOLDER_NOT_FOUND
};

/**
 * Callback to indicate the status of an asynchronous operation.
 *
 * The definition of this type makes it explicit that all callbacks are
 * invoked with a BluetoothError value, enforcing the contract that only
 * documented error values will be returned.
 *
 */
typedef std::function<void(BluetoothError)> BluetoothResultCallback;


#endif
