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

//
// Just about every source module in the SIL Test application needs to use
// a core set of includes, macros, and the SIL itself. So, they are bundled
// in a single common include file.
//
#ifndef SIL_TESTER_H_
#define SIL_TESTER_H_

#include <glib.h>               // Primarily for the test framework
#include "bluetooth-sil-api.h"  // The API on the SIL itself
#include "test_registry.h"      // Methods and macros for defining test modules

//
// Debug (logging) macros to assist in debugging sil-tester
//
//#if 1		// to easily enable debug output in OE build
#if WEBOS_DEBUG
#include <iostream>
#define DEBUG_MSG(str) do { std::cerr << "  " << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

//
// The newest version of glib provide some useful additional assert_* macros,
// but they may not be available in the build. Thus each one is conditionally
// defined here using the raw g_assert if necessary, so they may be safely used
// in tests.
//
// The names are self-explanatory.
//
#ifndef g_assert_true
#define g_assert_true(X) g_assert((X))
#endif

#ifndef g_assert_false
#define g_assert_false(X) g_assert(!(X))
#endif

#ifndef g_assert_nonnull
#define g_assert_nonnull(X) g_assert((X) != NULL)
#endif

#ifndef g_assert_null
#define g_assert_null(X) g_assert((X) == NULL)
#endif

#ifndef g_assert_equal
#define g_assert_equal(X,Y) g_assert((X) == (Y))
#endif

#ifndef g_assert_notequal
#define g_assert_notequal(X,Y) g_assert((X) != (Y))
#endif

//
// The open SIL instance. SILs are not required to support either multiple
// instances, or repeated open/close cycles. Therefore, the SIL is loaded,
// opened, and closed in main() and the instance shared via a (yuck) global
// variable.
//
extern BluetoothSIL *sil;

// SIL name (e.g. "mock") used in sil-tester to select which pairing tests to run
extern std::string silName;
extern bool testingMockSil;
extern BluetoothPairingIOCapability btPairingIOCapability;
extern std::string btPairingPartnerAddr;
extern std::map<std::string, BluetoothProfile*> mProfiles;

extern GMainLoop *mainLoop;
extern BluetoothAdapter *defaultAdapter;

//
// This Observer class simply records whether or not a callback was
// invoked, and the last value passed to it.
//
class TestAdapterObserver: public BluetoothAdapterStatusObserver
{
public:
	bool adapterStateChangedFlag;
	bool adapterStateValue;
	bool adapterPropertiesChangedFlag;
	bool devicePropertiesChangedFlag;
	std::string lastDevicePropertyAddress;
	std::string lastDeviceRemovedAddress;
	bool deviceRemovedFlag;
	bool discoveryStateChangedFlag;
	bool discoveryStateValue;
	BluetoothPropertiesList lastAdapterPropertySet;
	BluetoothPropertiesList lastDevicePropertySet;
	std::string pairingDeviceAddress;
	std::string pairingConfirmationPin;
	BluetoothPasskey pairingConfirmationPasskey;
	bool pairingConfirmationRequested;
	bool pairingDisplaySecretRequested;
	bool pairingSecretRequested;
	BluetoothPairingSecretType pairingSecretType;

	TestAdapterObserver() :
		adapterStateChangedFlag(false),
		adapterStateValue(false),
		adapterPropertiesChangedFlag(false),
		devicePropertiesChangedFlag(false),
		lastDevicePropertyAddress(""),
		lastDeviceRemovedAddress(""),
		deviceRemovedFlag(false),
		discoveryStateChangedFlag(false),
		discoveryStateValue(false),
		pairingDeviceAddress(""),
		pairingConfirmationPin(""),
		pairingConfirmationPasskey(0),
		pairingConfirmationRequested(false),
		pairingDisplaySecretRequested(false),
		pairingSecretRequested(false)
	{
	}

	~TestAdapterObserver()
	{
		lastDevicePropertyAddress.clear();
		lastDeviceRemovedAddress.clear();
		lastAdapterPropertySet.clear();
		lastDevicePropertySet.clear();
		pairingDeviceAddress.clear();
		pairingConfirmationPin.clear();
	}

	// Adapter State/Properties Callbacks

	void adapterStateChanged(bool powered)
	{
		DEBUG_MSG("*Callback TestAdapterObserver::adapterStateChanged");
		adapterStateValue = powered;
		adapterStateChangedFlag = true;
	}
	void adapterPropertiesChanged(BluetoothPropertiesList properties)
	{
		DEBUG_MSG("*Callback TestAdapterObserver::adapterPropertiesChanged");
		lastAdapterPropertySet = properties;
		adapterPropertiesChangedFlag = true;
	}

