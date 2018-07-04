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

#include "sil-tester.h"
#include "utils.h"

using namespace std;

typedef struct
{
	bool unused;
} Fixture;

class TestPairData
{
public:
	std::string address;
	BluetoothPairingSecretType secretType;
	BluetoothPasskey passkey;
	std::string pin;
	bool expectSuccess;

	TestPairData() :
		address(""),
		secretType(BLUETOOTH_PAIRING_SECRET_TYPE_PASSKEY),
		passkey(0),
		pin(""),
		expectSuccess(true)
	{
	}
	TestPairData(std::string init_address, bool init_expectSuccess) :
		address(init_address),
		expectSuccess(init_expectSuccess)
	{
	}
	TestPairData(std::string init_address, std::string init_pin, bool init_expectSuccess) :
		address(init_address),
		secretType(BLUETOOTH_PAIRING_SECRET_TYPE_PIN),
		pin(init_pin),
		expectSuccess(init_expectSuccess)
	{
	}
	TestPairData(std::string init_address, BluetoothPasskey init_passkey, bool init_expectSuccess) :
		address(init_address),
		secretType(BLUETOOTH_PAIRING_SECRET_TYPE_PASSKEY),
		passkey(init_passkey),
		expectSuccess(init_expectSuccess)
	{
	}
	virtual ~TestPairData()
	{
		address.clear();
		pin.clear();
	}
};

static TestAdapterObserver *observer;
static bool gPairFailed = false;
static guint gCheckSource = 0;
static guint gTimeoutSource = 0;
static guint gIdleSource = 0;
static int check_pairing_state_change_count = 0;
static std::string discovered_device_address;

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

static void unpair_result_ignore_cb(BluetoothError error)
{
	DEBUG_MSG("Inside unpair_result_ignore_cb, error is " << error << "; (ignoring error)");
	//g_assert_true(BLUETOOTH_ERROR_NONE == error);
}

static void pair_result_success_cb(BluetoothError error)
{
	DEBUG_MSG("Inside pair_result_success_cb, error is " << error << "; (expecting: no error)");
	g_assert_true(BLUETOOTH_ERROR_NONE == error);

	// got pairing result so this test is complete
	gPairFailed = true;
	clear_source(&gCheckSource);
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);
}

static void pair_result_fail_cb(BluetoothError error)
{
	DEBUG_MSG("Inside pair_result_fail_cb, error is " << error << "; (expecting: error)");
	g_assert_true(BLUETOOTH_ERROR_NONE != error);

	// got pairing result so this test is complete
	gPairFailed = true;
	clear_source(&gCheckSource);
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);
}

