// Copyright (c) 2014-2018 LG Electronics, Inc.
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

#include "sil-tester.h"
#include "utils.h"

using namespace std;
bool discoverable = false;
bool set_discoverable = false;
uint32_t discoverable_timeout;
uint32_t set_discoverable_timeout = 100;

static TestAdapterObserver *observer;
static guint gSource = 0;
static guint gIdleSource = 0;

static gboolean check_property_change(gpointer user_data)
{
	g_assert_nonnull(observer);
	g_assert_true(observer->adapterPropertiesChangedFlag);
	clear_source(&gSource);
	g_main_loop_quit(mainLoop);
	return FALSE;
}

/* Callback functions */
static void get_discoverable_property_cb(BluetoothError error,
        const BluetoothProperty &prop)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	discoverable = prop.getValue<bool>();
}

void get_discoverable_timeout_property_cb(BluetoothError error,
        const BluetoothProperty &prop)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	discoverable_timeout = prop.getValue<uint32_t>();
}

static void verify_discoverable_property_cb(BluetoothError error,
        const BluetoothProperty &prop)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	g_assert_equal(prop.getValue<bool>(), set_discoverable);
}

static void verify_discoverable_timeout_property_cb(BluetoothError error,
        const BluetoothProperty &prop)
{
	g_assert_true(BLUETOOTH_ERROR_NONE == error);
	g_assert_equal(prop.getValue<uint32_t>(), set_discoverable_timeout);
}

void get_properties_cb(BluetoothError error,
                       const BluetoothPropertiesList &properties)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);

	for (auto prop : properties)
	{
		g_assert_notequal(prop.getType(), (BluetoothProperty::Type::EMPTY));
	}
}

void get_adapter_uuids_cb(BluetoothError error,
                       const BluetoothPropertiesList &properties)
{
	std::vector<std::string> uuids;
	int num_adapter_uuids = 0;

	g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	DEBUG_MSG("Inside get_adapter_uuids_cb:");

	// Check for BluetoothProperty::Type::UUIDS and see how many we got (if any)
	for (auto prop : properties)
	{
		g_assert_notequal(prop.getType(), (BluetoothProperty::Type::EMPTY));
		DEBUG_MSG("  Got property: " << prop.getType());

		if (prop.getType() == BluetoothProperty::Type::UUIDS)
		{
			DEBUG_MSG("  Got UUIDS property:");
			uuids = prop.getValue<std::vector<std::string>>();
			num_adapter_uuids = uuids.size();
#if WEBOS_DEBUG
			for (auto uuid : uuids)
			{
				DEBUG_MSG("  Got uuid: " << uuid);
			}
#endif
			break;
		}
	}
	// make sure we found at least one uuid
	g_assert_notequal(num_adapter_uuids, 0);
}

void verify_properties_change_cb(BluetoothError error,
                                 const BluetoothPropertiesList &properties)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);

	for (auto prop : properties)
	{
		g_assert_notequal(prop.getType(), (BluetoothProperty::Type::EMPTY));

		if (prop.getType() == BluetoothProperty::Type::DISCOVERABLE)
		{
			g_assert_equal(prop.getValue<bool>(), set_discoverable);
		}

		if (prop.getType() == BluetoothProperty::Type::DISCOVERABLE_TIMEOUT)
		{
			g_assert_equal(prop.getValue<uint32_t>(), set_discoverable_timeout);
		}
	}
}

static void set_property_cb(BluetoothError error)
{
	g_assert_true(BLUETOOTH_ERROR_NONE == error);
}

/* Test functions */
// Test for get_adapter_property
static gboolean setup_test_get_adapter_property(gpointer user_data)
{
	clear_source(&gIdleSource);
	g_assert_nonnull(defaultAdapter);
	defaultAdapter->getAdapterProperty(BluetoothProperty::Type::DISCOVERABLE,
	                                   get_discoverable_property_cb);
	g_main_loop_quit(mainLoop);
	return FALSE;
}

static void test_get_adapter_property(void)
{
	gIdleSource = g_idle_add(setup_test_get_adapter_property, NULL);
	g_main_loop_run(mainLoop);
}

// Test for get_adapter_properties
static gboolean setup_test_get_adapter_properties(gpointer user_data)
{
	clear_source(&gIdleSource);
	g_assert_nonnull(defaultAdapter);
	observer = new TestAdapterObserver();
	defaultAdapter->registerObserver(observer);

	defaultAdapter->getAdapterProperties(get_properties_cb);
	g_main_loop_quit(mainLoop);
	return FALSE;
}

static void test_get_adapter_properties(void)
{
	gIdleSource = g_idle_add(setup_test_get_adapter_properties, NULL);
	g_main_loop_run(mainLoop);

	delete observer;
}