	// Device Discovery/Properties Callbacks

	void deviceFound(BluetoothPropertiesList properties)
	{
		DEBUG_MSG("*Callback TestAdapterObserver::deviceFound");
		lastDevicePropertySet = properties;
		// As on device search, multiple devices can be found, set the flag only
		// when BD_ADDR supplied in sil tester is found
		for (auto prop : properties)
		{
			g_assert_notequal(prop.getType(), (BluetoothProperty::Type::EMPTY));

			if (BluetoothProperty::Type::BDADDR == prop.getType())
			{
				auto address = prop.getValue<std::string>();
				devicePropertiesChangedFlag = (address == btPairingPartnerAddr);
				break;
			}
		}
	}

	void deviceRemoved(const std::string &address)
	{
		DEBUG_MSG("*Callback TestAdapterObserver::deviceRemoved");
		lastDeviceRemovedAddress = address;
		deviceRemovedFlag = true;
	}
	void devicePropertiesChanged(const std::string &address, BluetoothPropertiesList properties)
	{
		// It may happen that properties are changed for some other device, hence
		// check first BD_ADDR, it different continue looking for properties change
		if (address != btPairingPartnerAddr)
			return;

		DEBUG_MSG("*Callback TestAdapterObserver::devicePropertiesChanged");
		lastDevicePropertyAddress = address;
		lastDevicePropertySet = properties;
		devicePropertiesChangedFlag = true;
	}
	void discoveryStateChanged(bool active)
	{
		DEBUG_MSG("*Callback TestAdapterObserver::discoveryStateChanged");
		discoveryStateValue = active;
		discoveryStateChangedFlag = true;
	}

	// Pairing Callbacks

	void displayPairingConfirmation(const std::string &address, BluetoothPasskey passkey)
	{
		// respond to this (in check_pairing_state_change) by calling adapter->supplyPairingConfirmation
		DEBUG_MSG("*Callback TestAdapterObserver::displayPairingConfirmation");
		DEBUG_MSG("*** address/passkey: " << address << "/" << passkey);
		pairingDeviceAddress = address;
		pairingConfirmationPasskey = passkey;
		pairingConfirmationRequested = true;
	}
	void displayPairingSecret(const std::string &address, const std::string &pin)
	{
		// handle this (in check_pairing_state_change) by simply "displaying" the secret (pin)
		DEBUG_MSG("*Callback TestAdapterObserver::displayPairingSecret");
		DEBUG_MSG("*** address/pin: " << address << "/" << pin);
		pairingDeviceAddress = address;
		pairingSecretType = BLUETOOTH_PAIRING_SECRET_TYPE_PIN;
		pairingConfirmationPin = pin;
		pairingDisplaySecretRequested = true;
	}
	void displayPairingSecret(const std::string &address, BluetoothPasskey passkey)
	{
		// handle this (in check_pairing_state_change) by simply "displaying" the secret (passkey)
		DEBUG_MSG("*Callback TestAdapterObserver::displayPairingSecret");
		DEBUG_MSG("*** address/passkey: " << address << "/" << passkey);
		pairingDeviceAddress = address;
		pairingSecretType = BLUETOOTH_PAIRING_SECRET_TYPE_PASSKEY;
		pairingConfirmationPasskey = passkey;
		pairingDisplaySecretRequested = true;
	}
	void requestPairingSecret(const std::string &address, BluetoothPairingSecretType type)
	{
		// respond to this (in check_pairing_state_change) by calling adapter->supplyPairingSecret
		DEBUG_MSG("*Callback TestAdapterObserver::requestPairingSecret");
		DEBUG_MSG("*** address/pairingType: " << address << "/" << type);
		pairingDeviceAddress = address;
		pairingSecretType = type;
		pairingSecretRequested = true;
	}
	void pairingCanceled()
	{
		// TODO: Should consider adding tests for (testing) cancelling during pairing...
		DEBUG_MSG("*Callback TestAdapterObserver::pairingCanceled (NOT HANDLED)");
	}
};

//
// A simple SIL Observer implementation which records a call to the
// 'adaptersChanged' callback
//
class TestSilObserver: public BluetoothSILStatusObserver
{
public:
	bool adapterAvailable;

	TestSilObserver() :
		adapterAvailable(false)
	{
	}

	void adaptersChanged()
	{
		DEBUG_MSG("*Callback TestSilObserver::adaptersChanged");
		adapterAvailable = true;
	}
};

#endif // SIL_TESTER_H_