static gboolean check_pairing_state_change(gpointer user_data)
{
	DEBUG_MSG("Inside check_pairing_state_change # " << ++check_pairing_state_change_count);
	TestPairData* testPairData = static_cast<TestPairData*>(user_data);
	g_assert_nonnull(testPairData);
	g_assert_nonnull(observer);

	if (observer->pairingConfirmationRequested)
	{
		// pairingConfirmationRequested; confirm by calling supplyPairingConfirmation
		DEBUG_MSG("Inside check_pairing_state_change; got pairingConfirmationRequested");
		DEBUG_MSG("Inside check_pairing_state_change; device: " << observer->pairingDeviceAddress << " and passkey: " << observer->pairingConfirmationPasskey);
		g_assert_equal(observer->pairingDeviceAddress, testPairData->address);
		observer->pairingConfirmationRequested = false;
		clear_source(&gCheckSource);

		// return TRUE or FALSE for user confirmation as instructed by test
		DEBUG_MSG("Inside check_pairing_state_change, calling supplyPairingConfirmation with " << (testPairData->expectSuccess ? "TRUE" : "FALSE"));
		BluetoothError error = defaultAdapter->supplyPairingConfirmation(observer->pairingDeviceAddress, testPairData->expectSuccess /*accept*/);
		return FALSE;
	}
	else if (observer->pairingDisplaySecretRequested)
	{
		DEBUG_MSG("Inside check_pairing_state_change; got pairingDisplaySecretRequested");
		// pairingDisplaySecretRequested; no confirmation needed
		if (BLUETOOTH_PAIRING_SECRET_TYPE_PIN == observer->pairingSecretType)
		{
			DEBUG_MSG("Inside check_pairing_state_change; device: " << observer->pairingDeviceAddress << " and pin: " << observer->pairingConfirmationPin);
		}
		else
		{
			DEBUG_MSG("Inside check_pairing_state_change; device: " << observer->pairingDeviceAddress << " and passkey: " << observer->pairingConfirmationPasskey);
		}

		g_assert_equal(observer->pairingDeviceAddress, testPairData->address);
		if (testingMockSil)
		{
			// Skip this assertion for non Mock SIL
			g_assert_equal(observer->pairingConfirmationPin, testPairData->pin);
		}
		observer->pairingDisplaySecretRequested = false;
		clear_source(&gCheckSource);
		return FALSE;
	}
	else if (observer->pairingSecretRequested)
	{
		// pairingSecretRequested; confirm by calling supplyPairingSecret
		DEBUG_MSG("Inside check_pairing_state_change; got pairingSecretRequested");
		DEBUG_MSG("Inside check_pairing_state_change; device: " << observer->pairingDeviceAddress << " and type: " << observer->pairingSecretType);
		g_assert_equal(observer->pairingDeviceAddress, testPairData->address);
		if (testingMockSil)
		{
			// Skip this assertion for non Mock SIL
			g_assert_equal(observer->pairingSecretType, testPairData->secretType);
		}
		observer->pairingSecretRequested = false;
		clear_source(&gCheckSource);

		// return (good or bad) Pin or Passkey for user confirmation as requested
		if (BLUETOOTH_PAIRING_SECRET_TYPE_PIN == observer->pairingSecretType)
		{
			DEBUG_MSG("Inside check_pairing_state_change, calling supplyPairingSecret with pin: " << testPairData->pin);
			// TODO: This will presumably FAIL for non-MOCK SIL (so expect that):
			BluetoothError error = defaultAdapter->supplyPairingSecret(testPairData->address, testPairData->pin);
		}
		else
		{
			DEBUG_MSG("Inside check_pairing_state_change, calling supplyPairingSecret with passkey: " << testPairData->passkey);
			// TODO: This will presumably FAIL for non-MOCK SIL (so expect that):
			BluetoothError error = defaultAdapter->supplyPairingSecret(testPairData->address, testPairData->passkey);
		}
		return FALSE;
	}
	else
	{
		// state has not changed yet; keep checking...
		return TRUE;
	}
}

static gboolean check_pairing_timeout(gpointer user_data)
{
	// this is the overall timeout that should only fire if (e.g.) check_pairing_state_change never succeeds
	DEBUG_MSG("Inside check_pairing_timeout which means a test did not succeed within the timeout period.");
	g_assert_not_reached();
	clear_source(&gCheckSource);
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);
	return FALSE;
}

