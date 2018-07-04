
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
#include "test_registry.h"

#include <bluetooth-sil-api.h>

using namespace std;

static TestAdapterObserver *observer;

static guint gCheckSource = 0;
static guint gTimeoutSource = 0;
static guint gIdleSource = 0;

BluetoothProfile *hfpProfile;

template<typename T>
inline T* getImpl() { return dynamic_cast<T*>(hfpProfile); }

gboolean connected = FALSE;
gboolean scoConnected = FALSE;

class TestHfpObserver: public BluetoothHfpStatusObserver
{
public:

	virtual void scoStateChanged(const std::string &address, bool state) { }

	virtual void atCommandReceived(const std::string &address, const BluetoothHfpAtCommand &atCommand)
	{
		// Send response to CIND for SLC connection establishment
		if ("+CIND" == atCommand.getCommand())
			getImpl<BluetoothHfpProfile>()->sendResultCode(btPairingPartnerAddr, "+CIND:0,0,0,6,0,5,0,7");
	}
};

static TestHfpObserver *hfpObserver;

static void hfp_profile_disconnect_callback(BluetoothError error)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);

	connected = FALSE;
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);
}

static void hfp_profile_connect_callback(BluetoothError error)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);

	connected = TRUE;
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);
}

static gboolean check_hfp_state_timeout(gpointer user_data)
{
	// this is the overall timeout that should only fire if (e.g.) unable to connect/disconnect to BT headset for hfp profile
	g_assert_not_reached();
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);

	return FALSE;
}

static gboolean check_adapter_initialize(gpointer user_data)
{
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);

	return FALSE;
}

// Need to call after some time as give some time to disconnect/disable BT
static gboolean test_adapter_initialize(gpointer user_data)
{
	clear_source(&gTimeoutSource);

	g_assert_nonnull(defaultAdapter);

	auto iter = mProfiles.find(BLUETOOTH_PROFILE_ID_HFP);
	if (iter == mProfiles.end())
	{
		g_assert_not_reached();
		g_main_loop_quit(mainLoop);

		return FALSE;
	}

	hfpProfile = iter->second;
	if (!hfpProfile)
	{
		g_assert_not_reached();
		g_main_loop_quit(mainLoop);

		return FALSE;
	}

	observer = new TestAdapterObserver();
	defaultAdapter->registerObserver(observer);
	defaultAdapter->enable();

	gTimeoutSource = g_timeout_add(10000, check_adapter_initialize, (void *) NULL);

	return FALSE;
}

// Once paired and service UUIDS's are discovered, we turn off BT
// Before BT enable, add a timer so that BT gets properly switched off before we enable the same
static gboolean setup_test_hfp_initialize(gpointer user_data)
{
	clear_source(&gIdleSource);
	gTimeoutSource = g_timeout_add(10000, test_adapter_initialize, (void *) NULL);

	return FALSE;
}

static void test_hfp_initialize(void)
{
	gIdleSource = g_idle_add(setup_test_hfp_initialize, NULL);
	g_main_loop_run(mainLoop);
}

static gboolean setup_test_hfp_connect(gpointer user_data)
{
	DEBUG_MSG("");
	clear_source(&gIdleSource);

	g_assert_nonnull(defaultAdapter);

	hfpObserver = new TestHfpObserver();
	getImpl<BluetoothHfpProfile>()->registerObserver(hfpObserver);

    // Cancel device discovery before we start to connect
	defaultAdapter->cancelDiscovery([](BluetoothError error) {
		g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	});

	hfpProfile->connect(btPairingPartnerAddr,hfp_profile_connect_callback);

	// If not connected in 10 sec, testcase is failure
	gTimeoutSource = g_timeout_add(10000, check_hfp_state_timeout, (void *) NULL);

	return FALSE;
}

static void test_hfp_connect(void)
{
	gIdleSource = g_idle_add(setup_test_hfp_connect, NULL);
	g_main_loop_run(mainLoop);
}

static gboolean setup_test_hfp_disconnect(gpointer user_data)
{
	DEBUG_MSG("");
	clear_source(&gIdleSource);
	g_assert_nonnull(defaultAdapter);
	g_assert_equal(connected, TRUE);

	if (!hfpProfile)
	{
		g_assert_not_reached();
		g_main_loop_quit(mainLoop);

		return FALSE;
	}
	else
	{
		hfpProfile->disconnect(btPairingPartnerAddr, hfp_profile_disconnect_callback);

		// If callback not received in 5 sec, testcase is fail
		gTimeoutSource = g_timeout_add(5000, check_hfp_state_timeout, (void *) NULL);
	}

	return FALSE;
}

