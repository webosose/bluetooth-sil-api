bluetooth-sil-api
=================

Summary
-------
webOS bluetooth SIL API definition.

Description
-----------
This component contains the API definition a bluetooth SIL implementation
has to implement in order to be usable by the webos-bluetooth-service.

Additionally, the component can build and install a test harness which
exercises a SIL to check basic functionality.

How to Build on Linux
=====================

## Dependencies

Below are the tools and libraries (and their minimum versions) required to build
webos-bluetooth-service:

* cmake (version required by openwebos/cmake-modules-webos)
* gcc 4.7.2
* glib-2.0 2.34.3
* make (any version)
* openwebos/cmake-modueles-webos 1.3.1
* pkg-config 0.26

## Building

Once you have downloaded the source, enter the following to build it (after
changing into the directory under which it was downloaded):

    $ mkdir BUILD
    $ cd BUILD
    $ cmake ..
    $ make
    $ sudo make install

The directory under which the files are installed defaults to `/usr/local/webos`.
You can install them elsewhere by supplying a value for `WEBOS_INSTALL_ROOT`
when invoking `cmake`. For example:

    $ cmake -D WEBOS_INSTALL_ROOT:PATH=$HOME/projects/webosose ..
    $ make
    $ make install

will install the files in subdirectories of `$HOME/projects/webosose`.

Specifying `WEBOS_INSTALL_ROOT` also causes `pkg-config` to look in that tree
first before searching the standard locations. You can specify additional
directories to be searched prior to this one by setting the `PKG_CONFIG_PATH`
environment variable.

If not specified, `WEBOS_INSTALL_ROOT` defaults to `/usr/local/webos`.

To configure for a debug build, enter:

    $ cmake -D CMAKE_BUILD_TYPE:STRING=Debug ..

To see a list of the make targets that `cmake` has generated, enter:

    $ make help

## Uninstalling

From the directory where you originally ran `make install`, enter:

 $ [sudo] make uninstall

You will need to use `sudo` if you did not specify `WEBOS_INSTALL_ROOT`.

## The Test Harness

To build and install the test harness, specify `WEBOS_CONFIG_BUILD_TESTS` on
the CMake command line thus:

    $ cmake -D WEBOS_CONFIG_BUILD_TESTS=TRUE <other arguments>
    $ make install

This will build and  install an executable `bluetooth-sil-tester` which can
be invoked as:

    $ bluetooth-sil-tester /path/to/sil.so <btCapability> <btPairingPartnerAddr>

The tester is based on GLib's g_test framework, and thus accepts all of
the usual `gtester` command line options. Type `bluetooth-sil-tester -h`
for help.

The <btCapability> tells the sil what BluetoothPairingIOCapability to use.
The <btPairingPartnerAddr> tells a device SIL what Bluetooth MAC address
to use in the outbound pairing test.

To see a list of available tests, use "-l" e.g.:

	$ bluetooth-sil-tester <sil.so> -l

To run a specific test or set of tests, specify a test path, e.g.:

	$ bluetooth-sil-tester <sil.so> -p /SIL/Adapter/Discovery

It is recommended to build the sil-tester in debug mode (see above) to
enable debug output during the tests, or if building in OE for device,
enable the "#if 1" line at the top of "sil-tester.h".

NOTE: Tests should all work with the mock SIL, but there may be problems
running various tests (or especially multiple tests together) on a real
device.  Callbacks and glib timeouts from a previous test may fire in a
subsequent test and cause strange results or test failures.  This could
be fixed, but it is actually recommended to use the webos-bluetooth-service
for SIL testing on a real device.

# Copyright and License Information

Unless otherwise specified, all content, including all source code files and
documentation files in this repository are:

Copyright (c) 2014-2018 LG Electronics, Inc.


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at


http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

SPDX-License-Identifier: Apache-2.0