void get_discovered_device_uuids_cb(BluetoothError error,
                       const BluetoothPropertiesList &properties)
{
	std::vector<std::string> uuids;
	int num_device_uuids = 0;

	g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	DEBUG_MSG("Inside get_device_uuids_cb:");

	// Check for BluetoothProperty::Type::UUIDS and see how many we got (if any)
	for (auto prop : properties)
	{
		g_assert_notequal(prop.getType(), (BluetoothProperty::Type::EMPTY));
		DEBUG_MSG("  Got property: " << prop.getType());

		if (prop.getType() == BluetoothProperty::Type::UUIDS)
		{
			DEBUG_MSG("  Got UUIDS property:");
			uuids = prop.getValue<std::vector<std::string>>();
			num_device_uuids = uuids.size();
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
	g_assert_notequal(num_device_uuids, 0);
}

static gboolean check_device_discovered(gpointer user_data)
{
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

		// get the device uuids for our discovered device
		defaultAdapter->getDeviceProperties(discovered_device_address, get_discovered_device_uuids_cb);

		// all done
		g_main_loop_quit(mainLoop);
		return FALSE;
	}

	return TRUE;
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

// Perform incoming pairing test (Mock SIL only)
static gboolean setup_test_incoming_pair(gpointer user_data)
{
	TestPairData* testPairData = static_cast<TestPairData*>(user_data);
	clear_source(&gIdleSource);

	// unpair before requesting incoming pair (to ensure we're not already paired), and ignore any error in callback
	DEBUG_MSG("Calling unpair(" << testPairData->address << ")");

	defaultAdapter->unpair(testPairData->address, unpair_result_ignore_cb);

	// if Mock SIL, then poke Mock SIL and tell it which device we'd like to get an incoming pairing request from
	if (testingMockSil)
	{
		// poke Mock SIL to initiate (incoming) pairing request
		DEBUG_MSG("Requesting incoming pair for (" << testPairData->address << ")");
		defaultAdapter->testRequestIncomingPair(testPairData->address, testPairData->expectSuccess ? pair_result_success_cb : pair_result_fail_cb);
	}

	// call check_pairing_state_change every second; overall timeout is 15 seconds
	check_pairing_state_change_count = 0;
	gCheckSource = g_timeout_add(1000, check_pairing_state_change, testPairData);
	gTimeoutSource = g_timeout_add(15000, check_pairing_timeout, (void *) NULL);

	return FALSE;
}

static void test_incoming_pair(Fixture *fixture, gconstpointer tdata)
{
	gIdleSource = g_idle_add(setup_test_incoming_pair, (gpointer) tdata);
	g_main_loop_run(mainLoop);
}

// Perform outgoing pairing test
static gboolean start_outgoing_pair(gpointer user_data)
{
	TestPairData* testPairData = static_cast<TestPairData*>(user_data);
	clear_source(&gCheckSource);

	// TODO: If NOT Mock SIL (e.g. Bluez5 SIL), enable discovery to fetch a BT address, and use that address for outgoing pairing request
	// Of course, we still can't easily test pairing in any case that requires interaction with remote device or knowing a passkey/pin...
	// cancel discovery before pair operation., SIL(ex: BSA) needs discovery to be cancelled
	defaultAdapter->cancelDiscovery([](BluetoothError error) {
		g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	});

	// unpair before requesting incoming pair (to ensure we're not already paired), and ignore any error in callback
	DEBUG_MSG("Calling unpair(" << testPairData->address << ")");

	defaultAdapter->unpair(testPairData->address, unpair_result_ignore_cb);

	// initiate (outgoing) pairing request
	DEBUG_MSG("Calling Pair(" << testPairData->address << ")");
	gPairFailed = false;
	defaultAdapter->pair(testPairData->address, testPairData->expectSuccess ? pair_result_success_cb : pair_result_fail_cb);

	// if we did NOT get an immediate (synchronous) failure from pair request, then wait for pairing to finish
	// Otherwise, if the test has already failed, this gCheckSource timeout will cause the NEXT test to fail!!
	if (!gPairFailed)
	{
		// call check_pairing_state_change every second; overall timeout is 15 seconds
		check_pairing_state_change_count = 0;
		gCheckSource = g_timeout_add(1000, check_pairing_state_change, testPairData);
		gTimeoutSource = g_timeout_add(15000, check_pairing_timeout, (void *) NULL);
	}

	return FALSE;
}

static gboolean setup_test_outgoing_pair(gpointer user_data)
{
	gCheckSource = g_timeout_add(1000, start_outgoing_pair, user_data);
	clear_source(&gIdleSource);
}

static void test_outgoing_pair(Fixture *fixture, gconstpointer tdata)
{
	gIdleSource = g_idle_add(setup_test_outgoing_pair, (gpointer) tdata);
	g_main_loop_run(mainLoop);
}

static void set_property_cb(BluetoothError error)
{
	g_assert_true(BLUETOOTH_ERROR_NONE == error);
}

static gboolean setup_test_get_device_uuids(gpointer user_data)
{
	DEBUG_MSG("");
	clear_source(&gIdleSource);

	g_assert_nonnull(defaultAdapter);

	BluetoothProperty new_property(BluetoothProperty::Type::DISCOVERY_TIMEOUT, (uint32_t)7);
	defaultAdapter->setAdapterProperty(new_property, set_property_cb);

	g_assert_equal(defaultAdapter->startDiscovery(), BLUETOOTH_ERROR_NONE);

	// call check_device_discovered every 2 sec, overall timeout is 10 sec
	gCheckSource = g_timeout_add(2000, check_device_discovered, (void *) NULL);
	gTimeoutSource = g_timeout_add(10000, check_discovery_timeout, (void *) NULL);

	return FALSE;
}

static void test_get_device_uuids(Fixture *fixture, gconstpointer tdata)
{
	gIdleSource = g_idle_add(setup_test_get_device_uuids, (gpointer) tdata);
	g_main_loop_run(mainLoop);
}

// Setup for a test
static void fsetup(Fixture *fixture, gconstpointer unused)
{
	DEBUG_MSG("");
	g_assert_nonnull(defaultAdapter);
	observer = new TestAdapterObserver();
	defaultAdapter->registerObserver(observer);
	DEBUG_MSG("");
	defaultAdapter->enable();
}

// Tear down after a test
static void fteardown(Fixture *fixture, gconstpointer tdata)
{
	defaultAdapter->registerObserver(NULL);
	delete observer;
	defaultAdapter->disable();
	gpointer gdata = (gpointer) tdata;
	TestPairData* testPairData = static_cast<TestPairData*>(gdata);
	delete testPairData;
}

/* Pairing info for test devices implemented in mock sil (in mockbluetoothadapter.cpp)
std::map<std::string, BluetoothPasskey> remoteAddressToPasskey =
{
	// remote device addresses with passkey (Secure Simple Pairing) pairing
	{"aa:bb:cc:dd:ee:00", 999900},
	{"aa:bb:cc:dd:ee:11", 999911},
	{"aa:bb:cc:dd:ee:22", 999922},
	{"aa:bb:cc:dd:ee:33", 999933}
};

std::map<std::string, std::string> remoteAddressToPin =
{
	// remote device addresses with pin (legacy) pairing
	{"aa:bb:cc:dd:ff:aa", "aa123"}
};

std::map<std::string, BluetoothPairingIOCapability> remoteAddressToCapability =
{
	// remote device addresses with passkey (Secure Simple Pairing) pairing
	{"aa:bb:cc:dd:ee:00", BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT},
	{"aa:bb:cc:dd:ee:11", BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_ONLY},
	{"aa:bb:cc:dd:ee:22", BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_YES_NO},
	{"aa:bb:cc:dd:ee:33", BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_ONLY},
	// remote device addresses with pin (legacy) pairing
	{"aa:bb:cc:dd:ff:aa", BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT}	// legacy
};
*/

static void add_mock_pairing_passkey_tests()
{
	// TestPairData holds the device address, a pin or passkey (and secretType) to be displayed or entered,
	// and a flag indicating whether we are (testing and) expecting the pairing test to succeed or fail.
	// We allocate a new tdata object for each test, and it gets deleted in fteardown following each test.
	TestPairData *tdata;

	// These tests are for Mock SIL only:
	if (testingMockSil)
	{
		// Note that BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_DISPLAY is treated as BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_YES_NO by Mock SIL

		if ( (btPairingIOCapability == BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_DISPLAY) ||
			 (btPairingIOCapability == BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_YES_NO)   ||
			 (btPairingIOCapability == BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_ONLY) )
		{
			// ---------------------------------------------------------
			// OUTGOING PAIRING TESTS -- PASSKEY (Secure Simple Pairing)
			// ---------------------------------------------------------

			// Local (webOS)   direction   Remote             Result (callback)
			// ----------------------------------------------------------------
			// DisplayYesNo    outgoing    KeyboardOnly       displayPasskeyPin
			// DisplayYesNo    outgoing    NoInputNoOutput    confirmPasskey
			// KeyboardOnly    outgoing    KbdOnly            enterPasskeyPin
			// KeyboardOnly    outgoing    NoInputNoOutput    autoConfirm

			// Test outgoing pairing to NoInputNoOutput device (DisplayYesNo should call confirmPasskey; KeyboardOnly should call autoConfirm)
			tdata = new TestPairData("aa:bb:cc:dd:ee:00", 999900, true);
			g_test_add("/SIL/Adapter/Pairing/Passkey/Outgoing/NoInputNoOutput/Pass", Fixture, (gconstpointer) tdata, fsetup, test_outgoing_pair, fteardown);

			// Note: We can't ask remote KeyboardOnly device to reject an (autoConfirm) test so we only test this failure case with Display device
			if ( (btPairingIOCapability == BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_DISPLAY)   ||
				 (btPairingIOCapability == BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_YES_NO) )
			{
				tdata = new TestPairData("aa:bb:cc:dd:ee:00", 444444, false);
				g_test_add("/SIL/Adapter/Pairing/Passkey/Outgoing/NoInputNoOutput/Fail", Fixture, (gconstpointer) tdata, fsetup, test_outgoing_pair, fteardown);
			}

			// Test outgoing pairing to KeyboardOnly device (DisplayYesNo should call enterPasskeyPin; KeyboardOnly should call displayPasskeyPin)
			tdata = new TestPairData("aa:bb:cc:dd:ee:33", 999933, true);
			g_test_add("/SIL/Adapter/Pairing/Passkey/Outgoing/KeyboardOnly/Pass", Fixture, (gconstpointer) tdata, fsetup, test_outgoing_pair, fteardown);

			// Note: We can't ask remote KeyboardOnly device to reject a (displayPasskeyPin) test so we only test this failure case with KeyboardOnly device
			if (btPairingIOCapability == BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_ONLY)
			{
				tdata = new TestPairData("aa:bb:cc:dd:ee:33", 333333, false);
				g_test_add("/SIL/Adapter/Pairing/Passkey/Outgoing/KeyboardOnly/Fail", Fixture, (gconstpointer) tdata, fsetup, test_outgoing_pair, fteardown);
			}

			// ---------------------------------------------------------
			// INCOMING PAIRING TESTS -- PASSKEY (Secure Simple Pairing)
			// ---------------------------------------------------------

			// Local (webOS)   direction   Remote             Result (callback)
			// ----------------------------------------------------------------
			// DisplayYesNo    incoming    KeyboardOnly       displayPasskeyPin
			// DisplayYesNo    incoming    DisplayYesNo       confirmPasskey
			// DisplayYesNo    incoming    NoInputNoOutput    confirmPasskey
			// KeyboardOnly    incoming    KeyboardOnly       enterPasskeyPin
			// KeyboardOnly    incoming    DisplayYesNo       enterPasskeyPin
			// KeyboardOnly    incoming    NoInputNoOutput    autoConfirm

			// Test incoming pairing from NoInputNoOutput device (DisplayYesNo should get confirmPasskey; KeyboardOnly should get autoConfirm)
			tdata = new TestPairData("aa:bb:cc:dd:ee:00", 999900, true);
			g_test_add("/SIL/Adapter/Pairing/Passkey/Incoming/NoInputNoOutput/Pass", Fixture, (gconstpointer) tdata, fsetup, test_incoming_pair, fteardown);

			// Note: We can't ask remote NoInputNoOutput device to reject a (displayPasskeyPin) test and can't reject autoConfirm, so skip this test.
			//tdata = new TestPairData("aa:bb:cc:dd:ee:00", 444444, false);
			//g_test_add("/SIL/Adapter/Pairing/Passkey/Incoming/NoInputNoOutput/Fail", Fixture, (gconstpointer) tdata, fsetup, test_incoming_pair, fteardown);

			// Test incoming pairing from DisplayYesNo device (DisplayYesNo should get confirmPasskey; KeyboardOnly should get enterPasskeyPin)
			tdata = new TestPairData("aa:bb:cc:dd:ee:22", 999922, true);
			g_test_add("/SIL/Adapter/Pairing/Passkey/Incoming/DisplayYesNo/Pass", Fixture, (gconstpointer) tdata, fsetup, test_incoming_pair, fteardown);

			// When remote device is DisplayYesNo, we can test failure case since we are DisplayYesNo (confirmPasskey) or KeyboardOnly (enterPasskeyPin)
			tdata = new TestPairData("aa:bb:cc:dd:ee:22", 222222, false);
			g_test_add("/SIL/Adapter/Pairing/Passkey/Incoming/DisplayYesNo/Fail", Fixture, (gconstpointer) tdata, fsetup, test_incoming_pair, fteardown);

			// Test incoming pairing from KeyboardOnly device (DisplayYesNo should call displayPasskeyPin; KeyboardOnly should call enterPasskeyPin)
			tdata = new TestPairData("aa:bb:cc:dd:ee:33", 999933, true);
			g_test_add("/SIL/Adapter/Pairing/Passkey/Incoming/KeyboardOnly/Pass", Fixture, (gconstpointer) tdata, fsetup, test_incoming_pair, fteardown);

			// Note: We can't ask remote KeyboardOnly device to reject a (displayPasskeyPin) test and can't reject autoConfirm, so skip this test.
			// tdata = new TestPairData("aa:bb:cc:dd:ee:33", 333333, false);
			// g_test_add("/SIL/Adapter/Pairing/Passkey/Incoming/KeyboardOnly/Fail", Fixture, (gconstpointer) tdata, fsetup, test_incoming_pair, fteardown);
		}
	}

	// TODO: Consider adding tests for (testing) cancelling during pairing...
}

static void add_mock_pairing_pin_tests()
{
	// TestPairData holds the device address, a pin or passkey (and secretType) to be displayed or entered,
	// and a flag indicating whether we are (testing and) expecting the pairing test to succeed or fail.
	TestPairData *tdata;

	// These pin tests are for Mock SIL only:
	if (testingMockSil)
	{
		if (btPairingIOCapability == BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_DISPLAY)
		{
			// ---------------------------------------------
			// LEGACY PAIRING TESTS -- PIN (LEGACY DEVICE)
			// ---------------------------------------------

			// Local (webOS)   direction   Remote      Result (callback)
			// ---------------------------------------------------------
			// KeyboardDisplay    outgoing    legacy      enterPasskeyPin
			// KeyboardDisplay    incoming    legacy      enterPasskeyPin

			// Test outgoing pairing to Legacy device (KeyboardDisplay should call enterPasskeyPin)
			tdata = new TestPairData("aa:bb:cc:dd:ff:aa", "aa123", true);
			g_test_add("/SIL/Adapter/Pairing/Pin/Outgoing/KeyboardDisplay/Legacy/Pass", Fixture, (gconstpointer) tdata, fsetup, test_outgoing_pair, fteardown);

			tdata = new TestPairData("aa:bb:cc:dd:ff:aa", "xx123", false);
			g_test_add("/SIL/Adapter/Pairing/Pin/Outgoing/KeyboardDisplay/Legacy/Fail", Fixture, (gconstpointer) tdata, fsetup, test_outgoing_pair, fteardown);


			// Test incoming pairing from legacy device (KeyboardDisplay should get enterPasskeyPin)
			tdata = new TestPairData("aa:bb:cc:dd:ff:aa", "aa123", true);
			g_test_add("/SIL/Adapter/Pairing/Pin/Incoming/KeyboardDisplay/Legacy/Pass", Fixture, (gconstpointer) tdata, fsetup, test_incoming_pair, fteardown);

			tdata = new TestPairData("aa:bb:cc:dd:ff:aa", "xx123", false);
			g_test_add("/SIL/Adapter/Pairing/Pin/Incoming/KeyboardDisplay/Legacy/Fail", Fixture, (gconstpointer) tdata, fsetup, test_incoming_pair, fteardown);
		}
		else if ( (btPairingIOCapability == BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_YES_NO) ||
				  (btPairingIOCapability == BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_ONLY) )
		{
			// ---------------------------------------------
			// LEGACY PAIRING TESTS -- PIN (LEGACY DEVICE)
			// ---------------------------------------------

			// Local (webOS)   direction   Remote      Result (callback)
			// ---------------------------------------------------------
			// DisplayYesNo    outgoing    legacy      displayPasskeyPin
			// DisplayOnly     outgoing    legacy      displayPasskeyPin
			// DisplayYesNo    incoming    legacy      displayPasskeyPin
			// DisplayOnly     incoming    legacy      displayPasskeyPin

			// Test outgoing pairing to Legacy device (DisplayYesNo/DisplayOnly should call displayPasskeyPin)
			tdata = new TestPairData("aa:bb:cc:dd:ff:aa", "aa123", true);
			g_test_add("/SIL/Adapter/Pairing/Pin/Outgoing/DisplayOnly/Legacy/Pass", Fixture, (gconstpointer) tdata, fsetup, test_outgoing_pair, fteardown);

			// Note: We can't ask remote legacy device to reject a (displayPasskeyPin) test, so skip this test.
			// tdata = new TestPairData("aa:bb:cc:dd:ff:aa", "xx123", false);
			// g_test_add("/SIL/Adapter/Pairing/Pin/Outgoing/DisplayOnly/Legacy/Fail", Fixture, (gconstpointer) tdata, fsetup, test_outgoing_pair, fteardown);


			// Test incoming pairing from legacy device (DisplayYesNo/DisplayOnly should get displayPasskeyPin)
			tdata = new TestPairData("aa:bb:cc:dd:ff:aa", "aa123", true);
			g_test_add("/SIL/Adapter/Pairing/Pin/Incoming/DisplayOnly/Legacy/Pass", Fixture, (gconstpointer) tdata, fsetup, test_incoming_pair, fteardown);

			// Note: We can't ask remote legacy device to reject a (displayPasskeyPin) test, so skip this test.
			// tdata = new TestPairData("aa:bb:cc:dd:ff:aa", "xx123", false);
			// g_test_add("/SIL/Adapter/Pairing/Pin/Incoming/DisplayOnly/Legacy/Fail", Fixture, (gconstpointer) tdata, fsetup, test_incoming_pair, fteardown);
		}
		else if (btPairingIOCapability == BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_ONLY)
		{
			// ---------------------------------------------
			// LEGACY PAIRING TESTS -- PIN (LEGACY DEVICE)
			// ---------------------------------------------

			// Local (webOS)   direction   Remote      Result (callback)
			// ---------------------------------------------------------
			// KeyboardOnly    outgoing    legacy      unsupported
			// KeyboardOnly    incoming    legacy      enterPasskeyPin

			// Note: Outgoing pairing can't be initiated from a KeyboardOnly to a legacy device so expect this to fail.
			tdata = new TestPairData("aa:bb:cc:dd:ff:aa", "xx123", false /*expectSuccess*/);
			g_test_add("/SIL/Adapter/Pairing/Pin/Outgoing/KeyboardOnly/Legacy/Fail", Fixture, (gconstpointer) tdata, fsetup, test_outgoing_pair, fteardown);


			// Test incoming pairing from legacy device (KeyboardOnly should get enterPasskeyPin)
			tdata = new TestPairData("aa:bb:cc:dd:ff:aa", "aa123", true);
			g_test_add("/SIL/Adapter/Pairing/Pin/Incoming/KeyboardOnly/Legacy/Pass", Fixture, (gconstpointer) tdata, fsetup, test_incoming_pair, fteardown);

			tdata = new TestPairData("aa:bb:cc:dd:ff:aa", "xx123", false);
			g_test_add("/SIL/Adapter/Pairing/Pin/Incoming/KeyboardOnly/Legacy/Fail", Fixture, (gconstpointer) tdata, fsetup, test_incoming_pair, fteardown);
		}

	}

	// TODO: Consider adding tests for (testing) cancelling during pairing...
}

static void add_device_pairing_tests()
{
	// These tests are for Device SIL only, and really only helpful during SIL development
	// since it's hard to know what remote device(s) may or may not be available for pairing
	// and the sil-tester can't really interact with those remote devices...

	// TestPairData holds the device address, a pin or passkey (and secretType) to be displayed or entered,
	// and a flag indicating whether we are (testing and) expecting the pairing test to succeed or fail.
	TestPairData *tdata;

	// Test outgoing pairing to device address specified on the command line
	// Currently this test case is blocked with Mock Sil. Since testing the same with exact BT Stack SIL
	// needs additional effort of cancelling paring from remote device within specific period of time.
	if (testingMockSil)
	{
		tdata = new TestPairData(btPairingPartnerAddr, "0000", false);
		g_test_add("/SIL/Adapter/Pairing/Device/Outgoing/Fail", Fixture, (gconstpointer) tdata, fsetup, test_outgoing_pair, fteardown);
	}

	// Test outgoing pairing to device address specified on the command line, this is for success case
	// Do not cleanup and disable BT,once disable, clean up takes time so disable BT once testing is over
	tdata = new TestPairData(btPairingPartnerAddr, "0000", true);
	g_test_add("/SIL/Adapter/Pairing/Device/Outgoing/Success", Fixture, (gconstpointer) tdata, fsetup, test_outgoing_pair, NULL);

	// For (device) incoming pairing, check SIL/device capapbility to know what kind of request to expect
	// (no input, no output gets automatically paired and only notification is devicePropertiesChanged )

	// TODO: Consider adding tests for (testing) cancelling during pairing...
}

static void add_device_uuids_tests()
{
	// Some bluetooth stacks may inquire for services only once device is paired,hence post pairing
	// execute this testcase

	g_test_add("/SIL/Adapter/Device/UUID/search", Fixture, NULL, NULL, test_get_device_uuids, fteardown);
}

REGISTER_TEST_MODULE(add_mock_pairing_passkey_tests)
REGISTER_TEST_MODULE(add_mock_pairing_pin_tests)
REGISTER_TEST_MODULE(add_device_pairing_tests)
REGISTER_TEST_MODULE(add_device_uuids_tests)
SET_PROFILE()