// Test for get_adapter_uuids
static gboolean setup_test_get_adapter_uuids(gpointer user_data)
{
	DEBUG_MSG("");
	clear_source(&gIdleSource);
	g_assert_nonnull(defaultAdapter);
	observer = new TestAdapterObserver();
	defaultAdapter->registerObserver(observer);

	defaultAdapter->getAdapterProperties(get_adapter_uuids_cb);
	g_main_loop_quit(mainLoop);
	return FALSE;
}

static void test_get_adapter_uuids(void)
{
	gIdleSource = g_idle_add(setup_test_get_adapter_uuids, NULL);
	g_main_loop_run(mainLoop);

	delete observer;
}

// Test for set_adapter_property
static gboolean setup_test_set_adapter_property(gpointer user_data)
{
	DEBUG_MSG("");
	clear_source(&gIdleSource);
	g_assert_nonnull(defaultAdapter);
	observer = new TestAdapterObserver();
	defaultAdapter->registerObserver(observer);

	// Remember the discoverable property value before changing it
	defaultAdapter->getAdapterProperty(BluetoothProperty::Type::DISCOVERABLE,
	                                   get_discoverable_property_cb);

	// Reverse the property value before setting it
	if (discoverable)
	{
		set_discoverable = false;
	}
	else
	{
		set_discoverable = true;
	}

	BluetoothProperty new_property(BluetoothProperty::Type::DISCOVERABLE,
	                               set_discoverable);
	defaultAdapter->setAdapterProperty(new_property, set_property_cb);
	gSource = g_timeout_add(100, check_property_change, NULL);
	defaultAdapter->getAdapterProperty(BluetoothProperty::Type::DISCOVERABLE,
	                                   verify_discoverable_property_cb);

	// Put back the old value of discoverable property
	BluetoothProperty reset_property(BluetoothProperty::Type::DISCOVERABLE,
	                                 discoverable);
	defaultAdapter->setAdapterProperty(reset_property, set_property_cb);
	return FALSE;
}

static void test_set_adapter_property(void)
{
	gIdleSource = g_idle_add(setup_test_set_adapter_property, NULL);
	g_main_loop_run(mainLoop);
	delete observer;
}

// Test for set_adapter_properties
static gboolean setup_test_set_adapter_properties(gpointer user_data)
{
	DEBUG_MSG("");
	clear_source(&gIdleSource);
	g_assert_nonnull(defaultAdapter);
	observer = new TestAdapterObserver();
	defaultAdapter->registerObserver(observer);

	// Remember the discoverable and discovery_timeout property values before changing it
	defaultAdapter->getAdapterProperty(BluetoothProperty::Type::DISCOVERABLE,
	                                   get_discoverable_property_cb);
	defaultAdapter->getAdapterProperty(
	    BluetoothProperty::Type::DISCOVERABLE_TIMEOUT,
	    get_discoverable_timeout_property_cb);

	// Reverse the property value before setting it
	if (discoverable)
	{
		set_discoverable = false;
	}
	else
	{
		set_discoverable = true;
	}

	BluetoothPropertiesList propertiesToChange;
	propertiesToChange.push_back(BluetoothProperty(
	                                 BluetoothProperty::Type::DISCOVERABLE, set_discoverable));
	propertiesToChange.push_back(BluetoothProperty(
	                                 BluetoothProperty::Type::DISCOVERABLE_TIMEOUT, set_discoverable_timeout));
	defaultAdapter->setAdapterProperties(propertiesToChange, set_property_cb);
	gSource = g_timeout_add(100, check_property_change, NULL);

	// Verify properties were set properly
	defaultAdapter->getAdapterProperties(verify_properties_change_cb);

	// Put back the old value of discoverable property
	BluetoothPropertiesList propertiesToReset;
	propertiesToReset.push_back(BluetoothProperty(
	                                BluetoothProperty::Type::DISCOVERABLE, discoverable));
	propertiesToReset.push_back(BluetoothProperty(
	                                BluetoothProperty::Type::DISCOVERABLE_TIMEOUT, discoverable_timeout));
	defaultAdapter->setAdapterProperties(propertiesToReset, set_property_cb);
	return FALSE;
}

static void test_set_adapter_properties(void)
{
	gIdleSource = g_idle_add(setup_test_set_adapter_properties, NULL);
	g_main_loop_run(mainLoop);
	delete observer;
}

static void add_tests()
{
	g_test_add_func("/SIL/Adapter/Properties/getAdapterProperty", test_get_adapter_property);
	g_test_add_func("/SIL/Adapter/Properties/getAdapterProperties", test_get_adapter_properties);
	g_test_add_func("/SIL/Adapter/Properties/getAdapterUuids", test_get_adapter_uuids);
	g_test_add_func("/SIL/Adapter/Properties/setAdapterProperty", test_set_adapter_property);
	g_test_add_func("/SIL/Adapter/Properties/setAdapterProperties", test_set_adapter_properties);
}

REGISTER_TEST_MODULE(add_tests)
