// Copyright (c) 2015-2019 LG Electronics, Inc.
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

#ifndef BLUETOOTH_SIL_ADAPTER_H_
#define BLUETOOTH_SIL_ADAPTER_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

#include <unordered_map>
#include "uuid.h"

class BluetoothLeServiceUuid;
class BluetoothLeServiceData;
class BluetoothManufacturerData;
class BluetoothLeDiscoveryFilter;

/**
 * @brief Defines the default format for BLE advertising data.
 */
typedef std::vector<uint8_t> BluetoothLowEnergyData;

/**
 * @brief Defines the default format for BLE mask.
 */
typedef std::vector<uint8_t> BluetoothLowEnergyMask;

/**
 * @brief Defines the default format for a BLE service. This includes a string UUID and
 *        an array of service data.
 */
typedef std::unordered_map<std::string, BluetoothLowEnergyData> BluetoothLowEnergyServiceList;

/**
 * @brief List of WoBLE triggering device's MAC address.
 */
typedef std::vector<std::string> BluetoothWoBleTriggerDeviceList;

/**
 * @brief Callback which is called to provide the result of BluetoothAdapter::getTraceStatus
 */
typedef std::function<void(BluetoothError, bool stackTraceEnabled, bool snoopTraceEnabled, int stackTraceLevel,
								const std::string &stackLogPath, const std::string &snoopLogPath, bool IsTraceLogOverwrite)> BluetoothTraceStatusCallback;

/**
 * @brief Defines the format for proprietary advertising data with vendor proprietary
 *        advertising type or undefined type in sil-api
 */
struct ProprietaryData
{
	uint8_t type;
	std::vector<uint8_t> data;
} ;

typedef std::vector<ProprietaryData> ProprietaryDataList;

/**
 * @brief List of the service UUID for BLE discovery filter which identifies
 *        the bluetooth gatt services running on the device.
 */
typedef std::vector<std::string> BluetoothBleDiscoveryUuidFilterList;

/**
 * @brief Defines the format for advertiser setting parameters.
 *
 * @param connectable True if advertisements can form a two way connection with a device,
 *        false otherwise.
 * @param txPower Transmission power level.
 * @param minInterval Minimum advertising interval.
 * @param maxInterval Maximum advertising interval.
 * @param timeout Connection timeout
 */
struct AdvertiseSettings
{
    bool connectable;
    uint8_t txPower;
    uint16_t minInterval;
    uint16_t maxInterval;
    uint16_t timeout;
};

/**
 * @brief Defines the format for advertise data parameters.
 *
 * @param includeTxPower True if specific TxPower to be included, false otherwise
 * @param includeName True to include the local device name, false otherwise.
 * @param manufacturerData Array of bytes to be packed into advertisement.
 * @param services Services to be advertised - a service has a uuid and array of service data.
 * @param proprietaryData Array of proprietary data list for advertisement data.
 */
struct AdvertiseData
{
    bool includeTxPower;
    bool includeName;
    BluetoothLowEnergyData manufacturerData;
    BluetoothLowEnergyServiceList services;
    ProprietaryDataList proprietaryData;
};

/**
 * @brief Type of scan to run
 */
enum TransportType
{
	BT_TRANSPORT_TYPE_NONE = 0x00,
	BT_TRANSPORT_TYPE_BR_EDR = 0x01,
	BT_TRANSPORT_TYPE_LE = 0x02,
	BT_TRANSPORT_TYPE_DUAL = 0x03,
};

/**
 * @brief First level of filtering for finding Bluetooth devices and services.
 */
enum InquiryAccessCode
{
	BT_ACCESS_CODE_NONE = 0x00,
	BT_ACCESS_CODE_LIMIT = 0x01,
	BT_ACCESS_CODE_GENERAL = 0x02
};

/**
 * @brief Type of trace
 */
enum TraceType
{
       BT_TRACE_TYPE_STACK = 0,
       BT_TRACE_TYPE_SNOOP
};

/**
 * @brief Callback which is called to provide the result of unregisterAdvertiser and startAdvertising.
 */
typedef std::function<void(BluetoothError)> AdvertiserStatusCallback;

/**
 * @brief Callback which is called to provide the result of registerAdvertiser.
 * @param advertiserId Identifier of BLE multiple advertiser.
 */
typedef std::function<void(BluetoothError, uint8_t advertiserId)> AdvertiserIdStatusCallback;

/**
 * @brief Defines the default format for a BLE service. This includes a string UUID and
 *        an array of service data.
 */
typedef std::unordered_map<std::string, BluetoothLowEnergyData> BluetoothLowEnergyServiceList;

