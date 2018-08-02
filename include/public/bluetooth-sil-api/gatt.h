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

#ifndef BLUETOOTH_SIL_GATT_H_
#define BLUETOOTH_SIL_GATT_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

#include <stdint.h>

#include <set>
#include <vector>
#include <string>
#include <map>

const std::string BLUETOOTH_PROFILE_ID_GATT = "GATT";

class BluetoothGattDescriptor;
class BluetoothGattCharacteristic;
class BluetoothGattService;

/**
 * @brief Describes a Bluetooth GATT value which is represented as a sequence of bytes.
 */
typedef std::vector<uint8_t> BluetoothGattValue;

/**
 * @brief List of Bluetooth GATT descriptors
 */
typedef std::vector<BluetoothGattDescriptor> BluetoothGattDescriptorList;

/**
 * @brief Bluetooth GATT service properties which are a bit field with the length of one byte
 *        See @ref{BluetoothGattService}
 */
typedef uint8_t BluetoothGattCharacteristicProperties;

/**
 * @brief Bluetooth GATT characteristic permissions which are a bit field with the length of one byte.
 *        See @ref{BluetoothGattCharacteristic}
 */
typedef uint8_t BluetoothGattCharacteristicPermissions;

/**
 * @brief Bluetooth GATT descriptor permissions which are a bit field with the length of one byte.
 *        See @ref{BluetoothGattDescriptor}
 */
typedef uint8_t BluetoothGattDescriptorPermissions;

/**
 * @brief List of Bluetooth GATT characteristics
 */
typedef std::vector<BluetoothGattCharacteristic> BluetoothGattCharacteristicList;

/**
 * @brief List of Bluetooth GATT services
 */
typedef std::vector<BluetoothGattService> BluetoothGattServiceList;

/**
 * @brief Callback which is called to provide the result of BluetoothGattProfile::readCharacteristics
 */
typedef std::function<void(BluetoothError, BluetoothGattCharacteristic)> BluetoothGattReadCharacteristicCallback;

/**
 * @brief Callback which is called to provide the result of BluetoothGattProfile::readCharacteristics
 */
typedef std::function<void(BluetoothError, BluetoothGattCharacteristicList)> BluetoothGattReadCharacteristicsCallback;

/**
 * @brief Callback which is called to provide the result of BluetoothGattProfile::readDescriptors
 */
typedef std::function<void(BluetoothError, BluetoothGattDescriptor)> BluetoothGattReadDescriptorCallback;

/**
 * @brief Callback which is called to provide the result of BluetoothGattProfile::readDescriptors
 */
typedef std::function<void(BluetoothError, BluetoothGattDescriptorList)> BluetoothGattReadDescriptorsCallback;
typedef std::function<void(BluetoothError, uint16_t)> BluetoothGattAddCallback;
typedef std::function<void(BluetoothError, uint16_t)> BluetoothConnectCallback;
/**
 * @brief Permissions for GATT characteristics/descriptors.
 *
 *        See Bluetooth Specification Core 4.1 vol 3 Part F chapter 3.2.5
 */
enum BluetoothGattPermission
{
	PERMISSION_NONE = 0x0,
	PERMISSION_READ = 0x01,
	PERMISSION_READ_ENCRYPTED = 0x02,
	PERMISSION_READ_ENCRYPTED_MITM = 0x04,
	PERMISSION_WRITE = 0x08,
	PERMISSION_WRITE_ENCRYPTED = 0x10,
	PERMISSION_WRITE_ENCRYPTED_MITM = 0x20,
	PERMISSION_WRITE_SIGNED = 0x40,
	PERMISSION_WRITE_SIGNED_MITM = 0x80,
};

/**
 * @brief Transport mode for GATT service.
 *
 */
enum BluetoothGattTransportMode
{
	GATT_TRANSPORT_MODE_AUTO = 0x00,
	GATT_TRANSPORT_MODE_BR_EDR = 0x01,
	GATT_TRANSPORT_MODE_LE = 0x02,
	GATT_TRANSPORT_MODE_LE_BR_EDR = 0x03,
};

/**
 * @brief Write type of GATT characteristic/descriptor.
 */
enum WriteType
{
	DEFAULT,
	NO_RESPONSE,
	SIGNED
};

/**
 * @brief Type of GATT application.
 */
enum ApplicationType
{
	SERVER,
	CLIENT,
};

/**
 * @brief Bluetooth GATT descriptor
 *
 *        Describes a GATT descriptor with it's configuration.
 *
 *        NOTE: Any change made on the descriptor object will not have any direct effect. The
 *              class itself is just a data container and need to be supplied to the
 *              BluetoothGattProfile implementation to be updated in the local device database.
 */
class BluetoothGattDescriptor
{
public:
	/**
	 * @brief Default c'tor
	 */
	BluetoothGattDescriptor() :
	    permissions(PERMISSION_NONE),
	    write_type(DEFAULT),
	    handle(0)
	{
	}

   /**
	* @brief Copy c'tor
    * @param other Other descriptor object to copy from
    */
	BluetoothGattDescriptor(const BluetoothGattDescriptor &other) :
	    uuid(other.uuid),
	    value(other.value),
	    permissions(other.permissions),
	    write_type(other.write_type),
	    handle(other.handle)
	{
	}

	/**
	 * @brief Check if the descriptor is valid or not
	 *
	 *        A descriptor is valid when it has a valid UUID assigned.
	 *
	 * @return True if descriptor is valid. False otherwise.
	 */
	bool isValid() const { return uuid.isValid(); }

	/**
	 * @brief Set the UUID of the descriptor
	 * @param uuid UUID if the descriptor
	 */
	void setUuid(const BluetoothUuid &uuid) { this->uuid = uuid; }

	/**
	 * @brief Retrieve the UUID of the descriptor
	 * @return UUID of the descriptor
	 */
	BluetoothUuid getUuid() const { return uuid; }

