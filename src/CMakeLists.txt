# Copyright (c) 2014-2018 LG Electronics, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0

set(TEST_SOURCES
    test_adapter.cpp
#   test_power_management.cpp	#commenting till bugs in bluez5 and mock sils are fixed
    test_device_discovery.cpp
    test_adapter_properties.cpp
    test_adapter_pairing.cpp
    test_profile_spp.cpp
    test_profile_hfp.cpp)

add_definitions(-DWEBOS_PROFILES_ENABLED="")

add_executable(bluetooth-sil-tester utils.cpp main.cpp test_registry.cpp ${TEST_SOURCES})
target_link_libraries(bluetooth-sil-tester ${GLIB2_LDFLAGS} dl)
webos_build_program(NAME bluetooth-sil-tester)