/**
 * @brief List of HCI's command or event parameter. Each parameter is an integer number of bytes.
 */
typedef std::vector<uint16_t> BluetoothHCIParameterList;

/**
 * @brief Callback which is called to provide the result of BluetoothAdapter::sendHciCommand
 *        This is for HCI Event packet. eventCode means HCI Event code and BluetoothHCIParameterList
 *        means Event parameters in this Callback.
 */
typedef std::function<void(BluetoothError, uint16_t eventCode, BluetoothHCIParameterList)> BluetoothHCIEventCallback;

/**
 * @brief Bluetooth Low Energy Service UUID
 *
 *        Describes Bluetooth Low Energy Service UUID
 */
class BluetoothLeServiceUuid
{
public:
	/**
	 * @brief Default c'tor
	 */
	BluetoothLeServiceUuid()
	{
	}

	/**
	 * @brief Copy c'tor
	 * @param other Other LE service UUID object to copy from
	 */
	BluetoothLeServiceUuid(const BluetoothLeServiceUuid &other) :
		uuid(other.getUuid()),
		mask(other.getMask())
	{
	}

	/**
	 * @brief Retrieve the UUID of the service
	 * @return uuid UUID of the service
	 */
	std::string getUuid() const { return uuid; }

	/**
	 * @brief Retrieve the mask of the service
	 * @return mask Mask of the service
	 */
	std::string getMask() const { return mask; }

	/**
	 * @brief Set the UUID of the service
	 *
	 * @param uuid UUID of the service
	 */
	void setUuid(std::string uuid) { this->uuid = uuid; }

	/**
	 * @brief Set the mask of the service
	 *
	 * @param mask mask of the service
	 */
	void setMask(std::string mask) { this->mask = mask; }

private:
	std::string uuid;
	std::string mask;
};

/**
 * @brief Bluetooth Low Energy Service Data
 *
 *        Describes Bluetooth Low Energy Service Data
 */
class BluetoothLeServiceData
{
public:
	/**
	 * @brief Default c'tor
	 */
	BluetoothLeServiceData()
	{
	}

	/**
	 * @brief Copy c'tor
	 * @param other Other LE service UUID object to copy from
	 */
	BluetoothLeServiceData(const BluetoothLeServiceData &other) :
		uuid(other.getUuid()),
		data(other.getData()),
		mask(other.getMask())
	{
	}

	/**
	 * @brief Retrieve the UUID of the service data
	 * @return uuid UUID of the service
	 */
	std::string getUuid() const { return uuid; }

	/**
	 * @brief Retrieve the data of the service data
	 * @return data Data of the service
	 */
	BluetoothLowEnergyData getData() const { return data; }

	/**
	 * @brief Retrieve the mask of the service data
	 * @return mask mask of the service data
	 */
	BluetoothLowEnergyMask getMask() const { return mask; }

	/**
	 * @brief Set the UUID of the service
	 *
	 * @param uuid UUID of the service
	 */
	void setUuid(std::string uuid) { this->uuid = uuid; }

	/**
	 * @brief Set the data of the service
	 *
	 * @param data data of the service
	 */
	void setData(BluetoothLowEnergyData data) { this->data = data; }

	/**
	 * @brief Set the mask of the service
	 *
	 * @param mask mask of the service
	 */
	void setMask(BluetoothLowEnergyMask mask) { this->mask = mask; }

private:
	std::string uuid;
	BluetoothLowEnergyData data;
	BluetoothLowEnergyMask mask;
};

/**
 * @brief Bluetooth Manufacturer Data
 *
 *        Describes Bluetooth Manufacturer Data
 */
class BluetoothManufacturerData
{
public:
	/**
	 * @brief Default c'tor
	 */
	BluetoothManufacturerData() :
		id(-1)
	{
	}

	/**
	 * @brief Copy c'tor
	 * @param other Other LE service UUID object to copy from
	 */
	BluetoothManufacturerData(const BluetoothManufacturerData &other) :
		id(other.getId()),
		data(other.getData()),
		mask(other.getMask())
	{
	}

	/**
	 * @brief Retrieve the ID of the manufacturer data
	 * @return id ID of the manufacturer data
	 */
	int32_t getId() const { return id; }

	/**
	 * @brief Retrieve the data of the manufacture
	 * @return data Data of the manufacturer
	 */
	BluetoothLowEnergyData getData() const { return data; }

	/**
	 * @brief Retrieve the mask of the manufacturer data
	 * @return mask mask of the manufacturer data
	 */
	BluetoothLowEnergyMask getMask() const { return mask; }

