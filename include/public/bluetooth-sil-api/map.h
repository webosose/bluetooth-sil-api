// Copyright (c) 2020 LG Electronics, Inc.
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

#ifndef BLUETOOTH_SIL_MAP_H_
#define BLUETOOTH_SIL_MAP_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

const std::string BLUETOOTH_PROFILE_ID_MAP = "MAP";

/**
 * Callback to indicate the status of an asynchronous operation.
 *
 * The definition of this type makes it explicit that all callbacks are
 * invoked with a BluetoothError value, enforcing the contract that only
 * documented error values will be returned.
 *
 */
typedef std::function<void(BluetoothError, const std::string& )> BluetoothMapCallback;

typedef std::function<void(BluetoothError, std::list<std::string>filters)>
BluetoothMapListFiltersResultCallback;
/**
 * @brief This interface is the base to implement an observer for the MAP profile.
 */
class BluetoothMapStatusObserver
{
public:
	/**
	 * @brief The method is called when a MAP specific property changes
	 *
	 * @param properties List of properties which have changed.
	 */
	virtual void profilePropertiesChanged(const std::string &adapterAddress, const std::string &address) { }
};

/**
 * @brief Interface to abstract the operations for the MAP bluetooth profile.
 */
class BluetoothMapProfile
{
public:
	BluetoothMapProfile() { }

	virtual ~BluetoothMapProfile() { }

	/**
	 * @brief Register a observer for the MAP profile. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 * @param observer Observer instance to register.
	 */
	void registerObserver(BluetoothMapStatusObserver *observer)
	{
		this->observer = observer;
	}

	/**
	 * @brief Connect with a MAP profile on a specific remote device.
	 *
	 * @param address Address of the device to connect to.
	 * @param instanceName Indicate specific MAS instance of MSE to which connection should be initiated.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed with session Path.
	 */
	virtual void connect(const std::string &address, const std::string &instanceName, BluetoothMapCallback callback) = 0;

	/**
	 * @brief Disconnect a remote device from the MAP profile if connected.
	 *
	 *        If not connected the callback will return immediately and the operation
	 *        marked as succeeded.
	 *
	 * @param sessionKey Unique identifier of device Address of the remote device to disconnect
	 * @param sessionId Unique identifier of a session which was returned from map/connect.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void disconnect(const std::string &sessionKey, const std::string &sessionId, BluetoothMapCallback callback) = 0;

	/**
	 * @brief To get the list of supported filters field.
	 *
	 *        This method is only for MCE to get the list of filters.
	 *
	 * @param sessionKey Unique identifier of device Address of the remote device
	 * @param sessionId Unique identifier of a session which was returned from map/connect.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed with list of supported filters.
	 */
	virtual void getMessageFilters(const std::string &sessionKey, const std::string &sessionId, BluetoothMapListFiltersResultCallback callback) = 0;

protected:
	/**
	 * @brief Retrieve the MAP status observer
	 *
	 *        If no observer was registered with registerObserver before a NULL object
	 *        instance will be returned which is usable but will doing nothing when
	 *        called.
	 *
	 * @return The MAP status observer
	 */
	BluetoothMapStatusObserver *getMapObserver()
	{
		static BluetoothMapStatusObserver nullObserver;

		if (!observer)
			return &nullObserver;

		return observer;
	}

private:
	BluetoothMapStatusObserver *observer;

};

#endif //BLUETOOTH_SIL_MAP_H_