	/**
	 * @brief Set the value of the descriptor
	 * @param value Value to set
	 */
	void setValue(const BluetoothGattValue &value) { this->value = value; }

	/**
	 * @brief Set permissions of the characteristic
	 * @param permissions Permissions of the characteristic
	 */
	void setPermissions(BluetoothGattDescriptorPermissions permissions) { this->permissions = permissions; }

	/**
	 * @brief Retrieve the permissions of the characteristic
	 * @return Permissions of the characteristic
	 */
	BluetoothGattDescriptorPermissions getPermissions() const { return permissions; }

	/**
	 * @brief Check if the specified permission is set or not
	 * @param perm Permission to check
	 * @return True, if permission is set; False otherwise.
	 */
	bool isPermissionSet(BluetoothGattDescriptorPermissions perm) const { return (permissions & perm) == perm; }

	/**
	 * @brief Retrieve the value of the descriptor
	 * @return Value of the descriptor
	 */
	BluetoothGattValue getValue() const { return value; }

	/**
	 * @brief Set handle of the characteristic
	 * @param handle of the characteristic
	 */
	void setHandle(uint16_t handle) { this->handle = handle; }

	/**
	 * @brief Retrieve the handle of the characteristic
	 * @return handle of the characteristic
	 */
	uint16_t getHandle() const { return handle; }
	/**
	 * @brief set descriptor write type
	 * @param Write type of the descriptor
	 */
	void setWriteType(WriteType type) { this->write_type = type; }

	/**
	 * @brief get descriptor write type
	 * @return Write type of the descriptor
	 */
	WriteType getWriteType() const { return write_type; }
private:
	BluetoothUuid uuid;
	BluetoothGattValue value;
	BluetoothGattDescriptorPermissions permissions;
	uint16_t handle;
	WriteType write_type;
};

/**
 * @brief Bluetooth GATT characteristic
 *
 *        Describes a GATT characteristic with it's configuration.
 *
 *        NOTE: Any change made on the characteristic object will not have any direct effect. The
 *              class itself is just a data container and need to be supplied to the
 *              BluetoothGattProfile implementation to be updated in the local device database.
 */
class BluetoothGattCharacteristic
{
public:
	/**
	 * @brief Properties of the characteristic.
	 *
	 *        See Bluetooth Specification Core 4.1 vol 3 Part G chapter 3.3.1.1
	 */
	enum Property
	{
		PROPERTY_NONE = 0x0,
		PROPERTY_BROADCAST = 0x01,
		PROPERTY_READ = 0x02,
		PROPERTY_WRITE_WITHOUT_RESPONSE = 0x04,
		PROPERTY_WRITE = 0x08,
		PROPERTY_NOTIFY = 0x10,
		PROPERTY_INDICATE = 0x20,
		PROPERTY_AUTHENTICATED_SIGNED_WRITES = 0x40,
		PROPERTY_EXTENDED_PROPERTIES = 0x80
	};

	/**
	 * @brief Default c'tor
	 *
	 *        By default no property and no permission is set.
	 */
	BluetoothGattCharacteristic() :
	    properties(PROPERTY_NONE),
	    permissions(PERMISSION_NONE),
	    write_type(DEFAULT),
	    handle(0)
	{
	}

	/**
	 * @brief Copy c'tor
	 * @param other Other characteristic object to copy from
	 */
	BluetoothGattCharacteristic(const BluetoothGattCharacteristic &other) :
	    uuid(other.uuid),
	    value(other.value),
	    descriptors(other.descriptors),
	    properties(other.properties),
	    permissions(other.permissions),
	    write_type(other.write_type),
	    handle(other.handle)
	{
	}

	/**
	 * @brief Checks if the characteristic is a valid one.
	 *
	 *        Criteria for a characteristic to be valid is that it has a
	 *        UUID assigned.
	 *
	 * @return True if characteristic is valid. False otherwise.
	 */
	bool isValid() const { return uuid.isValid(); }

	/**
	 * @brief Set the UUID for characteristic
	 * @param uuid UUID of the characteristic
	 */
	void setUuid(const BluetoothUuid &uuid) { this->uuid = uuid; }

	/**
	 * @brief Retrieve the UUID of the characteristic
	 * @return UUID of the characteristic
	 */
	BluetoothUuid getUuid() const { return uuid; }

	/**
	 * @brief Set properties for the characteristic
	 * @param properties Properties for the characteristic
	 */
	void setProperties(BluetoothGattCharacteristicProperties properties) { this->properties = properties; }

	/**
	 * @brief Retrieve the properties of the characteristic
	 * @return Properties of the characteristic
	 */
	BluetoothGattCharacteristicProperties getProperties() const { return properties; }

	/**
	 * @brief Check if the specified property is set or not
	 * @param prop Property to check
	 * @return True, if property is set; False otherwise.
	 */
	bool isPropertySet(Property prop) const { return (properties & prop) == prop; }

	/**
	 * @brief Check if the specified property is set or not
	 * @param prop Property to check
	 * @return True, if property is set; False otherwise.
	 */
	bool isPropertySet(BluetoothGattCharacteristicProperties props) const { return (properties & props) == props; }
	/**
	 * @brief Set the value of the characteristic
	 * @param value Value to set
	 */
	void setValue(BluetoothGattValue value) { this->value = value; }

	/**
	 * @brief Return the value of the characteristic
	 * @return Value of the characteristic
	 */
	BluetoothGattValue getValue() const { return value; }

	/**
	 * @brief Set permissions of the characteristic
	 * @param permissions Permissions of the characteristic
	 */
	void setPermissions(BluetoothGattCharacteristicPermissions permissions) { this->permissions = permissions; }

	/**
	 * @brief Retrieve the permissions of the characteristic
	 * @return Permissions of the characteristic
	 */
	BluetoothGattCharacteristicPermissions getPermissions() const { return permissions; }

	/**
	 * @brief Check if the specified permission is set or not
	 * @param perm Permission to check
	 * @return True, if permission is set; False otherwise.
	 */
	bool isPermissionSet(BluetoothGattDescriptorPermissions perm) const { return (permissions & perm) == perm; }

