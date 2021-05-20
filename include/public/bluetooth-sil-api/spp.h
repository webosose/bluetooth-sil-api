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

#ifndef BLUETOOTH_SIL_SPP_H_
#define BLUETOOTH_SIL_SPP_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

const std::string BLUETOOTH_PROFILE_ID_SPP = "SPP";

typedef uint8_t BluetoothSppChannelId;
const BluetoothSppChannelId BLUETOOTH_SPP_CHANNEL_ID_INVALID = 0;

/**
 * @brief This interface is the base to implement an observer for the Bluetooth
 *        SPP to get notifications from the profile when something has changed.
 */
class BluetoothSppStatusObserver
{
public:
	/**
	 * @brief This method is called when the channel's state gets changed.
	 *
	 * @param adapterAddress Address of the Adapter
	 * @param address Address of the remote device
	 * @param uuid UUID used by the server application
	 * @param channelId Unique ID of a SPP channel
	 * @param state State of the channel
	 */
	virtual void channelStateChanged(const std::string &adapterAddress, const std::string &address, const std::string &uuid, const BluetoothSppChannelId channelId, const bool state) {}

	/**
	 * @brief This method is called when some data received through the channal.
	 *
	 * @param channelId Unique ID of a SPP channel
	 * @param adapterAddress Address of the Adapter
	 * @param data Data to be received
	 * @param size Data size in bytes
	 */
	virtual void dataReceived(const BluetoothSppChannelId channelId, const std::string &adapterAddress, const uint8_t *data, const uint32_t size) {}
};

/**
 * @brief Callback to return SPP channel ID
 */
typedef std::function<void(const BluetoothError, const BluetoothSppChannelId channelId)>
BluetoothChannelResultCallback;

/**
 * @brief Callback to return SPP channel state
 */
typedef std::function<void(const BluetoothError, const bool state)>
BluetoothChannelStateResultCallback;

/**
 * @brief Interface to abstract the operations for the SPP bluetooth profile.
 */
class BluetoothSppProfile
{
public:
	BluetoothSppProfile() : observer(0) { }

	virtual ~BluetoothSppProfile() { }

	/**
	 * @brief Register a observer for the SPP. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 * @param observer Observer instance to register.
	 */
	void registerObserver(BluetoothSppStatusObserver *observer)
	{
		this->observer = observer;
	}

	/**
	 * @brief Retrieve a channel state of the Bluetooth SPP.
	 *
	 * @param address Device address to retrieve the properties for
	 * @param uuid UUID used by the server application
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void getChannelState(const std::string &address, const std::string &uuid, BluetoothChannelStateResultCallback callback) = 0;

	/**
	 * @brief Connect with a SPP on a specific remote device.
	 *
	 * @param address Address of the device to connect to
	 * @param uuid UUID used by the server application when it opened its SPP channel
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void connectUuid(const std::string &address, const std::string &uuid, BluetoothChannelResultCallback callback) = 0;

	/**
	 * @brief Disconnect a remote device from the profile if connected.
	 *
	 *        If not connected the callback will return immediately and the operation
	 *        marked as succeeded.
	 *
	 * @param channelId Unique ID of a SPP channel
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void disconnectUuid(const BluetoothSppChannelId channelId, BluetoothResultCallback callback) = 0;

	/**
	 * @brief Transfer data to the connected remote device.
	 *
	 * @param channelId Unique ID of a SPP channel
	 * @param data Data to send
	 * @param size Data size in bytes
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void writeData(const BluetoothSppChannelId channelId, const uint8_t *data, const uint32_t size, BluetoothResultCallback callback) = 0;

	/**
	 * @brief Register a service record in the device service record database with the specified UUID and name.
	 *
	 * @param name An identifiable name of a SPP service in the server
	 * @param uuid UUID used by the server application
	 *
	 * @return BLUETOOTH_ERROR_NONE when operation was successful another error code otherwise
	 */
	virtual BluetoothError createChannel(const std::string &name, const std::string &uuid) = 0;

	/**
	 * @brief Remove a service record in the device service record database with the specified UUID
	 *
	 * @param uuid UUID used by the server application
	 *
	 * @return BLUETOOTH_ERROR_NONE when operation was successful another error code otherwise
	 */
	virtual BluetoothError removeChannel(const std::string &uuid) = 0;

protected:
	/**
	 * @brief Retrieve the SPP status observer
	 *
	 *        If no observer was registered with registerObserver before a NULL object
	 *        instance will be returned which is usable but will doing nothing when
	 *        called.
	 *
	 * @return SPP status observer
	 */
	BluetoothSppStatusObserver* getSppObserver()
	{
		static BluetoothSppStatusObserver nullObserver;

		if (!observer)
			return &nullObserver;

		return observer;
	}

private:
	BluetoothSppStatusObserver *observer;
};

#endif
