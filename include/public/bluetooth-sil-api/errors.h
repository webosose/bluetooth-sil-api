// Copyright (c) 2015-2021 LG Electronics, Inc.
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
	BLUETOOTH_ERROR_NONE = 0,
	/** The operation failed for an unspecified or generic reason */
	BLUETOOTH_ERROR_FAIL = 1,
	/** The device is not ready to perform the requested operation */
	BLUETOOTH_ERROR_NOT_READY = 2,
	/** The SIL failed to allocated memory */
	BLUETOOTH_ERROR_NOMEM = 3,
	/** The operation can not be performed at this time */
	BLUETOOTH_ERROR_BUSY = 4,
	/** the requested operation is not supported by the stack or device */
	BLUETOOTH_ERROR_UNSUPPORTED = 5,
	/** An invalid value was passed for one of the parameters */
	BLUETOOTH_ERROR_PARAM_INVALID = 6,
	/** An unhandled error was encountered */
	BLUETOOTH_ERROR_UNHANDLED = 7,
	/** Unknown device address */
	BLUETOOTH_ERROR_UNKNOWN_DEVICE_ADDR = 8,
	/** Authentication with a remote device was canceled */
	BLUETOOTH_ERROR_AUTHENTICATION_CANCELED = 9,
	/** Authentication with a remote device failed */
	BLUETOOTH_ERROR_AUTHENTICATION_FAILED = 10,
	/** Authentication with a remote was rejected*/
	BLUETOOTH_ERROR_AUTHENTICATION_REJECTED = 11,
	/** Authentication with a remote device timed out */
	BLUETOOTH_ERROR_AUTHENTICATION_TIMEOUT = 12,
	/** Device is already paired */
	BLUETOOTH_ERROR_DEVICE_ALREADY_PAIRED = 13,
	/** Device is not paired */
	BLUETOOTH_ERROR_DEVICE_NOT_PAIRED = 14,
	/** Device is already connected */
	BLUETOOTH_ERROR_DEVICE_ALREADY_CONNECTED = 15,
	/** Device is not connected */
	BLUETOOTH_ERROR_DEVICE_NOT_CONNECTED = 16,
	/** Performed operation is not allowed **/
	BLUETOOTH_ERROR_NOT_ALLOWED = 17,
	/** Operation was aborted **/
	BLUETOOTH_ERROR_ABORTED = 18,
	/** Tethering is already enabled **/
	BLUETOOTH_ERROR_TETHERING_ALREADY_ENABLED = 19,
	/** Tethering is already disabled **/
	BLUETOOTH_ERROR_TETHERING_ALREADY_DISABLED = 20,
	/** Firmware update is in process **/
	BLUETOOTH_ERROR_FIRMWARE_UPDATING = 21,
	/** Bluetooth device is already connected to one device **/
	BLUETOOTH_ERROR_ALREADY_CONNECTED_ONE_DEVICE = 22,
	/** Select folder to perform operation **/
	BLUETOOTH_ERROR_PBAP_CALL_SELECT_FOLDER_TYPE = 23,
	/** Item corresponding to the path passed is not playable */
	BLUETOOTH_ERROR_AVRCP_ITEM_NOT_PLAYABLE = 24,
	/** Item is not a folder */
	BLUETOOTH_ERROR_AVRCP_NOT_A_FOLDER = 25,
	/** Requested folder is not available */
	BLUETOOTH_ERROR_MAP_FOLDER_NOT_FOUND = 26,
	/** Requested message handle is not available */
	BLUETOOTH_ERROR_MAP_MESSAGE_HANDLE_NOT_FOUND = 27,
	/** netIndex does not exist */
	BLUETOOTH_ERROR_MESH_NET_KEY_INDEX_DOES_NOT_EXIST = 28,
	/** appIndex already exists */
	BLUETOOTH_ERROR_MESH_APP_KEY_INDEX_ALREADY_EXISTS = 29,
	/** appIndex does not exist */
	BLUETOOTH_ERROR_MESH_APP_KEY_INDEX_DOES_NOT_EXIST = 30,
	/** No response received from remote node */
	BLUETOOTH_ERROR_MESH_NO_RESPONSE = 31,
	/** Bad PDU during provisioning */
	BLUETOOTH_ERROR_MESH_BAD_PDU = 32,
	/** Provision confirmation failed */
	BLUETOOTH_ERROR_MESH_PROV_CONFIRMATION_FAILED = 33,
	/** Cannot assign address */
	BLUETOOTH_ERROR_MESH_CANNOT_ASSIGN_ADDRESS = 34,
	/** Mesh device not found */
	BLUETOOTH_ERROR_MESH_DEVICE_NOT_FOUND = 35,
	/** Mesh node not available */
	BLUETOOTH_ERROR_MESH_NODE_NOT_AVAILABLE = 36,
	/** Destination address is not valid */
	BLUETOOTH_ERROR_MESH_INVALID_DEST_ADDRESS = 37,
	/** Not authorised to perform this action */
	BLUETOOTH_ERROR_MESH_NOT_AUTHORISED = 38,
	/** Mesh network already exists */
	BLUETOOTH_ERROR_MESH_NETWORK_EXISTS = 39,
	/** Invalid model send cmd */
	BLUETOOTH_ERROR_MESH_INVALID_COMMAND = 40,
	/** Timeout during provisioning */
	BLUETOOTH_ERROR_MESH_TIMEOUT = 41,
	/** Confirmation-failed during provisioning */
	BLUETOOTH_ERROR_MESH_CONFIRMATION_FAILED = 42,
	/** Out-of-resources during provisioning */
	BLUETOOTH_ERROR_MESH_OUT_OF_RESOURCES = 43,
	/** Decryption-error during provisioning */
	BLUETOOTH_ERROR_MESH_DECRYPTION_ERROR = 44,
	/** Unexpected-error during provisioning */
	BLUETOOTH_ERROR_MESH_UNEXPECTED_ERROR = 45,
	/** Cannot-assign-addresses during provisioning */
	BLUETOOTH_ERROR_MESH_CANNOT_ASSIGN_ADDRESSES = 46,
	/** Cannot-update appKey */
	BLUETOOTH_ERROR_MESH_CANNOT_UPDATE_APPKEY = 47,
	/** No response from remote node */
	BLUETOOTH_ERROR_MESH_NO_RESPONSE_FROM_NODE = 48
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