	/**
	 * @brief Set the ID of the manufacturer data
	 *
	 * @param ID ID of the manufacturer data
	 */
	void setId(int32_t id) { this->id = id; }

	/**
	 * @brief Set the data of the manufacturer
	 *
	 * @param data data of the manufacturer data
	 */
	void setData(BluetoothLowEnergyData data) { this->data = data; }

	/**
	 * @brief Set the mask of the manufacturer data
	 *
	 * @param mask mask of the manufacturer data
	 */
	void setMask(BluetoothLowEnergyMask mask) { this->mask = mask; }

private:
	int32_t id;
	BluetoothLowEnergyData data;
	BluetoothLowEnergyMask mask;
};

/**
 * @brief Bluetooth Low Energy Scan Filter
 *
 *        Describes Bluetooth Low Energy Scan Filter
 */
class BluetoothLeDiscoveryFilter
{
public:
	/**
	 * @brief Default c'tor
	 */
	BluetoothLeDiscoveryFilter()
	{
	}

	/**
	 * @brief Copy c'tor
	 * @param other Other LE service UUID object to copy from
	 */
	BluetoothLeDiscoveryFilter(const BluetoothLeDiscoveryFilter &other) :
		address(other.getAddress()),
		name(other.getName()),
		serviceUuid(other.getServiceUuid()),
		serviceData(other.getServiceData()),
		manufacturerData(other.getManufacturerData())
	{
	}

	/**
	 * @brief Retrieve the device address
	 * @return address the device address
	 */
	std::string getAddress() const { return address; }

	/**
	 * @brief Retrieve the device name
	 * @return name the device name
	 */
	std::string getName() const { return name; }

	/**
	 * @brief Retrieve the service uuid object
	 * @return serviceUuid the service uuid object
	 */
	BluetoothLeServiceUuid getServiceUuid() const { return serviceUuid; }

	/**
	 * @brief Retrieve the service data object
	 * @return serviceData the service data object
	 */
	BluetoothLeServiceData getServiceData() const { return serviceData; }

	/**
	 * @brief Retrieve the manufacturer Data
	 * @return manufacturerData the manufacturer Data
	 */
	BluetoothManufacturerData getManufacturerData() const { return manufacturerData; }

	/**
	 * @brief Check if filter is empty
	 * @return True if filter is empty else False
	 */
	bool isFilterEmpty() const
	{
		if ((getServiceUuid().getUuid().empty()) && (getServiceData().getUuid().empty())  &&
		(getManufacturerData().getId() <= 0) && (getAddress().empty()) && (getName().empty()))
		{
			return true;
		}
		return false;
	}

	/**
	 * @brief Set the device address
	 *
	 * @param address the device address
	 */
	void setAddress(std::string address) { this->address = address; }

	/**
	 * @brief Set the device name
	 *
	 * @param name the device name
	 */
	void setName(std::string name) { this->name = name; }

	/**
	 * @brief Set the service uuid object
	 *
	 * @param serviceUuid the service uuid object
	 */
	void setServiceUuid(BluetoothLeServiceUuid serviceUuid) { this->serviceUuid = serviceUuid; }

	/**
	 * @brief Set the service data object
	 *
	 * @param serviceData the service data object
	 */
	void setServiceData(BluetoothLeServiceData serviceData) { this->serviceData = serviceData; }

	/**
	 * @brief Set the manufacturer Data
	 *
	 * @param manufacturerData the manufacturer Data
	 */
	void setManufacturerData(BluetoothManufacturerData manufacturerData) { this->manufacturerData = manufacturerData; }

private:
	std::string address;
	std::string name;
	BluetoothLeServiceUuid serviceUuid;
	BluetoothLeServiceData serviceData;
	BluetoothManufacturerData manufacturerData;
};


/**
 * @brief This interface defines the common operations available for a Bluetooth adapter.
 *        It offers access to available profiles which are handled through the separate
 *        BluetoothProfile class.
 */
class BluetoothAdapter
{
public:
	/**
	 * @brief Default C'tor
	 */
	BluetoothAdapter() : observer(0) { }

	/**
	 * @brief D'tor
	 */
	virtual ~BluetoothAdapter() = default;

	/**
	 * @brief Register a observer for the adapter. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 *        A subsequent call will override the currently registered observer.
	 *
	 * @param observer Observer instance to register.
	 */
	virtual void registerObserver(BluetoothAdapterStatusObserver *observer)
	{
		this->observer = observer;
	}

