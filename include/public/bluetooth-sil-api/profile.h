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

#ifndef BLUETOOTH_SIL_PROFILE_H_
#define BLUETOOTH_SIL_PROFILE_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

/**
 * @brief This interface is the base to implement an observer for the Bluetooth
 *        profile to get notifications from the profile when something has changed.
 */
class BluetoothProfileStatusObserver
{
public:
	/**
	 * @brief This method is called when a property of the profile regarding a specific
	 *        device has changed.
	 *
	 * @param address Device address of the device properties has changed for
	 * @param properties List of properties which have changed.
	 */
	virtual void propertiesChanged(const std::string &address, BluetoothPropertiesList properties) { }
};

/**
 * @brief This interface defines common operations for a Bluetooth profile.
 */
class BluetoothProfile
{
public:
	/**
	 * @brief Default C'tor
	 */
	BluetoothProfile() : observer(0) { }

	virtual ~BluetoothProfile() { }

	/**
	 * @brief Register a observer for the profile. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 *        A subsequent call will override the currently registered observer.
	 *
	 * @param observer Observer instance to register.
	 */
	virtual void registerObserver(BluetoothProfileStatusObserver *observer)
	{
		this->observer = observer;
	}

	/**
	 * @brief Perform a request to the SIL to retrieve all properties of the Bluetooth
	 *        profile. The result of the operation is handed back with the supplied
	 *        callback function.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after getProperties has returned to the caller
	 *        or before.
	 *
	 * @param address Device address to retrieve the properties for
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void getProperties(const std::string &address, BluetoothPropertiesResultCallback  callback) = 0;

	/**
	 * @brief Retrieve a specific property of the Bluetooth profile.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after getProperty has returned to the caller
	 *        or before.
	 *
	 * @param address Device address to retrieve the properties for
	 * @param type Property of the profile to retrieve from the SIL.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void getProperty(const std::string &address, BluetoothProperty::Type type,
	                         BluetoothPropertyResultCallback callback) = 0;

	/**
	 * @brief Connect with a profile on a specific remote device.
	 *
	 * @param address Address of the device to connect to.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void connect(const std::string &address, BluetoothResultCallback callback) = 0;

	/**
	 * @brief Disconnect a remote device from the profile if connected.
	 *
	 *        If not connected the callback will return immediately and the operation
	 *        marked as succeeded.
	 *
	 * @param address Address of the device to disconnect
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void disconnect(const std::string &address, BluetoothResultCallback callback) = 0;

	/**
	 * @brief Enable the profile and role.
	 *
	 *        This will perform internal actions required to enable the Bluetooth profile.
	 *        Changed property(BluetoothProperty::Type::UUIDS) of the adapter must be notified
	 *        to the user through the notifyAdapterPropertyChanged method of the
	 *        BluetoothAdapterStatusObserver interface.
	 *
	 * @param uuid Uuid of the profile
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void enable(const std::string &uuid, BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Disable the profile and role.
	 *
	 *        This will perform internal actions required to disable the Bluetooth profile.
	 *        Changed property(BluetoothProperty::Type::UUIDS) of the adapter must be notified
	 *        to the user through the notifyAdapterPropertyChanged method of the
	 *        BluetoothAdapterStatusObserver interface.
	 *
	 * @param uuid Uuid of the profile
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void disable(const std::string &uuid, BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

protected:
	/**
	 * @brief Retrieve the profile status observer
	 *
	 *        If no observer was registered with registerObserver before a NULL object
	 *        instance will be returned which is usable but will doing nothing when
	 *        called.
	 *
	 * @return The profile status observer
	 */
	BluetoothProfileStatusObserver* getObserver()
	{
		static BluetoothProfileStatusObserver nullObserver;

		if (!observer)
			return &nullObserver;

		return observer;
	}

	BluetoothProfileStatusObserver *observer;
};

#endif