	/**
	 * @brief Add a descriptor to the characteristic
	 * @param descriptor Descriptor to be added
	 */
	void addDescriptor(BluetoothGattDescriptor descriptor) { descriptors.insert(std::pair<BluetoothUuid, BluetoothGattDescriptor>(descriptor.getUuid(), descriptor)); }

	/**
	 * @brief Get the list of descriptors which are part of the characteristic
	 * @return List of descriptors
	 */
	BluetoothGattDescriptorList getDescriptors() const
	{
		std::vector<BluetoothGattDescriptor> result;
		for (auto iter = descriptors.begin(); iter != descriptors.end(); ++iter)
			result.push_back(iter->second);
		return result;
	}

	/**
	 * @brief Get a single descriptor identified by its UUID
	 *
	 * @param uuid UUID of the descriptor to return
	 * @return Descriptor object
	 */
	BluetoothGattDescriptor getDescriptor(const BluetoothUuid &uuid) const
	{
		auto iter = descriptors.find(uuid);
		if (iter == descriptors.end())
			return BluetoothGattDescriptor();

		return iter->second;
	}
	/**
	 * @brief Update the handle of a specific descriptor
	 *
	 *        As a characteristic can't be modified and only the value of a descriptor can change we
	 *        need to allow this for our users.
	 *
	 * @param descriptor UUID of the descriptor the value has be changed for
	 * @param handle Handle of the descriptor
	 * @return True if descriptor was updated successfully, false otherwise.
	 */
	bool updateDescriptorHandle(const BluetoothGattDescriptor &descriptor, const uint16_t &handle)
	{
		auto iter = descriptors.begin();

		for (; iter != descriptors.end(); iter++)
		{
			if (iter->second.getUuid() == descriptor.getUuid() &&
				iter->second.isPermissionSet(descriptor.getPermissions()))
				break;
		}

		if (iter == descriptors.end())
			return false;

		iter->second.setHandle(handle);

		return true;
	}
	/**
	 * @brief Update the value of a specific descriptor
	 *
	 *        As a characteristic can't be modified and only the value of a descriptor can change we
	 *        need to allow this for our users.
	 *
	 * @param descriptor UUID of the descriptor the value has be changed for
	 * @param value New value to set for the characteristic
	 * @return True if descriptor was updated successfully, false otherwise.
	 */
	bool updateDescriptorValue(const BluetoothUuid &descriptor, const BluetoothGattValue &value)
	{
		auto iter = descriptors.begin();

		for (; iter != descriptors.end(); iter++)
		{
			if (iter->second.getUuid() == descriptor)
				break;
		}

		if (iter == descriptors.end())
			return false;

		iter->second.setValue(value);

		return true;
	}

	/**
	 * @brief Set handle of the characteristic
	 * @param handle of the characteristic
	 */
	void setHandle(uint16_t handle) { this->handle = handle; }

	/**
	 * @brief Retrieve the handle of the characteristic
	 * @return handle of the characteristic
	 */
	uint16_t getHandle() const { return handle; }

	/**
	 * @brief set characteristic write type
	 * @param Write type of the characteristic
	 */
	void setWriteType(WriteType type) { this->write_type = type; }

	/**
	 * @brief get characteristic write type
	 * @return Write type of the characteristic
	 */
	WriteType getWriteType() const { return write_type; }
private:
	BluetoothUuid uuid;
	BluetoothGattValue value;
	std::map<BluetoothUuid, BluetoothGattDescriptor> descriptors;
	BluetoothGattCharacteristicProperties properties;
	BluetoothGattCharacteristicPermissions permissions;
	uint16_t handle;
	WriteType write_type;
};

/**
 * @brief Bluetooth GATT service
 *
 *        Describes a GATT service with it's configuration.
 *
 *        NOTE: Any change made on the service object will not have any direct effect. The
 *              class itself is just a data container and need to be supplied to the
 *              BluetoothGattProfile implementation to be updated in the local device database.
 */
class BluetoothGattService
{
public:
	/**
	 * @brief Type of a GATT service.
	 */
	enum Type
	{
		UNKNOWN,
		PRIMARY,
		SECONDARY
	};

	/**
	 * @brief Default c'tor
	 *
	 *        By default the type of the service is set to UNKNOWN
	 */
	BluetoothGattService() :
	    type(UNKNOWN)
	{
	}

	/**
	 * @brief Initialize the GATT service with its type and UUID
	 *
	 * @param type Type of the service
	 * @param uuid UUID of the service
	 */
	BluetoothGattService(Type type, BluetoothUuid uuid) :
	    type(type),
	    uuid(uuid)
	{
	}

	/**
	 * @brief Copy c'tor
	 * @param other Other service object to copy from
	 */
	BluetoothGattService(const BluetoothGattService &other) :
	    uuid(other.uuid),
	    type(other.type),
	    includes(other.includes),
	    characteristics(other.characteristics)
	{
	}

	/**
	 * @brief Checks if the service is valid.
	 *
	 *        Criteria for a service to be valid is that its type is
	 *        correctly set and it has a UUID assigned.
	 *
	 * @return True if service is valid. False otherwise.
	 */
	bool isValid() const { return type != UNKNOWN && uuid.isValid(); }

	/**
	 * @brief Set the UUID of the service
	 *
	 * @param uuid UUID of the service
	 */
	void setUuid(BluetoothUuid uuid) { this->uuid = uuid; }

	/**
	 * @brief Retrieve the UUID of the service
	 *
	 * @return UUID of the servie
	 */
	BluetoothUuid getUuid() const { return uuid; }

	/**
	 * @brief Set the type of the service
	 *
	 * @param type Type of the service
	 */
	void setType(Type type) { this->type = type; }

	/**
	 * @brief Retrieve the type of the service
	 *
	 * @return Type of the service
	 */
	Type getType() const { return type; }

