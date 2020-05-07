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

#ifndef BLUETOOTH_SIL_PROPS_H_
#define BLUETOOTH_SIL_PROPS_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

/**
 * @brief The BluetoothProperty class abstracts access to several properties
 *        of the Bluetooth adapter or found remote devices.
 */
class BluetoothProperty
{
public:
	enum Type
	{
		/**
		 * @brief Property does not contain any information.
		 **/
		EMPTY,
		/**
		 * @brief Friendly name of the Bluetooth adapter/device.
		 *
		 *        Type: std::string
		 *        Access: Device (read), Adapter(read)
		 **/
		NAME,
		/**
		 * @brief Alias name of the Bluetooth adapter/device.
		 *
		 *        Type: std::string
		 *        Access: Device (read), Adapter(read/write)
		 **/
		ALIAS,
		/**
		 * @brief Bluetooth address of the Bluetooth adapter/device.
		 *
		 *        Type: std::string
		 *        Access: Device (read), Adapter (read)
		 **/
		BDADDR,
		/**
		 * @brief Name of the Bluetooth stack
		 *
		 *        Type: std::string
		 *        Access: Adapter (read)
		 **/
		STACK_NAME,
		/**
		 * @brief Version of the Bluetooth stack
		 *
		 *        Type: std::string
		 *        Access: Adapter (read)
		 **/
		STACK_VERSION,
		/**
		 * @brief Version of the firmware used by the Bluetooth H/W module
		 *
		 *        Type: std::string
		 *        Access: Adapter (read/write)
		 **/
		FIRMWARE_VERSION,
		/**
		 * @brief UUIDs which represents the available services of a device.
		 *
		 *        Type: std::vector<std::string>
		 *        Access: Device (read), Adapter (read)
		 **/
		UUIDS,
		/**
		 * @brief Bluetooth class of device of the adapter/device.
		 *
		 *        Type: std::uint32_t
		 *        Access: Device (read), Adapter (read)
		 **/
		CLASS_OF_DEVICE,
		/**
		 * @brief Type of the device.
		 *
		 *        Type: std::uint32_t (matches enum BluetoothDeviceType)
		 *        Access: Device (read)
		 **/
		TYPE_OF_DEVICE,
		/**
		* @brief Timeout in seconds after which device discovery will be stopped. A
		*        value of zero means that the timeout is disabled and the adapter will
		*        run the discovery process forever.
		*
		*        The default value for this timeout should be disable (value 0).
		*
		*        Type: std::uint32_t
		*        Access: Adapter (read/write)
		**/
		DISCOVERY_TIMEOUT,
		/**
		* @brief Indicates the discoverable state of the adapter. If the discoverable
		*        timeout is set to a non-zero value the SIL will set the value back to
		*        false when the timer expires.
		*
		*        The value of this property defaults to false.
		*
		*        Type: bool
		*        Access: Adapter (read/write)
		**/
		DISCOVERABLE,
		/**
		* @brief The discoverable timeout in seconds. A value of zero means that
		*        the timeout is disabled and it will stay in discoverable mode
		*        forever.
		*
		*        The default value for this timeout should be disable (value 0).
		*
		*        Type: uint32_t
		*        Access: Adapter (read/write)
		**/
		DISCOVERABLE_TIMEOUT,
		/**
		* @brief Indicates if the adapter is pairable or not. This only applies
		*        for incoming pairing requests.
		*
		*        Type: bool
		*        Access: Adapter (read/write)
		**/
		PAIRABLE,
		/**
		* @brief The pairable timeout in seconds. A value of zero means that the
		*        timeout is disabled and the adapter will stay in pairable mode
		*        forever.
		*
		*        The default value for this timeout should be disabled (value 0).
		*
		*        Type: uint32_t
		*        Access: Adapter (read/write)
		**/
		PAIRABLE_TIMEOUT,
		/**
		 * @brief Pairing status of a device. If true the device is successfully
		 *        paired and if false it is not paired.
		 *
		 *        The default value is false.
		 *
		 *        Type: bool
		 *        Access: Device (read)
		 */
		PAIRED,
		/**
		 * @brief Indicates if the remote device or profile is currently connected.
		 *
		 *        The default value is false.
		 *
		 *        Type: bool
		 *        Access: Device (read), Profile (read)
		 */
		CONNECTED,
		/**
		 * @brief Indicates if the remote device is a trusted device.
		 *
		 *        The default value is false.
		 *
		 *        Type: bool
		 *        Access: Device (read/write)
		 */
		TRUSTED,
		/**
		 * @brief Indicates if the remote device is blocked for pairing and profile connections.
		 *
		 *        The default value is false.
		 *
		 *        Type: bool
		 *        Access: Device (read/write)
		 */
		BLOCKED,
		/**
		 * @brief received signal strength indicator (RSSI) indicates the strength of the Bluetooth signal from a classic or ble device
		 *
		 *        Type: int - in Decibel-milliwatts (dbm) units
		 *        Access: Device (read)
		 */
		RSSI,
		/**
		 * @brief Indicates the power with which a Bluetooth LE device broadcasts its signal
		 *
		 *        Type: int - in Decibel-milliwatts (dbm) units
		 *        Access: Device (read)
		 */
		TXPOWER,
		/**
		 * @brief Indicates the role which a Bluetooth device connects on
		 *
		 *        Type: std::uint32_t (bitwise OR for enum BluetoothDeviceRole)
		 *        Access: Device (read)
		 */
		ROLE,
		/**
		 * @brief Indicates the manufacturer data contained in the discoverable Bluetooth device.
		 *
		 *        Type: std::uint8_t[] - byte array of manufacturer data.
		 *        Access: Device (read)
		 */
		MANUFACTURER_DATA,
		/**
		 * @brief Indicates the Inquiry access code for the discoverable Bluetooth device.
		 *
		 *        Type: std::uint32_t (bitwise OR for enum InquiryAccessCode)
		 *        Access: Device (read)
		 */
		INQUIRY_ACCESS_CODE,
		/**
		 * @brief Indicates the content of the advertisement record offered by
		 *        the remote device.
		 *
		 *        Type: std::uint8_t[] - byte array of scan record.
		 *        Access: Device (read)
		 */
		SCAN_RECORD,
	};

