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

#ifndef TEST_REGISTRY_H_
#define TEST_REGISTRY_H_

//
// Simple typedef for a function with the prototype 'void foo()'
//
typedef void (*voidfn)();

//
// Add all registered test modules to the test framework (called by main)
//
extern void register_all_tests();

//
// Return true if profile is registered
//
extern bool isProfileEnabled(const std::string &profile);

//
// Every source module which defines tests must include an invokation
// of this macro. It declares a function, with the GCC constructor
// attribute, to add the module to the list called by main().
//
// The single parameter is the name of a void function which will add
// each test to the g_test test framework (via g_test_add).
//
// For example:
//
//   #include <test_registry.h>
//
//   void alwaysPass()
//   {
//       g_assert(1);
//   }
//
//   // Define other tests
//
//   void add_tests()
//   {
//       g_test_add_func("/path/to/test", alwaysPass);
//       // Add other tests
//   }
//
// REGISTER_TEST_MODULE(add_tests)

#define REGISTER_TEST_MODULE(reg_func) \
    __attribute__ ((constructor)) \
    static void SIL_TEST_MODULE_##reg_func##__() \
    { \
        extern void register_test_module(voidfn init_func); \
        register_test_module(reg_func); \
    }

//
// Invoking this macro is required to set bluetooth
// profiles to test.Once set, profiles will be registered
// with SIL layer and instance will be available with
// sil-tester.This macro should be called before
// REGISTER_PROFILE_TEST_MODULE which adds profiles
// testcases to g_test test framework
//
// For example:
// In src/CMakeLists.txt, add WEBOS_PROFILES_ENABLED="A2DP"
//
//   #include <test_registry.h>
//
//   void alwaysPass()
//   {
//       g_assert(1);
//   }
//
//   // Define other tests
//
//   void add_tests()
//   {
//       g_test_add_func("/a2dp/path/to/test", alwaysPass);
//       // Add other tests
//   }
//
// SET_PROFILE()
// REGISTER_PROFILE_TEST_MODULE("A2DP", add_tests)

#define SET_PROFILE() \
    __attribute__ ((constructor)) \
    static void SET_PROFILE__() \
    { \
        extern void set_profile_module(); \
        set_profile_module(); \
    }

//
// Every profile source module which defines tests must include an invokation
// of this macro. It declares a function, with the GCC constructor
// attribute, to add the module to the list called by main().
//
// First parameter is the bluetooth profile for which testcase
// needs to be added
//
// The second parameter is the name of a void function which will add
// each test to the g_test test framework (via g_test_add) if profile
// is enabled
//
// For example:
//
//   #include <test_registry.h>
//
//   void alwaysPass()
//   {
//       g_assert(1);
//   }
//
//   // Define other tests
//
//   void add_tests()
//   {
//       g_test_add_func("/path/to/test", alwaysPass);
//       // Add other tests
//   }
//
// REGISTER_PROFILE_TEST_MODULE("HFP", add_tests)

#define REGISTER_PROFILE_TEST_MODULE(profileId, reg_func) \
    __attribute__ ((constructor)) \
    static void SIL_PROFILE_TEST_MODULE_##reg_func##__() \
    { \
        extern void register_profile_test_module(std::string profile, voidfn init_func); \
        register_profile_test_module(profileId, reg_func); \
    }

#endif /* TEST_REGISTRY_H_ */