	/**
	 * @brief Include another GATT service
	 *
	 * @param service GATT service object to be included
	 */
	void includeService(BluetoothGattService service) { includes.push_back(service.getUuid()); }

	/**
	 * @brief Include another GATT service
	 *
	 * @param uuid UUID of the GATT service object to be included
	 */
	void includeService(BluetoothUuid uuid) { includes.push_back(uuid); }

	/**
	 * @brief Retrieve a list of included services
	 *
	 * @return List of included services
	 */
	BluetoothUuidList getIncludedServices() const { return includes; }

	/**
	 * @brief Add a characteristic to the service
	 *
	 * @param characteristic Characteristic to be added to the service
	 */
	void addCharacteristic(BluetoothGattCharacteristic characteristic) { characteristics.push_back(characteristic); }
	/**
	 * @brief Update the handle of a specific characteristic
	 *
	 * @param characteristic UUID of the characteristic the value has be changed for
	 * @param handle Handle of the characteristic
	 * @return True if characteristic was updated successfully, false otherwise.
	 */
	bool updateCharacteristicHandle(const BluetoothGattCharacteristic &characteristic, const uint16_t &handle)
	{
		auto iter = characteristics.begin();

		for (; iter != characteristics.end(); iter++)
		{
			if (iter->getUuid() == characteristic.getUuid() &&
				iter->isPermissionSet(characteristic.getPermissions()) &&
				iter->isPropertySet(characteristic.getProperties()))
				break;
		}

		if (iter == characteristics.end())
			return false;

		iter->setHandle(handle);

		return true;
	}
	/**
	 * @brief Update the value of a specific characteristic
	 *
	 * @param characteristic UUID of the characteristic the value has be changed for
	 * @param value New value to set for the characteristic
	 * @return True if characteristic was updated successfully, false otherwise.
	 */
	bool updateCharacteristicValue(const BluetoothUuid &characteristic, const BluetoothGattValue &value)
	{
		auto iter = characteristics.begin();

		for (; iter != characteristics.end(); iter++)
		{
			if (iter->getUuid() == characteristic)
				break;
		}

		if (iter == characteristics.end())
			return false;

		iter->setValue(value);
		return true;
	}
	/**
	 * @brief Update the value of a descriptor
	 *
	 * @param characteristic UUID of the characteristic the descriptor belongs to
	 * @param descriptor UUID of the descriptor the value should be changed for
	 * @param handle Handle of the descriptor
	 * @return True if descriptor was updated successfully, false otherwise.
	 */
	bool updateDescriptorHandle(const BluetoothGattCharacteristic &characteristic, const BluetoothGattDescriptor &descriptor, const uint16_t &handle)
	{
		auto iter = characteristics.begin();

		for (; iter != characteristics.end(); iter++)
		{
			if (iter->getUuid() == characteristic.getUuid() &&
				iter->isPermissionSet(characteristic.getPermissions()) &&
				iter->isPropertySet(characteristic.getProperties()))
				break;
		}

		if (iter == characteristics.end())
			return false;

		return iter->updateDescriptorHandle(descriptor, handle);
	}
	/**
	 * @brief Update the value of a descriptor
	 *
	 * @param characteristic UUID of the characteristic the descriptor belongs to
	 * @param descriptor UUID of the descriptor the value should be changed for
	 * @param value New value to set for the descriptor
	 * @return True if descriptor was updated successfully, false otherwise.
	 */
	bool updateDescriptorValue(const BluetoothUuid &characteristic, const BluetoothUuid &descriptor, const BluetoothGattValue &value)
	{
		auto iter = characteristics.begin();

		for (; iter != characteristics.end(); iter++)
		{
			if (iter->getUuid() == characteristic)
				break;
		}

		if (iter == characteristics.end())
			return false;

		return iter->updateDescriptorValue(descriptor, value);
	}

	/**
	 * @brief Replace stored list of characteristics with a new set.
	 *
	 * @param characteristics Set of new characteristics to store.
	 */
	void setCharacteristics(const BluetoothGattCharacteristicList &characteristics) { this->characteristics = characteristics; }

	/**
	 * @brief Get the list of characteristics which are part of the service
	 *
	 * @return List of characteristics of the service
	 */
	BluetoothGattCharacteristicList getCharacteristics() const { return characteristics; }

	/**
	 * @brief Get a specific characteristic from the service.
	 *
	 * @param uuid UUID of the characteristic
	 * @return Found characteristic or invalid one when not found.
	 */
	BluetoothGattCharacteristic getCharacteristic(const BluetoothUuid &uuid) const
	{
		for (auto &characteristic : characteristics)
		{
			if (characteristic.getUuid() == uuid)
				return characteristic;
		}

		return BluetoothGattCharacteristic();
	}

private:
	BluetoothUuid uuid;
	Type type;
	std::vector<BluetoothUuid> includes;
	std::vector<BluetoothGattCharacteristic> characteristics;
};

/**
 * @brief This interface is the base to implement an observer for the Bluetooth
 *        GATT profile to get notifications from the profile when something has changed.
 */
class BluetoothGattProfileStatusObserver
{
public:
	/**
	 * @brief This method is called when a new service is found.
	 *
	 * @param address Address of the device the serivce is found on
	 * @param service Service object which describes the found service.
	 */
	virtual void serviceFound(const std::string &address, const BluetoothGattService &service) { }

	/**
	 * @brief This method is called when service is lost.
	 *
	 * @param address Address of the device the service is lost for.
	 * @param service Service object which describes the lost service.
	 */
	virtual void serviceLost(const std::string &address, const BluetoothGattService &service) { }

	/**
	 * @brief This method is called when the value of a specific characteristic on a remote
	 *        device has changed.
	 *
	 *        The user has to register for monitoring value changes of this characteristic first
	 *        through calling BluetoothGattProfile::setCharacteristicWatchStatus.
	 *
	 * @param address Address of the device
	 * @param service UUID of the service the characteristic has changed for
	 * @param characteristic Characteristic which has changed
	 */
	virtual void characteristicValueChanged(const std::string &address, const BluetoothUuid &service, const BluetoothGattCharacteristic &characteristic) { }