	/**
	 * @brief Perform a request to the SIL to retrieve all properties of the Bluetooth
	 *        adapter. The result of the operation is handed back with the supplied
	 *        callback function.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after getAdapterProperties has returned to the caller
	 *        or before.
	 *
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void getAdapterProperties(BluetoothPropertiesResultCallback callback) = 0;

	/**
	 * @brief Retrieve a specific property of the Bluetooth Adapter.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after getAdapterProperty has returned to the caller
	 *        or before.
	 *
	 * @param type Property of the adapter to retrieve from the SIL.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void getAdapterProperty(BluetoothProperty::Type type,
	                                BluetoothPropertyResultCallback callback) = 0;

	/**
	 * @brief Set a specific property of the adapter.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after setAdapterProperty has returned to the caller
	 *        or before.
	 *
	 * @param property Property which should be set.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void setAdapterProperty(const BluetoothProperty &property,
	                                BluetoothResultCallback callback) = 0;

	/**
	 * @brief Set one or more properties of the adapter.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after getAdapterProperties has returned to the caller
	 *        or before.
	 *
	 * @param properties Properties which should be set.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void setAdapterProperties(const BluetoothPropertiesList &properties,
	                                  BluetoothResultCallback callback) = 0;

	/**
	 * @brief Enable the Bluetooth adapter. This will perform internally all actions needed
	 *        to bring the adapter into an active state. Afterwards the adapter is ready for
	 *        discovery requests, connection attempts, etc.
	 *
	 *        When the operation has finished the new adapter state is notified through
	 *        the adapterStateChanged method of the BluetoothAdapterStatusObserver interface.
	 *
	 * @return Possible error of the operation
	 */
	virtual BluetoothError enable() = 0;

	/**
	 * @brief Disable the Bluetooth adapter.
	 *
	 *        This will perform internally all actions needed to disable the Bluetooth
	 *        adapter. All discovery devices will become invalid once the new adapter
	 *        state is notified to the user through the adapterStateChanged method
	 *        of the BluetoothAdapterStatusObserver interface.
	 *
	 * @return Possible error of the operation
	 */
	virtual BluetoothError disable() = 0;

	/**
	 * @brief Start the discovery of devices.
	 *
	 *        If a discovery timeout is configured in the service layer, the discovery
	 *        will only be active during the configured timeout.
	 *
	 *        Newly found devices will be notified to the user through the deviceFound
	 *        method of the BluetoothAdapterStatusObserver interface.
	 *
	 *        If the Bluetooth adapter is disabled a call to this method will fail.
	 *
	 *        If a discovery is already ongoing, a call will succeed but does nothing.
	 *
	 * @return Possible error of the operation
	 */
	virtual BluetoothError startDiscovery() = 0;

	/**
	 * @brief Start the device discovery with parameters.
	 *
	 *        Filter the type of device and the inquiry access code for internal use.
	 *
	 * @param type Type of scan to run. User of this API can refer to "TransportType"
	 *        enumeration to find out the possible candidate.
	 * @param code Inquiry access code for distinguishing giac/liac.
	 * @return Possible error of the operation
	 */
	virtual BluetoothError startDiscovery(TransportType type, InquiryAccessCode code)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief Cancel a ongoing discovery process.
	 *
	 *        If the Bluetooth adapter is disabled a call to this method will fail.
	 *
	 *        If no device discovery process is ongoing, a call to this method will
	 *        succeed but does nothing.
	 *
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void cancelDiscovery(BluetoothResultCallback callback) = 0;

	/**
	 * @brief Add the LE discovery filter
	 *
	 * @param filter LE discovery filter
	 *
	 * @return Unique ID of BLE discovery filter. 1 and over.
	 */
	virtual int32_t addLeDiscoveryFilter(const BluetoothLeDiscoveryFilter &filter)
	{
		return -1;
	}

	/**
	 * @brief Remove the LE discovery filter
	 *
	 * @param scanId Unique ID of BLE discovery filter
	 *
	 * @return Possible error of the operation
	 */
	virtual BluetoothError removeLeDiscoveryFilter(uint32_t scanId)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief Check if already found devices matched LE filter criteria
	 *
	 * @param filter LE discovery filter
	 * @param scanId Unique ID of BLE discovery filter
	 */
	virtual void matchLeDiscoveryFilterDevices(const BluetoothLeDiscoveryFilter &filter, uint32_t scanId) { }

