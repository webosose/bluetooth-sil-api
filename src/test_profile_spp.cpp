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
#include "test_registry.h"

#include <bluetooth-sil-api.h>

using namespace std;

BluetoothSppChannelId channelId;


class TestSppObserver : public BluetoothSppStatusObserver
{
public:
	virtual void channelStateChanged(const std::string &address, const std::string &uuid, const BluetoothSppChannelId channelId, const bool state) {}
};

static TestSppObserver *sppProfileObserver;
static TestAdapterObserver *observer;

static guint gSource = 0;
static guint gIdleSource = 0;
static guint gTimeoutSource = 0;
BluetoothSppProfile *sppProfile;

static gboolean check_spp_timeout(gpointer user_data)
{
	g_assert_not_reached();
	clear_source(&gSource);
	clear_source(&gIdleSource);
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);

	return FALSE;
}

static gboolean check_adapter_initialized(gpointer user_data)
{
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);

	return FALSE;
}

static gboolean check_spp_connect(gpointer user_data)
{
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);

	return FALSE;
}

static gboolean spp_profile_connect_callback(BluetoothError error, uint16_t uipc_channelId)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	channelId = uint64_t(uipc_channelId);

	return FALSE;
}

static gboolean spp_profile_disconnect_callback(BluetoothError error)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);

	return FALSE;
}

static gboolean spp_profile_channelState_callback(BluetoothError error, const bool state)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	DEBUG_MSG("The Channel State has been changed "<< state);
	clear_source(&gTimeoutSource);
	g_main_loop_quit(mainLoop);

	return FALSE;
}

static gboolean spp_write_callback(BluetoothError error)
{
	g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	g_main_loop_quit(mainLoop);

	return FALSE;
}

static gboolean setup_sppGetChannelState(gpointer user_data)
{
	sppProfile->getChannelState(btPairingPartnerAddr, "10001101-0000-1000-8000-00805f9b34fb", spp_profile_channelState_callback);
	gTimeoutSource = g_timeout_add(20000, check_spp_timeout, (void *)NULL);

	return FALSE;
}

static gboolean initialize_spp(gpointer user_data)
{
	clear_source(&gTimeoutSource);
	g_assert_nonnull(defaultAdapter);

	auto iter = mProfiles.find(BLUETOOTH_PROFILE_ID_SPP);
	if (iter == mProfiles.end())
	{
		g_assert_not_reached();
		g_main_loop_quit(mainLoop);

		return FALSE;
	}

	sppProfile = dynamic_cast<BluetoothSppProfile*>(iter->second);
	if (!sppProfile)
	{
		g_assert_not_reached();
		g_main_loop_quit(mainLoop);

		return FALSE;
	}

	observer = new TestAdapterObserver();
	defaultAdapter->registerObserver(observer);

	defaultAdapter->enable();

	gTimeoutSource = g_timeout_add(10000, check_adapter_initialized, (void *)NULL);

	return FALSE;
}

static gboolean setup_test_spp_initialize(gpointer user_data)
{
	clear_source(&gIdleSource);
	gTimeoutSource = g_timeout_add(15000, initialize_spp, (void *)NULL);

	return FALSE;
}

static void test_sppInitialize(void)
{
	gIdleSource = g_idle_add(setup_test_spp_initialize, NULL);
	g_main_loop_run(mainLoop);
}

static gboolean setup_sppCreateChannel(gpointer user_data)
{
	BluetoothError error = sppProfile->createChannel("SPP_CHANNEL", "10001101-0000-1000-8000-00805f9b34fb");

	g_assert_equal(error, BLUETOOTH_ERROR_NONE);
	g_main_loop_quit(mainLoop);

	return FALSE;
}

static gboolean setup_sppConnectUuid(gpointer user_data)
{
	sppProfileObserver = new TestSppObserver();
	g_assert_nonnull(sppProfileObserver);

	sppProfile->registerObserver(sppProfileObserver);

	sppProfile->connectUuid(btPairingPartnerAddr, "10001101-0000-1000-8000-00805f9b34fb", spp_profile_connect_callback);

	// Added timeout so that user can enter data for read SPP operation
	gTimeoutSource = g_timeout_add(25000, check_spp_connect, (void *)NULL);

	return FALSE;
}

static gboolean setup_sppRemovalUuid(gpointer user_data)
{
	sppProfile->removeChannel("10001101-0000-1000-8000-00805f9b34fb");
	g_main_loop_quit(mainLoop);

	return FALSE;
}

static gboolean setup_sppDisconnectUuid(gpointer user_data)
{
	sppProfile->disconnectUuid(channelId, spp_profile_disconnect_callback);
	gTimeoutSource = g_timeout_add(10000, check_spp_timeout, (void *)NULL);

	return FALSE;
}

static gboolean setup_sppWriteData(gpointer user_data)
{

	unsigned char recievedData[] = {10, 110, 0, 5, 93, 4, 100, 30};

	sppProfile->writeData(channelId, (const uint8_t *) recievedData, sizeof(recievedData), spp_write_callback);
	gTimeoutSource = g_timeout_add(10000, check_spp_timeout, (void *)NULL);

	return FALSE;
}

static void test_sppConnectUuid(void)
{
	gIdleSource = g_idle_add(setup_sppConnectUuid, NULL);
	g_main_loop_run(mainLoop);
}

static void test_sppGetChannelState(void)
{
	gIdleSource = g_idle_add(setup_sppGetChannelState, NULL);
	g_main_loop_run(mainLoop);
}

static void test_sppDisconnectUuid(void)
{
	gIdleSource = g_idle_add(setup_sppDisconnectUuid, NULL);
	g_main_loop_run(mainLoop);
}

static void test_sppWriteData(void)
{
	gIdleSource = g_idle_add(setup_sppWriteData, NULL);
	g_main_loop_run(mainLoop);
}

static void test_sppCreateChannelUuid(void)
{
	gIdleSource = g_idle_add(setup_sppCreateChannel, NULL);
	g_main_loop_run(mainLoop);
}

static void test_sppRemovalUuid(void)
{
	gIdleSource = g_idle_add(setup_sppRemovalUuid, NULL);
	g_main_loop_run(mainLoop);
}

static void test_sppDeinitialize(void)
{
	sppProfile->registerObserver(NULL);
	delete sppProfileObserver;

	defaultAdapter->registerObserver(NULL);
	delete observer;

	defaultAdapter->disable();
}
static void add_tests()
{
	g_test_add_func("/SIL/SPP/SPPInitialize", test_sppInitialize);
	g_test_add_func("/SIL/SPP/ConnectUUID", test_sppConnectUuid);
	g_test_add_func("/SIL/SPP/GetChannelState", test_sppGetChannelState);
	g_test_add_func("/SIL/SPP/WriteData", test_sppWriteData);
	g_test_add_func("/SIL/SPP/Disconnect", test_sppDisconnectUuid);
	g_test_add_func("/SIL/SPP/CreateChannelUUID", test_sppCreateChannelUuid);
	g_test_add_func("/SIL/SPP/RemovalUUID", test_sppRemovalUuid);
	g_test_add_func("/SIL/SPP/SPPDeinitialize", test_sppDeinitialize);
}

REGISTER_PROFILE_TEST_MODULE("SPP", add_tests)