	/**
	 * @brief This method is called when the value of a specific characteristic of the
	 *        local adapter has changed.
	 *
	 *        This method is only used to indicate changes to locally registered services
	 *        and their characteristics.
	 *
	 * @param service UUID of the service the characteristic has changed for
	 * @param characteristic Characteristic which has changed
	 */
	virtual void characteristicValueChanged(const BluetoothUuid &service, const BluetoothGattCharacteristic &characteristic) { }

	/**
	 * @brief This method is called when the value of a specific descriptor of the
	 *        local adapter has changed.
	 *
	 *        This method is only used to indicate changes to locally registered characteristic
	 *        and their descriptor change
	 *
	 * @param service UUID
	 * @param characteristic UUID
	 * @param descriptor which is changed
	 */
	virtual void descriptorValueChanged(const BluetoothUuid &service, const BluetoothUuid &characteristic, BluetoothGattDescriptor &descriptor) { }

	/**
	 * The method is called when an eSCO connection state is changed. That is a BLE security link is established.
	 *
	 * @param address Address of the device.
	 * @param state True if a BLE security link is established from the remote device, false if the link goes down.
	 */
	virtual void incomingLeConnectionRequest(const std::string &address, bool state) { }
	/**
	 * The method is called when an client try to read a characteristic.
	 *
	 * @param requestId Id
	 * @param address Address of the device.
	 * @param serviceId Service handle Id
	 * @param charId Characteristic id
	 */
	virtual void characteristicValueReadRequested(uint32_t requestId, const std::string &address, uint16_t serviceId, uint16_t charId) { }
	virtual void characteristicValueWriteRequested(uint32_t requestId, const std::string &address, uint16_t serviceId, uint16_t charId,
				const BluetoothGattValue &value, bool response = true) { }
};

/**
 * @brief Interface to abstract operations for the GATT bluetooth profile.
 */
class BluetoothGattProfile
{
public:
	/**
	 * @brief Default C'tor
	 */
	BluetoothGattProfile() : gattObserver(0) { }

	virtual ~BluetoothGattProfile() { }

	/**
	 * @brief Register a observer for the profile. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 *        A subsequent call will override the currently registered observer.
	 *
	 * @param observer Observer instance to register.
	 */
	void registerObserver(BluetoothGattProfileStatusObserver *observer) { this->gattObserver = observer; }

	/**
	 * @brief Discover available services
	 *
	 *        This will discover available services from known devices.
	 *
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void discoverServices(BluetoothResultCallback callback) = 0;

	/**
	 * @brief Discover available services for a specific remote device.
	 *
	 * @param address Address of the remote device
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void discoverServices(const std::string &address, BluetoothResultCallback callback) = 0;

	/**
	 * @brief add client/server application
	 *
	 * @param appUuid application Uuid
	 * @param type type of application
	 */
	virtual uint16_t addApplication(const BluetoothUuid &appUuid, ApplicationType type) { return addApplication(appUuid); }

	/**
	 * @brief remove application for services.
	 *
	 * @param appId Application handle Id
	 * @param type type of application
	 */
	virtual bool removeApplication(uint16_t appId, ApplicationType type) { return false; }

	/**
	 * @brief add application for services.
	 *
	 * @param appUuid application Uuid
	 */
	virtual uint16_t addApplication(const BluetoothUuid &appUuid) { return -1; }

	/**
	 * @brief remove application for services.
	 *
	 * @param appId Application handle Id
	 */
	virtual bool removeApplication(uint16_t appId) { return false; }

