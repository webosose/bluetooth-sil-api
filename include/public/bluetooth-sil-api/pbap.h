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

#ifndef BLUETOOTH_SIL_PBAP_H_
#define BLUETOOTH_SIL_PBAP_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

#include <list>

const std::string BLUETOOTH_PROFILE_ID_PBAP = "PBAP";

typedef uint64_t BluetoothPbapAccessRequestId;
const BluetoothPbapAccessRequestId BLUETOOTH_PBAP_ACCESS_REQUEST_ID_INVALID = 0;

typedef std::map<std::string, std::string> BluetoothPbapVCardList;

typedef std::function<void(BluetoothError, uint16_t size)>
BluetoothPbapGetSizeResultCallback;
typedef std::function<void(BluetoothError, BluetoothPbapVCardList&)>
BluetoothPbapVCardListResultCallback;

typedef std::function<void(BluetoothError, std::list<std::string> filters)>
BluetoothPbapListFiltersResultCallback;

/**
 * @brief This interface is the base to implement an observer for the PBAP profile.
 */
class BluetoothPbapStatusObserver
{
public:
	/**
	 * @brief The method is called when the device has to display a request for PBAP access confirmation
	 *        dialog for a specific device. The user has to confirm that the acception or
	 *        rejection the same on both devices.
	 *
	 * @param accessRequestId Id of the incoming access request
	 * @param address Address of the device
	 * @param deviceName deviceName of the device
	*/
	virtual void accessRequested(BluetoothPbapAccessRequestId accessRequestId, const std::string &address,
	        const std::string &deviceName) { }

	/**
	 * @brief The method is called when a PBAP specific property changes
	 *
	 * @param properties List of properties which have changed.
	 */
	virtual void profilePropertiesChanged(BluetoothPropertiesList properties, const std::string &address) { }
};

/**
 * @brief Interface to abstract the operations for the PBAP bluetooth profile.
 */
class BluetoothPbapProfile
{
public:
	BluetoothPbapProfile() { }

	virtual ~BluetoothPbapProfile() { }

	/**
	 * @brief Register a observer for the PBAP profile. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 * @param observer Observer instance to register.
	 */
	void registerObserver(BluetoothPbapStatusObserver *observer)
	{
		this->observer = observer;
	}

	/**
	 * @brief Accept the PBAP access request with a remote device or reject the request. Should
	 *        be only called after the SIL asked for it by calling the displayAccessRequest
	 *        method of the registered observer which is the BluetoothPbapStatusObserver class.
	 *        The displayAccessRequest is the signal of phonebook access.
	 *
	 *        This method is only for the service side of PBAP to support incoming requests.
	 *
	 * @param accessRequestId Id of the access request
	 * @param accept True to accept the PBAP access request or False to reject.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void supplyAccessConfirmation(BluetoothPbapAccessRequestId accessRequestId, bool accept, BluetoothResultCallback callback) = 0;

	/**
	 * @brief To set phonebook path of server device. Should be only called after the client
	 *        PBAP profile is connected with server device by calling the connect
	 *
	 *        This method is only for the client side of PBAP(PCE) Role.
	 *
	 * @param address Address of the remote device
	 * @param repository Where the phonebook is store
	 * @param object Folder path for the phonebook
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void setPhoneBook(const std::string &address, const std::string &repository, const std::string &object, BluetoothResultCallback callback) = 0;

	 /**
	 * @brief To get the total number of vcf entries in selected phonebook path.
	 *
	 *        This method is only for the client side of PBAP(PCE) Role.
	 *
	 * @param address Address of the remote device
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed with total number of vcf entries in selected phonebook path.
	 */
	virtual void getPhonebookSize(const std::string &address, BluetoothPbapGetSizeResultCallback callback) = 0;

	/**
	 * @brief This method will fetch the total vcf files entries from PSE device.
	 *        Should be only called after setPhoneBook operation is successful.
	 *        The response will contain array of  strings [vcf handle, Name]
	 *
	 *        This method is only for the client side of PBAP(PCE) Role.
	 *
	 * @param address of the PSE device from which list of vcf need to fetched.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void vCardListing(const std::string &address, BluetoothPbapVCardListResultCallback callback) = 0;

	/**
	 * @brief This method will get supported properties from PSE device.
	 *        The response will contain array of  strings [folder, DatabaseIdentifier,
	 *        PrimaryCounter, SecondaryCounter, FixedImageSize]
	 *
	 *        This method is only for the client side of PBAP(PCE) Role.
	 *
	 * @param address Address of the remote device
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void getPhoneBookProperties(const std::string &address, BluetoothPropertiesResultCallback callback) = 0;

	/**
	 * @brief To get the list of supported filters field.
	 *
	 *        This method is only for the service side of PBAP to get the list of filters.
	 *
	 * @param address Address of the remote device
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed with total number of vcf entries in selected phonebook path.
	 */
	virtual void getvCardFilters(const std::string &address, BluetoothPbapListFiltersResultCallback callback) = 0;
protected:
	/**
	 * @brief Retrieve the PBAP status observer
	 *
	 *        If no observer was registered with registerObserver before a NULL object
	 *        instance will be returned which is usable but will doing nothing when
	 *        called.
	 *
	 * @return The PBAP status observer
	 */
	BluetoothPbapStatusObserver *getPbapObserver()
	{
		static BluetoothPbapStatusObserver nullObserver;

		if (!observer)
			return &nullObserver;

		return observer;
	}

private:
	BluetoothPbapStatusObserver *observer;

};

#endif //BLUETOOTH_SIL_PBAP_H_