	/**
	 * @brief Start the LE discovery of devices
	 *
	 * @return Possible error of the operation
	 */
	virtual BluetoothError startLeDiscovery()
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief Cancel an ongoing LE discovery process
	 *
	 * @param scanId Unique ID of BLE discovery
	 *
	 * @return Possible error of the operation
	 */
	virtual BluetoothError cancelLeDiscovery()
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief Start the LE discovery of devices with scan ID
	 *
	 *        NOTE: This method allows user to initiate multiple BLE discovery using
	 *              unique scan ID.
	 *
	 *        If a discovery timeout is configured in the service layer, discovery
	 *        will only be active during the configured timeout.
	 *
	 *        Newly found devices will be notified to the user through the leDeviceFoundByScanId
	 *        method of the BluetoothAdapterStatusObserver interface.
	 *
	 *        If the Bluetooth adapter is disabled, a call to this method will fail.
	 *
	 *        If a discovery is already ongoing, a call will succeed but does nothing
	 *        Also, discovery results retrieved by each ScanID will be delivered to the
	 *        respective client through leDeviceFoundByScanId.
	 *
	 * @param scanId Unique ID of BLE discovery
	 * @param uuids List of service UUID for BLE discovery filter which identifies the
	 *        bluetooth gatt services running on the device.
	 *        If the size of uuids is 0, filter is not used.
	 *
	 * @return Possible error of the operation
	 */
	virtual BluetoothError startLeDiscovery(uint32_t scanId, BluetoothBleDiscoveryUuidFilterList uuids)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief Cancel an ongoing discovery process with scan ID
	 *
	 *        If the Bluetooth adapter is disabled, a call to this method will fail.
	 *
	 *        If no device discovery process is ongoing a call to this method will
	 *        succeed but does nothing.
	 *
	 *        If cancelLeDiscovery is called when multiple startLeDiscovery is ongoing,
	 *        the discovery that matches the scan ID is canceled.
	 *
	 * @param scanId Unique ID of BLE discovery
	 *
	 * @return Possible error of the operation
	 */
	virtual BluetoothError cancelLeDiscovery(uint32_t scanId)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief Retrieve all properties of a device identified by its address.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after getAdapterProperties has returned to the caller
	 *        or before.
	 *
	 * @param address Address of the device to retrieve the properties for.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */

	virtual void getDeviceProperties(const std::string &address,
	                                 BluetoothPropertiesResultCallback callback) = 0;
	/**
	 * @brief Set a specific property of the device.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after setDeviceProperty has returned to the caller
	 *        or before.
	 *
	 * @param address Address of the device to set the property for.
	 * @param property Property which should be set.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void setDeviceProperty(const std::string &address, const BluetoothProperty &property,
	                                BluetoothResultCallback callback) = 0;

	/**
	 * @brief Set one or more properties of the device.
	 *
	 *        NOTE: The callback function will be called either after
	 *              setting all properties is done or when there is a failure
	 *              with setting any property.
	 *
	 * @param address Address of the device to set the property for.
	 * @param properties Properties which should be set.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void setDeviceProperties(const std::string &address, const BluetoothPropertiesList &properties,
	                                  BluetoothResultCallback callback) = 0;

	/**
	 * @brief Start sniff mode of the device.
	 *
	 * @param address Address of the device to start sniff
	 * @param minInterval Minimum sniff interval
	 * @param maxInterval Maximum sniff interval
	 * @param attempt Sniff anchor points which are spaced regularly with a sniff interval
	 * @param timeout The amount of time to wait before the sleep state within the sniff interval
	 * @return Possible error of the operation
	 */
	virtual BluetoothError startSniff(const std::string &address, int minInterval, int maxInterval, int attempt, int timeout)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief Stop sniff mode of the device.
	 *
	 * @param address Address of the device to stop sniff
	 * @return Possible error of the operation
	 */
	virtual BluetoothError stopSniff(const std::string &address)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief Retrieve a instance of a profile class. The profile is identified by
	 *        its id.
	 *
	 * @param profileId Id of the profile to retrieve.
	 * @return Instance of the profile if supported and available. Null otherwise.
	 */
	virtual BluetoothProfile *getProfile(const std::string& profileId)
	{
		return 0;
	}

	/**
	 * @brief Start the pairing process with a specific remote device. Additional
	 *        required information or actions are asked through the registered
	 *        observer instance.
	 *
	 *        Once the pairing process is done or has failed the supplied callback
	 *        is called with the corresponding error code.
	 *
	 * @param address Address of the device to pair with
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void pair(const std::string &address, BluetoothResultCallback callback) = 0;

	/**
	 * @brief Confirm the pairing with a remote device or disallow the connection. Should
	 *        be only called after the SIL asked for it by calling the displayPairingConfirmation
	 *        method of the registered observer.
	 *
	 *        This method is only meant to be called when a pairing operation is currently
	 *        ongoing in response to the method displayPairingConfirmation of the
	 *        BluetoothAdapterObserver class.
	 *
	 *        Only used with devices supporting Bluetooth version >= 2.1 and Simple Secure
	 *        Pairing (SSP).
	 *
	 * @param address Address of the remote device
	 * @param accept True to accept the pairing or False to deny.
	 * @return BLUETOOTH_ERROR_NONE when operation was successfull another error code otherwise.
	 */
	virtual BluetoothError supplyPairingConfirmation(const std::string &address, bool accept) = 0;