	/**
	 * @brief Initialize an empty property
	 *
	 *        The type of the property will be set to EMPTY.
	 */
	BluetoothProperty() :
		type(EMPTY),
		impl(0)
	{
	}

	/**
	 * @brief Initialize the property with a type
	 * @param type Type of the property
	 */
	BluetoothProperty(Type type) :
		type(type),
		impl(0)
	{
	}

	/**
	 * @brief Copy constructor
	 * @param other Other property to create a copy from
	 */
	BluetoothProperty(const BluetoothProperty &other) :
		type(other.type),
		impl(other.impl),
		typeName(other.typeName)
	{
	}

	/**
	 * @brief Initialize the property with a type and a value
	 * @param type Type of the property
	 * @param value Value of the property
	 */
	template<class T>
	BluetoothProperty(Type type, T value) :
		type(type),
		impl(new PropertyImpl<T>(value)),
		typeName(typeid(T).name()) { }

	/**
	 * @brief Get the type of the property
	 * @return Type of the property
	 */
	Type getType() const
	{
		return type;
	}

	/**
	 * @brief Get the value of the property
	 * @return Value of the property
	 */
	template<class T>
	T getValue() const
	{
		std::string requestedType = typeid(T).name();

		if (requestedType != typeName)
		{
			throw std::logic_error("Non-matching types");
		}

		return dynamic_cast<PropertyImpl<T>*>(impl.get())->getValue();
	}

	/**
	 * @brief Set the value of the property
	 * @param value New value of the property
	 */
	template<class T>
	void setValue(T value)
	{
		impl.reset(new PropertyImpl<T>(value));
		typeName = typeid(T).name();
	}

private:
	struct BasePropertyImpl
	{
		virtual ~BasePropertyImpl() { }
	};

	template<class T>
	struct PropertyImpl : public BasePropertyImpl
	{
		PropertyImpl(T value) : value(value) { }
		~PropertyImpl() { }

		T getValue() const
		{
			return value;
		}

		T value;
	};

private:
	Type type;
	std::shared_ptr<BasePropertyImpl> impl;
	std::string typeName;
};

typedef std::vector<BluetoothProperty> BluetoothPropertiesList;

/**
 * @brief Callback to return a list of properties asynchronously.
 */
typedef std::function<void(BluetoothError, const BluetoothPropertiesList &)>
BluetoothPropertiesResultCallback;

/**
 * @brief Callback to return a single property asynchronously.
 */
typedef std::function<void(BluetoothError, const BluetoothProperty &prop)>
BluetoothPropertyResultCallback;

/**
 * @brief Type of a Bluetooth device
 */
enum BluetoothDeviceType : std::uint32_t
{
	/** Unknown device type */
	BLUETOOTH_DEVICE_TYPE_UNKNOWN,
	/** Basic Rate/Enhanced Data Rate */
	BLUETOOTH_DEVICE_TYPE_BREDR,
	/** Bluetooth Low Energy */
	BLUETOOTH_DEVICE_TYPE_BLE,
	/** Device supports both Basic Rate/Enhanced Data Rate and Bluetooth Low Energy */
	BLUETOOTH_DEVICE_TYPE_DUAL
};

/**
 * @brief Role of a Bluetooth device in connecting
 */
enum BluetoothDeviceRole : std::uint32_t
{
	/** Don't care */
	BLUETOOTH_DEVICE_ROLE = 0x0,
	/** HF role for HFP */
	BLUETOOTH_DEVICE_ROLE_HFP_HF = 0x1,
	/** AG role for HFP */
	BLUETOOTH_DEVICE_ROLE_HFP_AG = 0x2,
	/** Source role for A2DP */
	BLUETOOTH_DEVICE_ROLE_A2DP_SRC = 0x4,
	/** Sink role for A2DP */
	BLUETOOTH_DEVICE_ROLE_A2DP_SINK = 0x8,
	/** RemoteControl role for AVRCP */
	BLUETOOTH_DEVICE_ROLE_AVRCP_RMT = 0x10,
	/** RemoteControlTarget role for AVRCP */
	BLUETOOTH_DEVICE_ROLE_AVRCP_TGT = 0x20,
	/** PANU role for PAN */
	BLUETOOTH_DEVICE_ROLE_PAN_PANU = 0x40,
	/** NAP role for PAN */
	BLUETOOTH_DEVICE_ROLE_PAN_NAP = 0x80,
	/** GN role for PAN */
	BLUETOOTH_DEVICE_ROLE_PAN_GN = 0x100,
	/** Source role for HDP */
	BLUETOOTH_DEVICE_ROLE_HDP_SRC = 0x200,
	/** Sink role for HDP */
	BLUETOOTH_DEVICE_ROLE_HDP_SINK = 0x400,
	/** Host role for HID */
	BLUETOOTH_DEVICE_ROLE_HID_HOST = 0x800,
	/** Gatt client */
	BLUETOOTH_DEVICE_ROLE_GATT_CLIENT = 0x1000,
	/** Gatt server */
	BLUETOOTH_DEVICE_ROLE_GATT_SERVER = 0x2000,
};

#endif