static void test_hfp_disconnect(void)
{
	gIdleSource = g_idle_add(setup_test_hfp_disconnect, NULL);
	g_main_loop_run(mainLoop);
}

static void test_hfp_deinitialize()
{
	getImpl<BluetoothHfpProfile>()->registerObserver(NULL);
	delete hfpObserver;

	defaultAdapter->registerObserver(NULL);
	delete observer;
	defaultAdapter->disable();
}

static void hfp_profile_opensco_callback(BluetoothError error)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);

	scoConnected = TRUE;
	// state has changed; hfp profile is now connected/disconnected
	clear_source(&gCheckSource);
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);
}

static gboolean check_hfp_scostate_timeout(gpointer user_data)
{
	// this is the overall timeout that should only fire if (e.g.) unable to open/close sco to BT headset
	g_assert_not_reached();
	clear_source(&gCheckSource);
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);

	return FALSE;
}

static gboolean check_hfp_openSco(gpointer user_data)
{
	DEBUG_MSG("");
	clear_source(&gCheckSource);

	if (!hfpProfile)
	{
		g_assert_not_reached();
		g_main_loop_quit(mainLoop);
		return FALSE;
	}
	else
	{
		getImpl<BluetoothHfpProfile>()->openSCO(btPairingPartnerAddr, hfp_profile_opensco_callback);

		// Timeout is 10 second if sco state is not changed
		gTimeoutSource = g_timeout_add(20000, check_hfp_scostate_timeout, (void *) NULL);
	}

	return FALSE;
}

static gboolean setup_test_open_sco(gpointer user_data)
{
	clear_source(&gIdleSource);
	g_assert_equal(connected, TRUE);

	// Add a delay as during connection AT commands will get exchanged
	gCheckSource = g_timeout_add(20000, check_hfp_openSco, NULL);

	return FALSE;
}

static void test_open_sco(void)
{
	gIdleSource = g_idle_add(setup_test_open_sco, NULL);
	g_main_loop_run(mainLoop);
}

static void hfp_profile_closesco_callback(BluetoothError error)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);

	scoConnected = FALSE;
	// state has changed; hfp profile is now connected/disconnected
	clear_source(&gCheckSource);
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);
}

static gboolean check_hfp_closeSco(gpointer user_data)
{
	DEBUG_MSG("");
	clear_source(&gCheckSource);

	if (!hfpProfile)
	{
		g_assert_not_reached();
		g_main_loop_quit(mainLoop);

		return FALSE;
	}
	else
	{
		getImpl<BluetoothHfpProfile>()->closeSCO(btPairingPartnerAddr, hfp_profile_closesco_callback);

		// Timeout is 5 second if sco state is not changed
		gTimeoutSource = g_timeout_add(5000, check_hfp_scostate_timeout, (void *) NULL);
	}

	return FALSE;
}

static gboolean setup_test_close_sco(gpointer user_data)
{
	clear_source(&gIdleSource);
	g_assert_equal(connected, TRUE);

	// Add delay as we try to close already opened SCO channel
	gCheckSource = g_timeout_add(5000, check_hfp_closeSco, NULL);

	return FALSE;
}

static void test_close_sco(void)
{
	gIdleSource = g_idle_add(setup_test_close_sco, NULL);
	g_main_loop_run(mainLoop);
}

static void add_tests()
{
	g_test_add_func("/SIL/Profile/Hfp/initialize", test_hfp_initialize);
	g_test_add_func("/SIL/Profile/Hfp/connect", test_hfp_connect);
	g_test_add_func("/SIL/Profile/Hfp/opensco", test_open_sco);
	g_test_add_func("/SIL/Profile/Hfp/closesco", test_close_sco);
	g_test_add_func("/SIL/Profile/Hfp/disconnect", test_hfp_disconnect);
	g_test_add_func("/SIL/Profile/Hfp/deinitialize", test_hfp_deinitialize);
}

REGISTER_PROFILE_TEST_MODULE("HFP", add_tests)