	/**
	 * @brief Supply PIN for a pairing attempt after the SIL has asked for it by calling
	 *        the requestPairingSecret method of the registered observer with the secret
	 *        type BLUETOOTH_PAIRING_SECRET_TYPE_PIN.
	 *
	 *        This method is only meant to be called when a pairing operation is currently
	 *        ongoing in response to the method requestPairingSecret of the
	 *        BluetoothAdapterObserver class.
	 *
	 * @param address Address of the remote device
	 * @param pin PIN used to finish the pairing operation.
	 * @return BLUETOOTH_ERROR_NONE when operation was successfull another error code otherwise.
	 */
	virtual BluetoothError supplyPairingSecret(const std::string &address, const std::string &pin) = 0;

	/**
	 * @brief Supply passkey for a pairing attempt after the SIL has asked for it by calling
	 *        the requestPairingSecret method of the registered observer with the secret
	 *        type BLUETOOTH_PAIRING_SECRET_TYPE_PASSKEY.
	 *
	 *        This method is only meant to be called when a pairing operation is currently
	 *        ongoing in response to the method requestPairingSecret of the
	 *        BluetoothAdapterObserver class.
	 *
	 * @param address Address of the remote device
	 * @param passkey Passkey used to finish the pairing operation.
	 * @return BLUETOOTH_ERROR_NONE when operation was successfull another error code otherwise.
	 */
	virtual BluetoothError supplyPairingSecret(const std::string &address, BluetoothPasskey passkey) = 0;

	/**
	 * @brief Unpair with a specific remote device. The device will disappear and the
	 *        deviceRemoved method of the observer is called to indicate this.
	 *
	 * @param address Address of the remote device
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void unpair(const std::string &address, BluetoothResultCallback callback) = 0;

	/**
	 * @brief Cancel an ongoing pairing process for a device with the given address.
	 *
	 *        Any cleanup for the cancelled pairing operation should be done when the
	 *        callback provided to the pair method call is called and not within the
	 *        callback passed to this method.
	 *
	 * @param address Address of the device which pairing attempt should be canceled.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void cancelPairing(const std::string &address, BluetoothResultCallback callback) = 0;

	/**
	 * Test function to poke mock SIL to initiate IncomingPair
	 * @param address Address of the device from which incoming pairing attempt should be initiated.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void testRequestIncomingPair(const std::string &address, BluetoothResultCallback callback) { }

	/**
	 * @brief Will be deprecated
	 */
	virtual void configureAdvertisement(bool connectable, bool includeTxPower, bool includeName, BluetoothLowEnergyData manufacturerData,
			                    BluetoothLowEnergyServiceList services, BluetoothResultCallback callback, uint8_t TxPower = 0,
			                    BluetoothUuid solicitedService128 = BluetoothUuid("0"))
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Set configuration of a BLE advertisement.
	 *
	 * @param connectable True if advertisements can form a two way connection with a device,
	 *        false otherwise.
	 * @param includeTxPower True if specific TxPower to be included, false otherwise
	 * @param includeName True to include the local device name, false otherwise.
	 *
	 *        NOTE: including the device name will take up space in the manufacturer
	 *        data, limiting the size of the advertisement data.
	 *
	 * @param isScanResponse If it is true, SIL should make Scan_Response_Data.
	 *        Otherwise, SIL Should not do Scan_Response_Data.
	 * @param manufacturerData Array of bytes to be packed into advertisement.
	 * @param services Services to be advertised - a service has a uuid and array of service data.
	 * @param dataList Array of proprietary data list for advertisement data.
	 *
	 *        NOTE: ProprietaryDataList is used for advertising data with vendor proprietary
	 *        advertising type or undefined type as parameter (ex. LE Bluetooth Devices (0x1B))"
	 *
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 * @param TxPower 2's complement value of transmission power. This is an optional parameter.
	 * @param solicitedService128 128-bit solicitation service uuid. This is an optional parameter.
	 */
	virtual void configureAdvertisement(bool connectable, bool includeTxPower, bool includeName, bool isScanResponse,
			                    BluetoothLowEnergyData manufacturerData,
			                    BluetoothLowEnergyServiceList services, ProprietaryDataList dataList,
			                    BluetoothResultCallback callback, uint8_t TxPower = 0,
			                    BluetoothUuid solicitedService128 = BluetoothUuid("0"))
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Set configuration of a BLE advertisement.
	 *
	 * @param discoverable True if advertisements can form a two way connection with a device,
	 *        false otherwise.
	 * @param connectable True if advertisements can form a two way connection with a device,
	 *        false otherwise.
	 */
	virtual bool configureAdvertisement(bool discoverable, bool connectable) { return false; }