	/**
	* @brief Add a new service to the stack
	*
	*        The new service will become visible to other remote devices after it was
	*        successfully added.
	*
	* @param appId Application handle Id
	* @param service Service to add to the database
	* @param callback Callback function which is called when the operation is done or
	*		has failed.
	*/
	virtual void addService(uint16_t appId, const BluetoothGattService &service, BluetoothGattAddCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, 0);
	}

	/**
	* @brief Remove service from the stack
	*
	* @param appId Application handle Id
	* @param service Service to remove from the database
	* @param callback Callback function which is called when the operation is done or
	*		has failed.
	*/
	virtual void removeService(uint16_t appId, const BluetoothGattService &service, BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	* @brief Remove service from the stack
	*
	* @param appId Application handle Id
	* @param serviceId Handle id of Service to remove from the database
	* @param callback Callback function which is called when the operation is done or
	*		has failed.
	*/
	virtual void removeService(uint16_t appId, uint16_t serviceId, BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/*
	*
	* @brief add characteristic to the stack.
	*
	* @param serviceId Service handle Id
	* @param characteristic new characteristic to store.
	* @param callback Callback function which is called when the operation is done or
	*		has failed.
	*/
	virtual void addCharacteristic(uint16_t appId, uint16_t serviceId, const BluetoothGattCharacteristic &characteristic, BluetoothGattAddCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, 0);
	}

	/*
	*
	* @brief add descriptor to the stack.
	*
	* @param appId Application handle Id
	* @param serviceId Service handle Id
	* @param descriptor new descriptor to store.
	* @param callback Callback function which is called when the operation is done or
	*		has failed.
	*/
	virtual void addDescriptor(uint16_t appId, uint16_t serviceId, const BluetoothGattDescriptor &descriptor, BluetoothGattAddCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, 0);
	}

	/*
	*
	* @brief add characteristic to the stack.
	*
	* @param appId Application handle Id
	* @param serviceId Service handle Id
	* @param characteristic new characteristic to store.
	* @param callback Callback function which is called when the operation is done or
	*		has failed.
	*/
	virtual void addCharacteristic(uint16_t serviceId, const BluetoothGattCharacteristic &characteristic, BluetoothGattAddCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, 0);
	}
	/*
	*
	* @brief add descriptor to the stack.
	*
	* @param serviceId Service handle Id
	* @param descriptor new descriptor to store.
	* @param callback Callback function which is called when the operation is done or
	*		has failed. */
	virtual void addDescriptor(uint16_t serviceId, const BluetoothGattDescriptor &descriptor, BluetoothGattAddCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, 0);
	}

	/*
	*
	* @brief start service
	*
	* @param appId Application handle Id
	* @param serviceId Service handle Id
	* @param mode transport mode
	* @param callback Callback function which is called when the operation is done or
	*		has failed.
	*/
	virtual void startService(uint16_t appId, uint16_t serviceId, BluetoothGattTransportMode mode, BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/*
	*
	* @brief start service
	*
	* @param serviceId Service handle Id
	* @param mode transport mode
	* @param callback Callback function which is called when the operation is done or
	*		has failed.
	*/
	virtual void startService(uint16_t serviceId, BluetoothGattTransportMode mode, BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Enable or disable watching a specific characteristic.
	 *
	 *        Changes will be reported through the characteristicValueChanged method of the
	 *        BluetoothGattProfileStatusObserver observer instance registered with the
	 *        profile.
	 *
	 *        If no observer is registered enabling the watch for a characteristic will fail.
	 *
	 * @param address Address of remote device
	 * @param appId Application handle Id
	 * @param handle handle of characteristic to enable/disable the watch for
	 * @param enabled True to enable watching, false to disable.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void changeCharacteristicWatchStatus(const std::string &address, const uint16_t &appId, const uint16_t &handle, bool enabled,
												 BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Read a characteristic for a remote device.
	 *
	 * @param connId ID of remote device
	 * @param characteristicHandle Handle of characteristic to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readCharacteristic(const uint16_t &connId, const uint16_t &characteristicHandle,
									 BluetoothGattReadCharacteristicCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattCharacteristic());
	}

	/**
	 * @brief Read a list of characteristics for a remote device.
	 *
	 * @param connId ID of remote device
	 * @param characteristicHandles List of handles for characteristics to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readCharacteristics(const uint16_t &connId, const std::vector<uint16_t> &characteristicHandles,
									 BluetoothGattReadCharacteristicsCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattCharacteristicList());
	}

	/**
	 * @brief Write a single characteristic for a remote device.
	 *
	 *        The supplied characteristic needs to be initialized with all needed information otherwise
	 *        a call will result in an error.
	 *
	 * @param connId ID of remote device
	 * @param characteristic Characteristic to write
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void writeCharacteristic(const uint16_t &connId, const BluetoothGattCharacteristic &characteristic,
									 BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Read a descriptor from a remote device.
	 *
	 * @param connId ID of remote device
	 * @param descriptor Handle of descriptors to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readDescriptor(const uint16_t &connId, const uint16_t &descriptorHandle,
								 BluetoothGattReadDescriptorCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattDescriptor());
	}

	/**
	 * @brief Read a list of descriptors from a remote device.
	 *
	 * @param connId ID of remote device
	 * @param descriptors List of handles for descriptors to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readDescriptors(const uint16_t &connId, const std::vector<uint16_t> &descriptorHandles,
								 BluetoothGattReadDescriptorsCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattDescriptorList());
	}
	/**
	 * @brief Write a single descriptor served by a remote device.
	 *
	 *        The supplied descriptor needs to be initialized with all needed information otherwise
	 *        a call will result in an error.
	 *
	 * @param connId ID of remote device
	 * @param descriptor Descriptor to write
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void writeDescriptor(const uint16_t &connId, const BluetoothGattDescriptor &descriptor,
								 BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Enable or disable watching a specific characteristic.
	 *
	 *        Changes will be reported through the characteristicValueChanged method of the
	 *        BluetoothGattProfileStatusObserver observer instance registered with the
	 *        profile.
	 *
	 *        If no observer is registered enabling the watch for a characteristic will fail.
	 *
	 * @param address Address of remote device
	 * @param appId ID of remote device
	 * @param service Service the characteristic belongs to
	 * @param characteristic Characteristic to enable/disable the watch for
	 * @param enabled True to enable watching, false to disable.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void changeCharacteristicWatchStatus(const std::string &address, const uint16_t &appId, const BluetoothUuid &service,
												 const BluetoothUuid &characteristic, bool enabled,
												 BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Read a characteristic for a remote device.
	 *
	 * @param connId ID of remote device
	 * @param service Service the characteristics belong to
	 * @param characteristic UUID for characteristics to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readCharacteristic(const uint16_t &connId, const BluetoothUuid& service,
									 const BluetoothUuid &characteristics,
									 BluetoothGattReadCharacteristicCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattCharacteristic());
	}

	/**
	 * @brief Read a list of characteristics for a remote device.
	 *
	 * @param connId ID of remote device
	 * @param service Service the characteristics belong to
	 * @param characteristics List of UUIDs for characteristics to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readCharacteristics(const uint16_t &connId, const BluetoothUuid& service,
									 const BluetoothUuidList &characteristics,
									 BluetoothGattReadCharacteristicsCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattCharacteristicList());
	}

	/**
	 * @brief Write a single characteristic for a remote device.
	 *
	 *        The supplied characteristic needs to be initialized with all needed information otherwise
	 *        a call will result in an error.
	 *
	 * @param connId ID of remote device
	 * @param service Service the characteristic belongs to
	 * @param characteristic Characteristic to write
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void writeCharacteristic(const uint16_t &connId, const BluetoothUuid& service,
									 const BluetoothGattCharacteristic &characteristic,
									 BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Read a list of descriptors from a remote device.
	 *
	 * @param connId ID of remote device
	 * @param service Service the characteristics belong to
	 * @param characteristic Characteristic the descriptors belong to
	 * @param descriptor UUID for descriptor to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readDescriptor(const uint16_t &connId, const BluetoothUuid& service, const BluetoothUuid &characteristic,
								 const BluetoothUuid &descriptor, BluetoothGattReadDescriptorCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattDescriptor());
	}
	/**
	 * @brief Read a list of descriptors from a remote device.
	 *
	 * @param connId ID of remote device
	 * @param service Service the characteristics belong to
	 * @param characteristic Characteristic the descriptors belong to
	 * @param descriptors List of UUIDs for descriptors to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readDescriptors(const uint16_t &connId, const BluetoothUuid& service, const BluetoothUuid &characteristic,
								 const BluetoothUuidList &descriptors, BluetoothGattReadDescriptorsCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattDescriptorList());
	}
	/**
	 * @brief Write a single descriptor served by a remote device.
	 *
	 *        The supplied descriptor needs to be initialized with all needed information otherwise
	 *        a call will result in an error.
	 *
	 * @param connId ID of remote device
	 * @param service Service the characteristic belongs to
	 * @param characteristic Characteristic the descriptor belongs to.
	 * @param descriptor Descriptor to write
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void writeDescriptor(const uint16_t &connId, const BluetoothUuid &service, const BluetoothUuid &characteristic,
								 const BluetoothGattDescriptor &descriptor, BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Enable or disable watching a specific characteristic.
	 *
	 *        Changes will be reported through the characteristicValueChanged method of the
	 *        BluetoothGattProfileStatusObserver observer instance registered with the
	 *        profile.
	 *
	 *        If no observer is registered enabling the watch for a characteristic will fail.
	 *
	 * @param address Address of remote device
	 * @param handle handle of characteristic to enable/disable the watch for
	 * @param enabled True to enable watching, false to disable.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void changeCharacteristicWatchStatus(const std::string &address, const uint16_t handle, bool enabled,
	                                             BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Read a characteristic for a remote device.
	 *
	 * @param address Address of remote device
	 * @param characteristicHandle handle of characteristic to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readCharacteristic(const std::string &address, const uint16_t &characteristicHandle,
									 BluetoothGattReadCharacteristicCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattCharacteristic());
	}

	/**
	 * @brief Read a list of characteristics for a remote device.
	 *
	 * @param address Address of remote device
	 * @param characteristicHandles List of handles for characteristics to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readCharacteristics(const std::string &address, const std::vector<uint16_t> &characteristicHandles,
									 BluetoothGattReadCharacteristicsCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattCharacteristicList());
	}

	/**
	 * @brief Write a single characteristic for a remote device.
	 *
	 *        The supplied characteristic needs to be initialized with all needed information otherwise
	 *        a call will result in an error.
	 *
	 * @param address Address of remote device
	 * @param characteristic Characteristic to write
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void writeCharacteristic(const std::string &address, const BluetoothGattCharacteristic &characteristic,
									 BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Read a descriptor from a remote device.
	 *
	 * @param address Address of remote device
	 * @param descriptor Handles of descriptor to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readDescriptor(const std::string &address, const uint16_t &descriptorHandle,
								 BluetoothGattReadDescriptorCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattDescriptor());
	}

	/**
	 * @brief Read a list of descriptors from a remote device.
	 *
	 * @param address Address of remote device
	 * @param descriptors List of handles for descriptors to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readDescriptors(const std::string &address, const std::vector<uint16_t> &descriptorHandles,
								 BluetoothGattReadDescriptorsCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattDescriptorList());
	}
	/**
	 * @brief Write a single descriptor served by a remote device.
	 *
	 *        The supplied descriptor needs to be initialized with all needed information otherwise
	 *        a call will result in an error.
	 *
	 * @param address Address of remote device
	 * @param descriptor Descriptor to write
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void writeDescriptor(const std::string &address, const BluetoothGattDescriptor &descriptor,
								 BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Enable or disable watching a specific characteristic.
	 *
	 *        Changes will be reported through the characteristicValueChanged method of the
	 *        BluetoothGattProfileStatusObserver observer instance registered with the
	 *        profile.
	 *
	 *        If no observer is registered enabling the watch for a characteristic will fail.
	 *
	 * @param address Address of remote device
	 * @param service Service the characteristic belongs to
	 * @param characteristic Characteristic to enable/disable the watch for
	 * @param enabled True to enable watching, false to disable.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void changeCharacteristicWatchStatus(const std::string &address, const BluetoothUuid &service,
												 const BluetoothUuid &characteristic, bool enabled,
												 BluetoothResultCallback callback) = 0;

	/**
	 * @brief Read a characteristic for a remote device.
	 *
	 * @param address Address of remote device
	 * @param service Service the characteristics belong to
	 * @param characteristic UUID for characteristic to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readCharacteristic(const std::string &address, const BluetoothUuid& service,
									 const BluetoothUuid &characteristic,
									 BluetoothGattReadCharacteristicCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattCharacteristic());
	}

	/**
	 * @brief Read a list of characteristics for a remote device.
	 *
	 * @param address Address of remote device
	 * @param service Service the characteristics belong to
	 * @param characteristics List of UUIDs for characteristics to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readCharacteristics(const std::string &address, const BluetoothUuid& service,
	                                 const BluetoothUuidList &characteristics,
	                                 BluetoothGattReadCharacteristicsCallback callback) = 0;

	/**
	 * @brief Write a single characteristic for a remote device.
	 *
	 *        The supplied characteristic needs to be initialized with all needed information otherwise
	 *        a call will result in an error.
	 *
	 * @param address Address of remote device
	 * @param service Service the characteristic belongs to
	 * @param characteristic Characteristic to write
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void writeCharacteristic(const std::string &address, const BluetoothUuid& service,
	                                 const BluetoothGattCharacteristic &characteristic,
	                                 BluetoothResultCallback callback) = 0;

	/**
	 * @brief Read a descriptor from a remote device.
	 *
	 * @param address Address of remote device
	 * @param service Service the characteristics belong to
	 * @param characteristic Characteristic the descriptors belong to
	 * @param descriptor UUID for descriptors to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readDescriptor(const std::string &address, const BluetoothUuid& service, const BluetoothUuid &characteristic,
								 const BluetoothUuid &descriptor, BluetoothGattReadDescriptorCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothGattDescriptor());
	}
	/**
	 * @brief Read a list of descriptors from a remote device.
	 *
	 * @param address Address of remote device
	 * @param service Service the characteristics belong to
	 * @param characteristic Characteristic the descriptors belong to
	 * @param descriptors List of UUIDs for descriptors to read
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void readDescriptors(const std::string &address, const BluetoothUuid& service, const BluetoothUuid &characteristic,
	                             const BluetoothUuidList &descriptors, BluetoothGattReadDescriptorsCallback callback) = 0;
	/**
	 * @brief Write a single descriptor served by a remote device.
	 *
	 *        The supplied descriptor needs to be initialized with all needed information otherwise
	 *        a call will result in an error.
	 *
	 * @param address Address of remote device
	 * @param service Service the characteristic belongs to
	 * @param characteristic Characteristic the descriptor belongs to.
	 * @param descriptor Descriptor to write
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void writeDescriptor(const std::string &address, const BluetoothUuid &service, const BluetoothUuid &characteristic,
	                             const BluetoothGattDescriptor &descriptor, BluetoothResultCallback callback) = 0;

	/**
	 * @brief Retrieve a specific services for a remote device.
	 *
	 *        A call will not start a service discovery for the remote device. If service wasn't yet discoverd
	 *        the service will not be found and an invalid service is returned.
	 *
	 *        The service is identified by it's UUID.
	 *
	 *        If the service couldn't be found the returned service object is marked as invalid.
	 *
	 * @param address Address of the remote device to return the service for.
	 * @param uuid UUID of the service to retrieve.
	 * @return Found service object for specified UUID or an invalid service object.
	 */
	virtual BluetoothGattService getService(const std::string &address, const BluetoothUuid &uuid) = 0;

	/**
	 * @brief Retrieve a list of available services for specified remote device.
	 *
	 *        A call will not start a service discovery for the remote device. If service wasn't yet discoverd
	 *        the service will not be found and an invalid service is returned.
	 *
	 * @param address Address of the remote device to return the service list for.
	 * @return List of services for the remote device or an empty list.
	 */
	virtual BluetoothGattServiceList getServices(const std::string &address) = 0;

	// Check avrcp usage

	/*
	*
	* @brief characteristic value read response to stack
	*
	* @param id request Id
	* @param value read value
	*/
	virtual void characteristicValueReadResponse(uint32_t requestId, BluetoothError error, const BluetoothGattValue &value) {}
	/*
	*
	* @brief characteristic value write response to stack
	*
	* @param id request Id
	* @param value write value
	*/
	virtual void characteristicValueWriteResponse(uint32_t requestId, BluetoothError error, const BluetoothGattValue &value) {}
	/*
	*
	* @brief characteristic value write response to stack
	*
	* @param characteristic changed characteristic value
	*/
	virtual void notifyCharacteristicValueChanged(BluetoothGattCharacteristic characteristic)
	{
		return;
	}
	/*
	*
	* @brief characteristic value write response to stack
	*
	* @param serviceId Service handle Id
	* @param characteristic changed characteristic value
	* @param charId changed characteristic handle id
	*/
	virtual void notifyCharacteristicValueChanged(uint32_t serviceId, BluetoothGattCharacteristic characteristic, uint16_t charId) { }

	/*
	*
	* @brief characteristic value write response to stack
	* @param serverId or appId
	* @param serviceId Service handle Id
	* @param characteristic changed characteristic value
	* @param charId changed characteristic handle id
	*/
	virtual void notifyCharacteristicValueChanged(uint16_t serverId, uint16_t serviceId, BluetoothGattCharacteristic characteristic, uint16_t charId) { }

	/*
	*
	* @brief characteristic value write response to stack
	* @param serverId or appId
	* @param serviceId Service handle Id
	* @param descId descriptor handle Id
	* @param descriptor changed descriptor value
	* @param charId changed characteristic handle id
	*/
	virtual void notifyDescriptorValueChanged(uint16_t serverId, uint16_t serviceId, uint16_t descId, BluetoothGattDescriptor descriptor, uint16_t charId) { }

	/**
	* @brief Get connectId of the specific remote device.
	*
	* @param address Address of the device to connect to.
	*/
	virtual uint16_t getConnectId(const std::string &address)
	{
		return 0;
	}

	/**
	* @brief Get appId of the specific remote device.
	*
	* @param address Address of the device to connect to.
	*/
	virtual uint16_t getAppId(const std::string &address)
	{
		return 0;
	}

	/**
	 * @brief Connect with a profile on a specific remote device.
	 *
	 * @param appId ID of GATT client application
	 * @param autoConnection True to directly connect to the remote device (false) or
	 *        to automatically connect as soon as the remote device becomes available
	 * @param address Address of the device to connect to.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void connectGatt(const uint16_t &appId, bool autoConnection, const std::string &address, BluetoothConnectCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, 0);
	}

	/**
	 * @brief Disconnect a remote device from the profile if connected.
	 *
	 *        If not connected the callback will return immediately and the operation
	 *        marked as succeeded.
	 *
	 * @param appId ID of GATT client application
	 * @param connectId ID of remote device
	 * @param address Address of the device to disconnect
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void disconnectGatt(const uint16_t &appId, const uint16_t &connectId, const std::string &address, BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

protected:
	/**
	 * @brief Retrieve the GATT status observer
	 *
	 *        If no observer was registered with registerObserver before a NULL object
	 *        instance will be returned which is usable but will doing nothing when
	 *        called.
	 *
	 * @return The GATT status observer
	 */
	BluetoothGattProfileStatusObserver* getGattObserver()
	{
		static BluetoothGattProfileStatusObserver nullObserver;

		if (!gattObserver)
			return &nullObserver;

		return gattObserver;
	}

private:
	BluetoothGattProfileStatusObserver *gattObserver;
};

#endif // BLUETOOTH_SIL_GATT_H_
