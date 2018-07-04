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

#include <iostream>
#include <map>
#include <string>
#include <dlfcn.h>

#include "sil-tester.h"
#include "utils.h"

typedef BluetoothSIL *(*CreateSILFunc)(const int version, BluetoothPairingIOCapability capability);

//
// The global SIL instance to be used by all tests
//
BluetoothSIL *sil;
static void *handle;
GMainLoop *mainLoop = NULL;

static TestSilObserver *silObserver;
static guint gSource = 0;
static guint gIdleSource = 0;
BluetoothAdapter *defaultAdapter = NULL;

std::string silName = "unknown";
bool testingMockSil = false;
BluetoothPairingIOCapability btPairingIOCapability = BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT;
std::string btPairingPartnerAddr;
std::map<std::string, BluetoothProfile*> mProfiles;

// TODO: Consider making a sil API for this so tester and service can both use it.
std::map<std::string, BluetoothPairingIOCapability> mainPairingIOCapability =
{
	{"NoInputNoOutput", BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT},
	{"DisplayOnly", BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_ONLY},
	{"DisplayYesNo", BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_YES_NO},
	{"KeyboardOnly", BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_ONLY},
	{"KeyboardDisplay", BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_DISPLAY}
};

void identify_used_sil(char *path)
{
	std::string str = path;
	unsigned basename = str.rfind("/");
	unsigned extension = str.rfind(".so");
	if (basename && extension)
	{
		silName = str.substr(basename+1, extension-basename-1);
		DEBUG_MSG("silName is: '" << silName << "'");

		testingMockSil = (str.find("mock") != std::string::npos);
		if (testingMockSil)
		{
			DEBUG_MSG("--> mock SIL detected!");
		}
		DEBUG_MSG("");
	}
}

//
// Load the SIL library and credate the BluetoothSIL instance
//
BluetoothSIL *create_sil(char *path, BluetoothPairingIOCapability capability)
{
	// Exit if not path provided
	if (!path)
	{
		std::cerr << "No SIL specified" << std::endl;
		return 0;
	}

	// Get SIL name so tests can know if they're working with Mock SIL or not
	identify_used_sil(path);

	// Load the SIL library
	handle = dlopen(path, RTLD_NOW);
	if (!handle)
	{
		std::cerr << "Failed to load SIL " << path << std::endl;
		return 0;
	}

	// Locate the CreateSILFunc function
	CreateSILFunc createSIL = (CreateSILFunc) dlsym(handle, "createBluetoothSIL");
	if (!createSIL)
	{
		std::cerr << "Failed to load SIL "
		          << path
		          << ". Could not find entry point"
		          << std::endl;
		dlclose(handle);
		return 0;
	}

	// Create the BluetoothSIL instance
	sil = createSIL(BLUETOOTH_SIL_API_VERSION, capability);
	if (!sil)
	{
		std::cerr << "Failed to load SIL "
		          << path
		          << ". Create SIL failed"
		          << std::endl;
		dlclose(handle);
		return 0;
	}

	return sil;
}

static gboolean check_adapter_available(gpointer user_data)
{
	g_assert_true(silObserver->adapterAvailable);
	defaultAdapter = sil->getDefaultAdapter();
	g_assert_nonnull(defaultAdapter);
        clear_source(&gSource);
        g_main_loop_quit(mainLoop);
        return FALSE;
}

static gboolean get_default_adapter(gpointer user_data)
{
	clear_source(&gIdleSource);
	defaultAdapter = sil->getDefaultAdapter();

	if (defaultAdapter)
	{
		g_main_loop_quit(mainLoop);
	}
	else
	{
		silObserver = new TestSilObserver();
		sil->registerObserver(silObserver);
		gSource = g_timeout_add(2000, check_adapter_available, NULL);
	}
	return FALSE;
}

void addProfile(const std::string profile)
{
	if (isProfileEnabled(profile))
		mProfiles.insert({ profile, defaultAdapter->getProfile(profile) });
}

void createProfiles()
{
	addProfile(BLUETOOTH_PROFILE_ID_FTP);
	addProfile(BLUETOOTH_PROFILE_ID_OPP);
	addProfile(BLUETOOTH_PROFILE_ID_A2DP);
	addProfile(BLUETOOTH_PROFILE_ID_GATT);
	addProfile(BLUETOOTH_PROFILE_ID_AVRCP);
	addProfile(BLUETOOTH_PROFILE_ID_SPP);
	addProfile(BLUETOOTH_PROFILE_ID_HFP);
}

//
// usage: bluetooth-sil-tester [g_test options and arguments] <path to SIL .so file>
//
int main(int argc, char **argv)
{
	//
	// Initialize glib's test framework first. It will consume any command line
	// options it recognizes and mutate argc and argv to reflect the result.
	//
	g_test_init(&argc, &argv, NULL);

	//
	// Now attempt to load the SIL library and create the instance before
	// building and running the list of tests.
	//

	//create main loop
	mainLoop = g_main_loop_new(NULL, FALSE);

	if (NULL == mainLoop)
	{
		return -1;
	}

	// Command line options:
	//   bluetooth-sil-tester silname.so btCapability btPairingPartnerAddr
	// e.g.
	//   ./bluetooth-sil-tester ./mock.so DisplayOnly FF:61:71:5E:B1:FC

	// Figure out what device capability we're testing:
	// Read test btPairingIOCapability from second parameter on the command line, e.g.:
	// src/bluetooth-sil-tester -p /SIL/Adapter/Pairing ../../bluetooth-sil-mock/BUILD/mock.so DisplayYesNo
	// or default to DisplayYesNo (BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_YES_NO) if not specified
	// Note: pairingTest.sh shell script is useful for scripted/automated pairing tests.
	std::string bluetoothCapability = argc > 2 ? argv[2] : "DisplayYesNo";
	if (mainPairingIOCapability.find(bluetoothCapability) != mainPairingIOCapability.end())
	{
		btPairingIOCapability = mainPairingIOCapability[bluetoothCapability];
	}
	else
	{
		DEBUG_MSG("ERROR: Specified pairing capability (" << bluetoothCapability << ") not valid!");
		return -1;
	}

	DEBUG_MSG("Creating SIL for API version " << BLUETOOTH_SIL_API_VERSION << ", capability " << bluetoothCapability);

	// Load and create the sil, as specified on the command line
	sil = create_sil(argc > 1 ? argv[1] : 0, btPairingIOCapability);

	if (!testingMockSil)
	{
		// Grab third argument as a MAC address to attempt to pair with
		btPairingPartnerAddr = argc > 3 ? argv[3] : "00:11:22:33:44:55";
	}

	if (!sil)
	{
		return -1;
	}

	gIdleSource = g_idle_add(get_default_adapter, NULL);
	g_main_loop_run(mainLoop);

	if (NULL == defaultAdapter)
	{
		delete sil;
		dlclose(handle);
		return -1;
	}

	createProfiles();

	// Call the registration functions to add all tests, and then run them.
	register_all_tests();
	int result = g_test_run();

	// Delete the SIL instance to free up any resources
	delete sil;
	dlclose(handle);

	// And we're done
	return result;
}
