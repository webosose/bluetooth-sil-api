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

#include <glib.h>

#include <iostream>
#include <typeinfo>
#include <unordered_map>

#include "bluetooth-sil-api.h"

static void test_uuid_correct_validation(void)
{
	BluetoothUuid uuid16("1eef");
	g_assert(uuid16.isValid());
	g_assert(uuid16.getType() == BluetoothUuid::UUID16);

	BluetoothUuid invalidUuid0("ad,2");
	g_assert(!invalidUuid0.isValid());

	BluetoothUuid uuid32("11dd3344");
	g_assert(uuid32.isValid());
	g_assert(uuid32.getType() == BluetoothUuid::UUID32);

	BluetoothUuid invalidUuid1("11d,34;4");
	g_assert(!invalidUuid1.isValid());

	BluetoothUuid uuid128("44444444-aaaa-bbbb-1111-112233445555");

	g_assert(uuid128.isValid());
	g_assert(uuid128.getType() == BluetoothUuid::UUID128);

	BluetoothUuid invalidUuid3("44444444-aaaa-bb-b-11-1-112233445555");
	g_assert(!invalidUuid3.isValid());

	BluetoothUuid invalidUuid4("44444444daaaacbbbbb1111a112233445555");
	g_assert(!invalidUuid4.isValid());

	BluetoothUuid invalidUuid5("fdsafdg32r32r932rfdwf3r93f32");
	g_assert(!invalidUuid5.isValid());
}

static void test_uuid_conversion_to_value(void)
{
	BluetoothUuid uuid16("1eef");
	g_assert(uuid16.toUInt16() == 0x1eef);
	g_assert(uuid16.toString() == "1eef");

	BluetoothUuid uuid32("11dd3344");
	g_assert(uuid32.toUInt16() == 0x3344);
	g_assert(uuid32.toUInt32() == 0x11dd3344);
	g_assert(uuid32.toString() == "11dd3344");

	BluetoothUuid invalidUuid0("ad,2");
	g_assert(!invalidUuid0.isValid());
	g_assert(invalidUuid0.toUInt16() == 0x0);
	g_assert(invalidUuid0.toUInt32() == 0x0);

	BluetoothUuid invalidUuid1("11d,34;4");
	g_assert(!invalidUuid1.isValid());
	g_assert(invalidUuid1.toUInt16() == 0x0);
	g_assert(invalidUuid1.toUInt32() == 0x0);

	BluetoothUuid uuid128("12345678-abcd-ef12-1a1b-112233445566");
	uint128_t value128 = uuid128.toUInt128();
	g_assert(value128.data[0] == 0x12);
	g_assert(value128.data[1] == 0x34);
	g_assert(value128.data[2] == 0x56);
	g_assert(value128.data[3] == 0x78);

	g_assert(value128.data[4] == 0xab);
	g_assert(value128.data[5] == 0xcd);

	g_assert(value128.data[6] == 0xef);
	g_assert(value128.data[7] == 0x12);

	g_assert(value128.data[8] == 0x1a);
	g_assert(value128.data[9] == 0x1b);

	g_assert(value128.data[10] == 0x11);
	g_assert(value128.data[11] == 0x22);
	g_assert(value128.data[12] == 0x33);
	g_assert(value128.data[13] == 0x44);
	g_assert(value128.data[14] == 0x55);
	g_assert(value128.data[15] == 0x66);
}

static void test_uuid_comparison(void)
{
	BluetoothUuid uuid("abc3fff0-c71f-11e4-8731-1681e6b88ec1");

	g_assert(uuid == "abc3fff0-c71f-11e4-8731-1681e6b88ec1");
	g_assert(uuid == BluetoothUuid("abc3fff0-c71f-11e4-8731-1681e6b88ec1"));

	g_assert(uuid != "aa");
	g_assert(uuid != "123aef343243");
	g_assert(uuid != "abc3fff0-c71f-11e4-1234-1681e6b88ec1");
	g_assert(!(uuid != "abc3fff0-c71f-11e4-8731-1681e6b88ec1"));
}

static void test_uuid_hash(void)
{
	std::unordered_map<BluetoothUuid, int> map;
	map.insert({ BluetoothUuid("abc3fff0-c71f-11e4-8731-1681e6b88ec1"), 10 });
	g_assert(map.find(BluetoothUuid("abc3fff0-c71f-11e4-8731-1681e6b88ec1")) != map.end());
}

int main(int argc, char **argv)
{
	g_test_init(&argc, &argv, nullptr);

	g_test_add_func("/uuid/input-validation", test_uuid_correct_validation);
	g_test_add_func("/uuid/conversion-to-value", test_uuid_conversion_to_value);
	g_test_add_func("/uuid/comparision", test_uuid_comparison);
	g_test_add_func("/uuid/hash", test_uuid_hash);

	return g_test_run();
}
