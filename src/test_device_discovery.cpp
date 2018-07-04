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

static TestAdapterObserver *observer;
static guint gCheckSource = 0;
static guint gTimeoutSource = 0;
static guint gIdleSource = 0;
static int check_discovery_state_change_count = 0;
static int check_device_discovered_count = 0;

static std::string discovered_device_address;

#define ADAPTER_POWER_ON_TIMEOUT        10000

static void get_device_address(const BluetoothPropertiesList &properties)
{
	DEBUG_MSG("Inside get_device_address:");
	discovered_device_address.clear();

	// Check for BluetoothProperty::Type::UUIDS and see how many we got (if any)
	for (auto prop : properties)
	{
		g_assert_notequal(prop.getType(), (BluetoothProperty::Type::EMPTY));
		DEBUG_MSG("  Got property: " << prop.getType());

		if (prop.getType() == BluetoothProperty::Type::BDADDR)
		{
			discovered_device_address = prop.getValue<std::string>();
			DEBUG_MSG("  Got BDADDR property: " << discovered_device_address);
			break;
		}
	}
	// make sure we found a device address
	g_assert_notequal(discovered_device_address.length(), 0);
}

void get_device_properties_cb(BluetoothError error,
                       const BluetoothPropertiesList &properties)
{
	int rssi = 0;
	discovered_device_address.clear();

	g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	DEBUG_MSG("Inside get_device_properties_cb:");

	// Device needs to be discovered for pairing
	for (auto prop : properties)
	{
		switch (prop.getType())
		{
		case BluetoothProperty::Type::BDADDR:
			discovered_device_address = prop.getValue<std::string>();
			g_assert_notequal(discovered_device_address.length(), 0);
			break;
		case BluetoothProperty::Type::RSSI:
			rssi = prop.getValue<int>();
			g_assert_notequal(rssi, 0);
			break;
		default:
			break;
		}
	}
}

static void set_property_cb(BluetoothError error)
{
	g_assert_true(BLUETOOTH_ERROR_NONE == error);
}

static gboolean check_device_discovered(gpointer user_data)
{
	DEBUG_MSG("Inside check_device_discovered # " << ++check_device_discovered_count);
	g_assert_nonnull(observer);

	if (observer->devicePropertiesChangedFlag)
	{
		// device properties changed; now get our device address
		get_device_address(observer->lastDevicePropertySet);
		clear_source(&gCheckSource);
		clear_source(&gTimeoutSource);

		// make sure we discovered a device
		g_assert_notequal(discovered_device_address.length(), 0);

		// TODO: could confirm address matches, but that depends on SIL calling devicePropertiesChanged rather than deviceFound
		//g_assert_cmpstr(discovered_device_address, ==, observer->lastDevicePropertyAddress);

		// get the device properties to make sure device is found
		defaultAdapter->getDeviceProperties(discovered_device_address, get_device_properties_cb);

		// all done
		g_main_loop_quit(mainLoop);
		return FALSE;
	}
	else
	{
		// device not discovered yet; keep checking...
		return TRUE;
	}
}

static gboolean check_discovery_state_change(gpointer user_data)
{
	DEBUG_MSG("Inside check_discovery_state_change # " << ++check_discovery_state_change_count);
	bool discovery = (bool)user_data;
	g_assert_nonnull(observer);

	if (observer->discoveryStateChangedFlag)
	{
		// state has changed; validate new discoveryStateValue
		g_assert_equal(observer->discoveryStateValue, discovery);
		clear_source(&gCheckSource);
		clear_source(&gTimeoutSource);
		g_main_loop_quit(mainLoop);
		return FALSE;
	}
	else
	{
		// state has not changed yet; keep checking...
		return TRUE;
	}
}

static gboolean check_discovery_timeout(gpointer user_data)
{
	// this is the overall timeout that should only fire if (e.g.) check_discovery_state_change never succeeds
	g_assert_not_reached();
	clear_source(&gCheckSource);
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);
	return FALSE;
}

