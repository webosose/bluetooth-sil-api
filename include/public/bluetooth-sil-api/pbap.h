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

const std::string BLUETOOTH_PROFILE_ID_PBAP = "PBAP";

typedef uint64_t BluetoothPbapAccessRequestId;
const BluetoothPbapAccessRequestId BLUETOOTH_PBAP_ACCESS_REQUEST_ID_INVALID = 0;


/**
 * @brief List of vCard filters.
 */
typedef std::vector<std::string> BluetoothPbapVCardFilterList;

typedef std::map<std::string, std::string> BluetoothPbapVCardList;

/**
 * @brief Bluetooth Pbap Application Parameters
 *
 *        Describes Pbap Application Parameters.
 */

class BluetoothPbapApplicationParameters
{
public:

	enum ApplicationParameters
	{
		/**
		 * @brief Current folder path of connected server device(PBAP)
		 *
		 *        Type: std::string
		 *        Access: Device (read/write)
		 **/
		FOLDER,
		/**
		 * @brief The primary version counter shall increment on every completion of
		 *        changes to any of the properties in the vCards as well as on insertion
		 *        or removal of entries.(PBAP)
		 *
		 *        Type: std::string
		 *        Access: Device (read)
		 **/
		PRIMARY_COUNTER,
		/**
		 * @brief The secondary version counter shall only increment on every completion of
		 *        changes to the vCard's N, FN, TEL, EMAIL, MAILER, ADR, X-BT-UCI properties
		 *        as well as on insertion or removal of entries.(PBAP)
		 *
		 *        Type: std::string
		 *        Access: Device (read)
		 **/
		SECONDERY_COUNTER,
		/**
		 * @brief The PSE shall return the DatabaseIdentifier application parameter containing
		 *        the unique database identifier of the PSE.(PBAP)
		 *
		 *        Type: std::string
		 *        Access: Device (read)
		 **/
		DATABASE_IDENTIFIER,
		/**
		 * @brief Indicate support for fixed image size.(PBAP)
		 *
		 *        The default value is false.
		 *
		 *        Type: bool
		 *        Access: Device (read)
		 **/
		FIXED_IMAGE_SIZE
	};

	/**
	 * @brief Default c'tor
	 */
	BluetoothPbapApplicationParameters() : fixedImageSize(0)
	{
	}

	std::string getFolder() const { return folder; }
	std::string getPrimaryCounter() const { return primaryCounter; }
	std::string getSecondaryCounter() const { return secondaryCounter; }
	std::string getDataBaseIdentifier() const { return databaseIdentifier; }
	bool getFixedImageSize() { return fixedImageSize; }


	void setFolder(const std::string &folder) { this->folder = folder; }
	void setPrimaryCounter(const std::string &primaryCounter) { this->primaryCounter = primaryCounter; }
	void setSecondaryCounter(const std::string &secondaryCounter) { this->secondaryCounter = secondaryCounter; }
	void setDataBaseIdentifier(const std::string &databaseIdentifier) { this->databaseIdentifier = databaseIdentifier; }
	void setFixedImageSize(bool fixedImageSize) { this->fixedImageSize = fixedImageSize; }

private:
	std::string folder;
	std::string primaryCounter;
	std::string secondaryCounter;
	std::string databaseIdentifier;
	bool fixedImageSize;
};


typedef std::function<void(BluetoothError, uint16_t size)>
BluetoothPbapGetSizeResultCallback;

typedef std::function<void(BluetoothError, BluetoothPbapVCardList&)>
BluetoothPbapVCardListResultCallback;

typedef std::function<void(BluetoothError, std::list<std::string>filters)>
BluetoothPbapListFiltersResultCallback;

typedef std::function<void(BluetoothError, BluetoothPbapApplicationParameters &applicationParams)>
BluetoothPbapApplicationParameterCallback;

typedef std::function<void(BluetoothError, std::string)>
BluetoothGetPhoneBookResultCallback;

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
	virtual void profilePropertiesChanged(const std::string &adapterAddress, const std::string &address, BluetoothPbapApplicationParameters &properties) { }
	/**
	 * @brief The method is called when PhoneBook transfer status changes
	 *
	 * @param adapterAddress adapterAddress to which device is connected
	 * @param address Address of the device
	 * @param destinationPath path where Phonebook is getting stored
	 * @param objectPath unique key for each transfer
	 * @param state state of current transfer
	 */
	virtual void transferStatusChanged(const std::string &adapterAddress, const std::string &address, const std::string &destinationPath, const std::string &objectPath, const std::string &state) { }
};

/**
 * @brief Interface to abstract the operations for the PBAP bluetooth profile.
 */
class BluetoothPbapProfile
{
public:
	BluetoothPbapProfile() : observer(0) { }

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
	virtual void getPhoneBookProperties(const std::string &address, BluetoothPbapApplicationParameterCallback callback) = 0;

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

	/**
	 * @brief This method will fetch vcf file from PSE device.
	 *
	 *        This method is only for the client side of PBAP(PCE) Role.
	 *
	 * @param address Address of the remote device
	 * @param destinationFile Destination file name
	 * @param vCardHandle vcf handle of the vcf entries
	 * @param vCardVersion vcard version vcard21 or vcard30
	 * @param vCardFilters vcard supported filters
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void pullvCard (const std::string &address, const std::string &destinationFile, const std::string &vCardHandle, const std::string &vCardVersion, BluetoothPbapVCardFilterList &vCardFilters, BluetoothResultCallback callback) = 0;
	/**
	 * @brief This method will fetch phonebook  from PSE device.
	 *
	 *        This method is only for the client side of PBAP(PCE) Role.
	 *
	 * @param address Address of the remote device
	 * @param destinationFile Destination file name
	 * @param vCardVersion vcard version vcard21 or vcard30
	 * @param vCardFilters List of Vcard fileds which needs to be downloaded
	 * @param startIndex Index from which phonebook needs to be downloaded
	 * @param maxCount max Number of item which required to be downloaded
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void pullPhoneBook(const std::string &address,const std::string &destinationFile,const std::string &vCardVersion,BluetoothPbapVCardFilterList &vCardFilters, const uint16_t &startIndex, const uint16_t &maxCount, BluetoothGetPhoneBookResultCallback callback) = 0;
	/**
	 * @brief This method will Search for entries matching the given condition and return an array of vcard-listing data from PSE device.
	 *
	 *        This method is only for the client side of PBAP(PCE) Role.
	 *
	 * @param address Address of the remote device
	 * @param searchOrder Order for search vcf entries
	 * @param searchKey key for search vcf entries
	 * @param searchValue vlaue for search vcf entries
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void searchPhoneBook (const std::string &address, const std::string &searchOrder, const std::string &searchKey, const std::string &searchValue, BluetoothPbapVCardListResultCallback callback) = 0;

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
