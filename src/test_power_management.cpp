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

#include <unistd.h>
#include "sil-tester.h"
#include "utils.h"

using namespace std;

static TestAdapterObserver *observer;
static guint gSource = 0;
static guint gIdleSource = 0;

static gboolean check_adapter_change(gpointer user_data)
{
	bool powered = (bool *)user_data;
	g_assert_nonnull(observer);
        g_assert_true(observer->adapterStateChangedFlag);
        g_assert_equal(observer->adapterStateValue, powered);
	clear_source(&gSource);
	g_main_loop_quit(mainLoop);
	return FALSE;
}

static gboolean setup_test_enable_adapter(gpointer user_data)
{
	clear_source(&gIdleSource);
	//TODO: Move below 3 lines to setUp()
	g_assert_nonnull(defaultAdapter);
	observer = new TestAdapterObserver();
	defaultAdapter->registerObserver(observer);

	g_assert_equal(defaultAdapter->enable(), BLUETOOTH_ERROR_NONE);
	gSource = g_timeout_add(1000, check_adapter_change, (bool *)true);

	return FALSE;
}

static void test_enable_adapter(void)
{
	gIdleSource = g_idle_add(setup_test_enable_adapter, NULL);
	g_main_loop_run(mainLoop);
	//TODO: Move this line to tearDown()
	delete observer;
}

static gboolean setup_test_disable_adapter(gpointer user_data)
{
	clear_source(&gIdleSource);
	//TODO: Move below 3 lines to setUp()
	g_assert_nonnull(defaultAdapter);
	observer = new TestAdapterObserver();
	defaultAdapter->registerObserver(observer);

	g_assert_equal(defaultAdapter->disable(), BLUETOOTH_ERROR_NONE);
	gSource = g_timeout_add(1000, check_adapter_change, (bool *)false);
	//power on the adapter back before leaving the test
	g_assert_equal(defaultAdapter->enable(), BLUETOOTH_ERROR_NONE);

	return FALSE;
}

static void test_disable_adapter(void)
{
	gIdleSource = g_idle_add(setup_test_disable_adapter, NULL);
	g_main_loop_run(mainLoop);
	//TODO: Move this line to tearDown()
	delete observer;
}

static void add_tests()
{
	g_test_add_func("/SIL/Adapter/Enable/adapterEnableTest", test_enable_adapter);
	g_test_add_func("/SIL/Adapter/Enable/adapterDisableTest", test_disable_adapter);
}


REGISTER_TEST_MODULE(add_tests)