static gboolean check_power_on_timeout(gpointer user_data)
{
	clear_source(&gCheckSource);

	// TODO: Add support to Mock SIL for DISCOVERY_TIMEOUT.
	// NOTE: Otherwise, we should call cancelDiscovery after startDiscovery.
	BluetoothProperty new_property(BluetoothProperty::Type::DISCOVERY_TIMEOUT, (uint32_t)7);
	defaultAdapter->setAdapterProperty(new_property, set_property_cb);

	g_assert_equal(defaultAdapter->startDiscovery(), BLUETOOTH_ERROR_NONE);

	// call check_discovery_state_change every 100mS; overall timeout is 5 sec
	check_discovery_state_change_count = 0;
	gCheckSource = g_timeout_add(100, check_discovery_state_change, (bool *)true);
	gTimeoutSource = g_timeout_add(5000, check_discovery_timeout, (void *) NULL);

	return FALSE;
}

static gboolean setup_test_start_discovery(gpointer user_data)
{
	DEBUG_MSG("");
	clear_source(&gIdleSource);
	g_assert_nonnull(defaultAdapter);
	observer = new TestAdapterObserver();
	defaultAdapter->registerObserver(observer);
	defaultAdapter->enable();

	// Add delay to start BT stack and initialize profiles registered
	// Timeout added is 10 seconds
	gCheckSource = g_timeout_add(ADAPTER_POWER_ON_TIMEOUT, check_power_on_timeout, (void *) NULL);

	return FALSE;
}

static void test_start_discovery(void)
{
	gIdleSource = g_idle_add(setup_test_start_discovery, NULL);
	g_main_loop_run(mainLoop);
	//TODO: Move this line to tearDown()
	delete observer;
}

static gboolean setup_test_cancel_discovery(gpointer user_data)
{
	DEBUG_MSG("");
	clear_source(&gIdleSource);
	g_assert_nonnull(defaultAdapter);
	observer = new TestAdapterObserver();
	defaultAdapter->registerObserver(observer);

	// make sure discovery is ongoing before we cancel
	g_assert_equal(defaultAdapter->startDiscovery(), BLUETOOTH_ERROR_NONE);
	defaultAdapter->cancelDiscovery([](BluetoothError error) {
		g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	});

	// call check_discovery_state_change every 100mS; overall timeout is 5 sec
	check_discovery_state_change_count = 0;
	gCheckSource = g_timeout_add(100, check_discovery_state_change, (bool *)false);
	gTimeoutSource = g_timeout_add(5000, check_discovery_timeout, (void *) NULL);

	return FALSE;
}

static void test_cancel_discovery(void)
{
	gIdleSource = g_idle_add(setup_test_cancel_discovery, NULL);
	g_main_loop_run(mainLoop);
	delete observer;
}

static gboolean setup_test_get_device_properties(gpointer user_data)
{
	DEBUG_MSG("");
	clear_source(&gIdleSource);
	g_assert_nonnull(defaultAdapter);
	observer = new TestAdapterObserver();
	defaultAdapter->registerObserver(observer);

	// Be sure discovery is stopped, then restart it
	// We need to search for the device for pairing and other operations
	defaultAdapter->cancelDiscovery([](BluetoothError error) {
		g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	});
	g_assert_equal(defaultAdapter->startDiscovery(), BLUETOOTH_ERROR_NONE);

	// call check_device_discovered every 100mS; overall timeout is 5 sec
	check_device_discovered_count = 0;
	gCheckSource = g_timeout_add(100, check_device_discovered, (void *) NULL);
	gTimeoutSource = g_timeout_add(5000, check_discovery_timeout, (void *) NULL);

	return FALSE;
}

static void test_get_device_properties(void)
{
	gIdleSource = g_idle_add(setup_test_get_device_properties, NULL);
	g_main_loop_run(mainLoop);

	delete observer;
}

static void add_tests()
{
	g_test_add_func("/SIL/Adapter/Discovery/startDiscovery", test_start_discovery);
	g_test_add_func("/SIL/Adapter/Discovery/cancelDiscovery", test_cancel_discovery);

	g_test_add_func("/SIL/Adapter/Discovery/getDevice", test_get_device_properties);
}


REGISTER_TEST_MODULE(add_tests)

