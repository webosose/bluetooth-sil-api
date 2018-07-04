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

#include <vector>
#include <sstream>

#include "sil-tester.h"

using namespace std;

// The registry of test module initialization functions.
typedef vector<voidfn> registry_t;
static registry_t test_registry;

// Vector of profiles to be tested
static std::vector<std::string> mEnabledProfiles;

bool isProfileEnabled(const std::string &profile)
{
	for (auto currentProfile : mEnabledProfiles)
	{
		if (currentProfile == profile)
			return true;
	}

	return false;
}

//
// Register a module initialization function
//
// This method is invoked via the REGISTER_TEST_MODULE macro, which
// ensures the list of test modules is populated before entering main().
//
// init_func:
//   A voidfn function pointer. The provided method is called once
//   the g_test framwork has been initialized, and should primarily
//   add tests to the framework, though it can also perfrom some
//   one-off module initialization.
//
void register_test_module(voidfn init_func)
{
	test_registry.push_back(init_func);
}

//
// Registers bluetooth profiles
//
// This method is invoked via the SET_PROFILE macro, which
// ensures the list of bluetooth profiles is available before entering main().
//
//Should only be called ONCE, once pairing is done, start profile testing.

void set_profile_module()
{
	std::stringstream ss(std::string(WEBOS_PROFILES_ENABLED));
	std::string item;
	while (std::getline(ss, item, ' '))
	{
		mEnabledProfiles.push_back(item);
	}
}

//
// Register a profile initialization function
//
// This method is invoked via the REGISTER_PROFILE_TEST_MODULE macro, which
// ensures the list of profiles is populated before we start testing
//
// init_func:
//   A voidfn function pointer. The provided method is called once
//   the g_test framwork has been initialized, based on the profile
//   name from WEBOS_PROFILES_ENABLED flag, these methods will be
//   added and will add tests to the framework
//
// profileId
//   Profile name: associated with init_func with which testing is required
//
void register_profile_test_module(std::string profileId, voidfn init_func)
{
	for (auto currentServiceClass : mEnabledProfiles)
	{
		if (currentServiceClass == profileId)
			test_registry.push_back(init_func);
	}
}

//
// Invoke each registered initialization method (for the test modules).
//
// Should only be called ONCE, and then only after g_test_init() has been called.
//
void register_all_tests()
{
	for (auto iter = test_registry.begin(); iter != test_registry.end(); ++iter)
	{
		// Cast the iterator to a function pointer, and invoke the function.
		voidfn func = *iter;

		if (func)
		{
			func();
		}
	}
}

