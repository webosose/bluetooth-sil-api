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

#ifndef BLUETOOTH_SIL_OBSERVER_H_
#define BLUETOOTH_SIL_OBSERVER_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

/**
 * @brief List of WoBLE triggering device's MAC address.
 */
typedef std::vector<int32_t> BluetoothLinkKey;

class BluetoothAdapter;
/**
 * @brief This interface is the base to implement an observer for the Bluetooth
 *        adapter to get notifications from the adapter when something has changed.
 */
class BluetoothAdapterStatusObserver
{
public:
	/**
	 * Virtual destructor to support deletion through pointers to the base class.
	 */

	virtual ~BluetoothAdapterStatusObserver() { }

	/**
	 * @brief The method is called when the state of the adapter changes.
	 *
	 * @param powered Indicates the power state of the adapter. True means the adapter
	 *        is fully powered and ready for connection attempts or device discovery.
	 *        If false the adapter is powered down an no actions are possible.
	 */
	virtual void adapterStateChanged(bool powered) { }

	/**
	 * @brief The method is called when the HCI timeout of adapter occurs.
	 *
	 */
	virtual void adapterHciTimeoutOccurred() { }

	/**
	 * @brief This method is called when a property of the adapter changes.
	 *
	 * @param properties List of properties which have changed.
	 */
	virtual void adapterPropertiesChanged(BluetoothPropertiesList properties) { }

	/**
	 * @brief The method is called when the state of the keep alive changes.
	 *
	 * @param enabled Indicates the state of the keep alive. True means the keep alive
	 *        is running. If false the keep alive is not running.
	 */
	virtual void adapterKeepAliveStateChanged(bool enabled) { }

	/**
	 * @brief The method is called when a new device is discovered.
	 *
	 * @param properties Properties of the new device.
	 */
	virtual void deviceFound(BluetoothPropertiesList properties) { }

	/**
	 * @brief The method is called when a new device is discovered.
	 *
	 * @param properties Properties of the new device.
	 */
	virtual void deviceFound(const std::string &address, BluetoothPropertiesList properties) { }

	/**
	 * @brief The method is called when a device has disappeared and is no longer
	 *        available.
	 *
	 * @param address Address of the removed device.
	 */
	virtual void deviceRemoved(const std::string &address) { }

	/**
	 * @brief The method is called when a new LE device is discovered by scan ID
	 *
	 * @param scanId Unique ID of LE scan
	 * @param properties Properties of the new device.
	 */
	virtual void leDeviceFoundByScanId(uint32_t scanId, BluetoothPropertiesList properties) { }

	/**
	 * @brief The method is called when a device has disappeared and is no longer
	 *        available.
	 *
	 * @param scanId Unique ID of LE scan
	 * @param address Address of the removed device.
	 */
	virtual void leDeviceRemovedByScanId(uint32_t scanId, const std::string &address) { }

	/**
	 * @brief The method is called when the link key is created for a connected remote device.
	 *
	 * @param address Address of the device which the link key is created.
	 * @param
	 */
	virtual void deviceLinkKeyCreated(const std::string &address, BluetoothLinkKey LinkKey) { }

	/**
	 * @brief The method is called when the link key is destroyed for a connected remote device.
	 *
	 * @param address Address of the device which the link key is created.
	 * @param
	 */
	virtual void deviceLinkKeyDestroyed(const std::string &address, BluetoothLinkKey LinkKey) { }

	/**
	 * @brief The method is called when one or more properties have changed for a
	 *        specific device.
	 * @param address Address of the device whose properties have changed.
	 * @param properties List of changed properties.
	 */
	virtual void devicePropertiesChanged(const std::string &address,
	                                     BluetoothPropertiesList properties) { }

	/**
	 * @brief The method is called when one or more properties have changed for a
	 *        specific device.
	 *
	 * @param scanId Unique ID of BLE discovery
	 * @param address Address of the device whose properties have changed.
	 * @param properties List of changed properties.
	 */
	virtual void leDevicePropertiesChangedByScanId(uint32_t scanId, const std::string &address,
	                                     BluetoothPropertiesList properties) { }

	/**
	 * @brief The method is called when the status of the device discovery process changes.
	 *        This will happen when either startDiscovery or cancelDiscovery of the adapter
	 *        class are called or the configured discovery timeout is reached.
	 *
	 * @param active Indicates whether the device discovery process is active or not.
	 */
	virtual void discoveryStateChanged(bool active) { }

	/**
	 * @brief The method is called when the device has to display a pairing confirmation
	 *        dialog for a specific device. The user has to confirm that the passkey is
	 *        the same on both devices. The passkey can be accepted by calling the
	 *        supplyPairingConfirmation method of the BluetoothAdapter class.
	 *
	 *        Only used with devices supporting Bluetooth version >= 2.1 and Simple Secure
	 *        Pairing (SSP).
	 *
	 * @param address Address of the device
	 * @param passkey Passkey which the user has to confirm that it is the same on both devices.
	 */
	virtual void displayPairingConfirmation(const std::string &address, BluetoothPasskey passkey) { }

	/**
	 * @brief The method is called when the PIN for legacy pairing should be shown to the user.
	 *
	 * @param address Address of the device
	 * @param secret PIN to display to the user.
	 */
	virtual void displayPairingSecret(const std::string &address, const std::string &pin) { }

	/**
	 * @brief The method is called when the passkey should be shown to the user. The
	 *        passkey is autogenerated in this case and doesn't need to be confirmed but
	 *        has to be presented to the user until the pairing process succeeds or fails.
	 *
	 *        The secret has to be visible for the user until the pairing succeeds or fails.
	 *
	 * @param address Address of the device
	 * @param secret Autogenerated passkey to display to the user.
	 */
	virtual void displayPairingSecret(const std::string &address, BluetoothPasskey passkey) { }

	/**
	 * @brief The method is called when the user has to supply a secret for authentication with
	 *        the remote device. The secret can be supplied by calling the supplyPairingSecret
	 *        method of the BluetoothAdapter class.
	 *
	 *        The pairing secret type abstracts the usage of Bluetooth devices with version
	 *        < 2.1 which only supports legacy pairing and device with version >= 2.1 which
	 *        support Simple Secure Pairing (SSP). See \ref{BluetoothPairingSecretType} for
	 *        details.
	 *
	 * @param address Address of the device
	 * @param type Type of requested secret
	 */
	virtual void requestPairingSecret(const std::string &address, BluetoothPairingSecretType type) { }

	/**
	 * @brief The method is called when an ongoing pairing request is canceled.
	 */
	virtual void pairingCanceled() { }

	/**
	 * @brief This method is called when a remote device initiates or disconnects a BLE connection.
	 *
	 * @param address Address of the device.
	 * @param state Value is true for connect request and false for disconnect.
	 */
	virtual void leConnectionRequest(const std::string &address, bool state) { }
};

#endif