	/**
	 * @brief Start the BLE advertising.
	 *
	 * @param callback Callback function which is called when
	 *        the operation is done or has failed.
	 */
	virtual void startAdvertising(BluetoothResultCallback callback) = 0;

	/**
	 * @brief Stop the BLE advertising.
	 *
	 * @param callback Callback function which is called when
	 *        the operation is done or has failed.
	 */
	virtual void stopAdvertising(BluetoothResultCallback callback) = 0;

	/**
	 * @brief Register new advertiser.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after advertiserId has returned to the caller
	 *        or before.
	 *
	 * @param callback Callback function which is called when
	 *        the operation is done or has failed.
	 */
	virtual void registerAdvertiser(AdvertiserIdStatusCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, 0);
	}

	/**
	 * @brief Unregister existing advertiser.
	 *
	 * @param advertiserId Identifier of the BLE multiple advertiser to retrieve.
	 * @param callback Callback function which is called when
	 *        the operation is done or has failed.
	 */
	virtual void unregisterAdvertiser(uint8_t advertiserId, AdvertiserStatusCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Set advertising parameters.
	 *
	 * @param advertiserId Identifier of BLE multiple advertiser.
	 * @param parameters advertiser setting parameters.
	 * @param callback Callback function which is called when
	 *        the operation is done or has failed.
	 */
	virtual void setAdvertiserParameters(uint8_t advertiserId, AdvertiseSettings parameters, AdvertiserStatusCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Set scan request data or scan response data for BLE multiple advertising.
	 *
	 * @param advertiserId Identifier of BLE multiple advertiser.
	 * @param isScanResponse If user of this API wants to set scan request data, set isScanResponse to true.
	 *        If user of this API wants to set scan response data, set isScanResponse to false.
	 * @param data Data to be packed into advertisement.
	 * @param callback Callback function which is called when
	 *        the operation is done or has failed.
	 */
	virtual void setAdvertiserData(uint8_t advertiserId, bool isScanResponse, AdvertiseData data, AdvertiserStatusCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Enable advertiser.
	 *
	 * @param advertiserId Identifier of BLE multiple advertiser.
	 * @param timeoutSeconds Advertising timeout seconds.
	 * @param callback Callback function which is called when
	 *        the operation is done or has failed.
	 */
	virtual void enableAdvertiser(uint8_t advertiserId, int timeoutSeconds, AdvertiserStatusCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Disable advertiser.
	 *
	 * @param advertiserId Identifier of BLE multiple advertiser.
	 * @param callback Callback function which is called when
	 *        the operation is done or has failed.
	 */
	virtual void disableAdvertiser(uint8_t advertiserId, AdvertiserStatusCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Start the BLE advertising.
	 *
	 * @param advertiserId Identifier of BLE multiple advertiser.
	 * @param settings Advertiser setting data.
	 * @param advertiseData Data to be packed into scan request.
	 * @param scanResponse Data to be packed into scan response.
	 * @param callback Callback function which is called when
	 *        the operation is done or has failed.
	 */
	virtual void startAdvertising(uint8_t advertiserId, const AdvertiseSettings &settings,
			const AdvertiseData &advertiseData, const AdvertiseData &scanResponse, AdvertiserStatusCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Update firmware of bt hw module
	 *
	 * @param deviceName Full path of device file.
	 * @param fwFileName Full path of firmware file.
	 * @param miniDriverName Full path of mini-driver file if needs.
	 * @param isShared True if shared module
	 */
	virtual BluetoothError updateFirmware(const std::string &deviceName,
			const std::string &fwFileName,
			const std::string &miniDriverName,
			bool isShared) = 0;

	/**
	 * @brief Reset bt hw module
	 *
	 * @param deviceName Full path of device file.
	 * @param isShared True if shared module
	 */
	virtual BluetoothError resetModule(const std::string &deviceName, bool isShared) = 0;

	/**
	 * @brief Send vendor command to device
	 *
	 * @param address Address of the device.
	 * @param command Vendor specific command.
	 */
	virtual void sendVendorCommand(const std::string &address, uint32_t command) { }

	/**
	 * @brief Enalbe WoBLE of the Bluetooth adapter.
	 *
	 * @return Possible error of the operation
	 */
	virtual BluetoothError enableWoBle() { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Disable WoBLE of the Bluetooth adapter.
	 *
	 * @return Possible error of the operation
	 */
	virtual BluetoothError disableWoBle() { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Enalbe WoBLE of the Bluetooth adapter.
	 *
	 * @param isSuspend the power state is suspend or not.
	 * @return Possible error of the operation
	 */
	virtual BluetoothError enableWoBle(bool isSuspend) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Disable WoBLE of the Bluetooth adapter.
	 *
	 * @param isSuspend the power state is suspend or not.
	 * @return Possible error of the operation
	 */
	virtual BluetoothError disableWoBle(bool isSuspend) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Set the devices which trigger WoBLE.
	 *
	 * @param devices List of WoBLE triggering device's MAC address.
	 * @return Possible error of the operation
	 */
	virtual BluetoothError setWoBleTriggerDevices(BluetoothWoBleTriggerDeviceList devices) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Send HCI commands
	 *
	 * @param ogf OpCode Group Field of HCI command packet.
	 * @param ocf OpCode Command Field of HCI command packet.
	 * @param parameters HCI Command Packet's parameters.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed. This handles HCI event packet.
	 */
	virtual void sendHciCommand(int16_t ogf, uint16_t ocf, BluetoothHCIParameterList parameters, BluetoothHCIEventCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, 0, std::vector<uint16_t>());
	}

	/**
	 * @brief Enable the trace of stack and snoop.
	 *
	 * @param type Type of trace
	 * @return Possible error of the operation
	 */
	virtual BluetoothError enableTrace(TraceType type) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Disable the trace of stack and snoop.
	 *
	 * @param type Type of trace
	 * @return Possible error of the operation
	 */
	virtual BluetoothError disableTrace(TraceType type) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Set the stack trace level configuration.
	 *
	 * @param level Level of trace
	 * @return Possible error of the operation
	 */
	virtual BluetoothError setStackTraceLevel(int level) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Set the log path of stack or snoop.
	 *
	 * @param type Type of trace
	 * @param Path Path of log output file (i.e. full path with file name)
	 * @return Possible error of the operation
	 */
	virtual BluetoothError setLogPath(TraceType type, const std::string &path) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Set the log path of stack or snoop.
	 *
	 * @param isOverwirte True if there is a same file and file is overwritten, false otherwise
	 * @return Possible error of the operation
	 */
	virtual BluetoothError setTraceOverwrite(bool isOverwrite) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Set the log path of stack or snoop.
	 *
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed. This handles HCI event packet.
	 */
	virtual void getTraceStatus(BluetoothTraceStatusCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, false, false, 0, std::string(), std::string(), false);
	}

	/**
	 * @brief Enable keep alive of bluetooth stack
	 *
	 * @return Possible error of the operation
	 */
	virtual BluetoothError enableKeepAlive() { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Disable keep alive of bluetooth stack
	 *
	 * @return Possible error of the operation
	 */

	virtual BluetoothError disableKeepAlive() { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Set the Interval of keep alive(Default is 1 sec.)
	 *
	 * @param interval Interval of keep alive
	 * @return Possible error of the operation
	 */
	virtual BluetoothError setKeepAliveInterval(int interval) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Retrieve a instance of a profile class. The profile is identified by
	 *        its id.
	 *
	 * @param profileId Id of the profile to retrieve.
	 * @return Instance of the profile if supported and available. Null otherwise.
	 */
	template<class T>
	T *getProfileAs(const char *profileId)
	{
		BluetoothProfile *profile = getProfile(profileId);

		if (!profile)
		{
			return 0;
		}

		return dynamic_cast<T*>(profile);
	}

protected:
	BluetoothAdapterStatusObserver* getObserver()
	{
		static BluetoothAdapterStatusObserver nullObserver;
		if (!observer)
			return &nullObserver;

		return observer;
	}

	BluetoothAdapterStatusObserver *observer;
};

/**
 * @brief This interface is the base to implement an observer for the Bluetooth
 *        SIL to get notifications when something in the SIL changes.
 */
class BluetoothSILStatusObserver
{
public:
	virtual ~BluetoothSILStatusObserver() {}

	/**
	 * @brief This method is called once the list of available adapters changes
	 *        within the SIL.
	 *
	 *        A user of the SIL will check directly on startup if any adapters
	 *        are available and if not wait for this method being called to
	 *        start using any available adapters.
	 */
	virtual void adaptersChanged() { }
};


#endif
