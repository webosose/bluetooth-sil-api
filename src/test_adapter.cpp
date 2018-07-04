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
#include "sil-tester.h"
#include "utils.h"

using namespace std;

static guint gIdleSource = 0;
static vector<BluetoothAdapter *> adapters;

static gboolean setup_test_get_adapters(gpointer user_data)
{
	clear_source(&gIdleSource);
	adapters = sil->getAdapters();

	g_assert_notequal(adapters.size(),0);
	for (auto adapter : adapters )
	{
		g_assert_nonnull(adapter);
	}
	g_main_loop_quit(mainLoop);

	return FALSE; // prevents this being called again
}

void test_get_adapters(void)
{
	gIdleSource = g_idle_add(setup_test_get_adapters, NULL);
	g_main_loop_run(mainLoop);
}

static void add_tests()
{
	g_test_add_func("/SIL/Adapter/getAdapters", test_get_adapters);
}

REGISTER_TEST_MODULE(add_tests)

