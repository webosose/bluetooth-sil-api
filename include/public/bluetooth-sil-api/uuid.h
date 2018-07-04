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

#ifndef BLUETOOTH_SIL_UUID_H_
#define BLUETOOTH_SIL_UUID_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

#include <stdint.h>
#include <cstdlib>
#include <memory.h>
#include <arpa/inet.h>

#include <string>
#include <vector>
#include <algorithm>

#define BLUETOOTH_UUID_128_LENGTH	36
#define BLUETOOTH_UUID_32_LENGTH	8
#define BLUETOOTH_UUID_16_LENGTH	4

struct uint128_t
{
    uint8_t data[16];
};

/**
 * @brief Abstraction of a Bluetooth UUID
 *
 *        This class is able to deal with 16, 32 or 128 bit long UUIDs. The origin
 *        of the UUID is always a string which is then parsed and handled accordingly.
 *
 *        Internally only the string is stored and when needed parsed and converted
 *        into the needed format.
 */
class BluetoothUuid
{
public:
	/**
	 * @brief Type of the UUID
	 */
	enum Type
	{
		UNKNOWN,
		UUID16,
		UUID32,
		UUID128
	};

	/**
	 * @brief Default c'tor
	 */
	BluetoothUuid() : type(UNKNOWN) { }

	/**
	 * @brief Create UUID from a string and determine its type automatically
	 * @param uuid UUID to be set
	 */
	BluetoothUuid(const std::string &uuid) : uuid(uuid), type(UNKNOWN) { determineType(); }

	/**
	 * @brief Create UUID from a string and set its type manually
	 * @param uuid UUID to be set
	 * @param type Type of the UUID
	 */
	BluetoothUuid(const std::string &uuid, Type type) : uuid(uuid), type(type) { }

	/**
	 * @brief Copy c'tor
	 * @param other Other object to copy values from
	 */
	BluetoothUuid(const BluetoothUuid &other) : uuid(other.uuid), type(other.type) { }

	/**
	 * @brief Operator implementation to support UUID comparison
	 * @param rhs Right-hand side object to compare
	 * @return Result of the comparison.
	 */
	bool operator <(const BluetoothUuid& rhs) const { return uuid < rhs.uuid; }

	/**
	 * @brief Operator implementation to support UUID comparison
	 * @param rhs Right-hand side object to compare
	 * @return Result of the comparison.
	 */
	bool operator ==(const BluetoothUuid& rhs) const { return uuid == rhs.uuid; }

	/**
	 * @brief Operator implementation to support UUID comparison
	 * @param rhs Right-hand side object to compare
	 * @return Result of the comparison.
	 */
        bool operator !=(const BluetoothUuid& rhs) const { return uuid != rhs.uuid; }

	/**
	 * @brief Operator implementation to support UUID comparison
	 * @param rhs Right-hand side object to compare
	 * @return Result of the comparison.
	 */
	bool operator ==(const std::string& rhs) const { return uuid == rhs; }

	/**
	 * @brief Operator implementation to support UUID comparison
	 * @param rhs Right-hand side object to compare
	 * @return Result of the comparison.
	 */
	bool operator !=(const std::string& rhs) const { return uuid != rhs; }

	/**
	 * @brief Return UUID as string
	 * @return UUID as std::string
	 */
	std::string toString() const { return uuid; }

	/**
	 * @brief Convert UUID to a uint16 value
	 * @return UUID converted to uint16 or 0 if conversion is not possible.
	 */
	uint16_t toUInt16() const
	{
		if (!isValid())
			return 0;

		if (type == UUID128)
			return 0;

		char *endptr = nullptr;

		uint16_t value = static_cast<uint16_t>(strtoull(uuid.c_str(), &endptr, 16));
		if (!endptr || *endptr != '\0')
			return 0;

		return value;
	}

	/**
	 * @brief Convert UUID to a uint32 value
	 * @return UUID converted to uint32 or 0 if conversion is not possible.
	 */
	uint32_t toUInt32() const
	{
		if (!isValid())
			return 0;

		if (type == UUID128)
			return 0;

		char *endptr = nullptr;

		uint32_t value = static_cast<uint32_t>(strtoull(uuid.c_str(), &endptr, 16));
		if (!endptr || *endptr != '\0')
			return 0;

		return value;
	}

	/**
	 * @brief Convert UUID to a uint128 value
	 * @return UUID converted to uint128 or 0 if conversion is not possible.
	 */
	uint128_t toUInt128() const
	{
		uint128_t value;
		uint8_t *ptr = (uint8_t*) &value;

		memset(&value, 0, sizeof(uint128_t));

		if (!isValid())
			return value;

		if (type != UUID128)
		{
			// Getting a 32 bit value will always work also when we have only a 16 bit UUID
			uint32_t u32 = toUInt32();
			memcpy(&ptr[0], &u32, 4);
		}
		else
		{
			uint32_t data0, data4;
			uint16_t data1, data2, data3, data5;

			auto ret = sscanf(uuid.c_str(), "%08x-%04hx-%04hx-%04hx-%08x%04hx",
			       &data0, &data1, &data2, &data3, &data4, &data5);
			if (ret != 6)
				return value;

			data0 = htonl(data0);
			data1 = htons(data1);
			data2 = htons(data2);
			data3 = htons(data3);
			data4 = htonl(data4);
			data5 = htons(data5);

			memcpy(&ptr[0], &data0, 4);
			memcpy(&ptr[4], &data1, 2);
			memcpy(&ptr[6], &data2, 2);
			memcpy(&ptr[8], &data3, 2);
			memcpy(&ptr[10], &data4, 4);
			memcpy(&ptr[14], &data5, 2);
		}

		return value;
	}

	/**
	 * @brief Check if UUID is valid.
	 * @return True if UUID is valid. False otherwise.
	 */
	bool isValid() const { return type != UNKNOWN; }

	/**
	 * @brief Retrieve the type of the UUID
	 * @return Type of the UUID
	 */
	Type getType() const { return type; }

private:
	void determineType()
	{
		bool onlyAlNum = std::find_if(uuid.begin(), uuid.end(), [](char c) { return !isalnum(c); }) == uuid.end();

		if (onlyAlNum)
		{
			if (uuid.length() == BLUETOOTH_UUID_16_LENGTH)
				type = UUID16;
			else if (uuid.length() == BLUETOOTH_UUID_32_LENGTH)
				type = UUID32;
		}
		else if (!onlyAlNum)
		{
			if (uuid.length() != BLUETOOTH_UUID_128_LENGTH)
				return;

			auto validateUuid128 = [](std::string uuid) {
				auto iter = uuid.begin();
				unsigned int pos = 0;

				while (iter != uuid.end())
				{
					// verify that hyphens are at the right place
					if (pos == 8 || pos == 13 || pos == 18 || pos == 23)
					{
						if (*iter != '-')
							return false;
					}
					// all other characters are allowed to be alpha-numeric only
					else if(!isalnum(*iter))
						return false;

					pos++;
					iter++;
				}

				return true;
			};

			if (!validateUuid128(uuid))
				return;

			type = UUID128;
		}
	}

private:
	std::string uuid;
	Type type;
};

typedef std::vector<BluetoothUuid> BluetoothUuidList;

namespace std
{
	template <>
	struct hash<BluetoothUuid>
	{
		std::size_t operator()(const BluetoothUuid &uuid) const
		{
			return hash<string>()(uuid.toString());
		}
	};
}

#endif // BLUETOOTH_SIL_UUID_H_
