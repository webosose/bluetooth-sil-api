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

#include <glib.h>

#include <iostream>
#include <typeinfo>

#include "bluetooth-sil-api.h"

class MockObserver : public BluetoothGattProfileStatusObserver
{
public:
	MockObserver() :
	    serviceFoundCalled(false),
	    serviceLostCalled(false),
	    characteristicFoundCalled(false),
	    characteristicValueChangedCalled(false)
	{
	}

	void serviceFound(const std::string &address, const BluetoothGattService &service) { serviceFoundCalled = true; }
	void serviceLost(const std::string &address, const BluetoothGattService &service) { serviceLostCalled = true; }
	void characteristicFound(const std::string &address, const BluetoothGattCharacteristic &service) { characteristicFoundCalled = true; }
	void characteristicValueChanged(const BluetoothGattCharacteristic &characteristic) { characteristicValueChangedCalled = true; }

	void reset()
	{
		serviceFoundCalled = false;
		serviceLostCalled = false;
		characteristicFoundCalled = false;
		characteristicValueChangedCalled = false;
	}

	bool serviceFoundCalled;
	bool serviceLostCalled;
	bool characteristicFoundCalled;
	bool characteristicValueChangedCalled;
};

class TestGattProfile : public BluetoothProfile,
                        public BluetoothGattProfile
{
public:
	TestGattProfile(MockObserver *observer = 0)
	{
		// If no observer is registered a null object is returned.
		g_assert(getGattObserver() != 0);

		BluetoothGattProfile::registerObserver(observer);
	}
	~TestGattProfile() { }

	void getProperties(const std::string &address, BluetoothPropertiesResultCallback  callback) { }
	void getProperty(const std::string &address, BluetoothProperty::Type type,
	                         BluetoothPropertyResultCallback callback) { }
	void connect(const std::string &address, BluetoothResultCallback callback) { }
	void disconnect(const std::string &address, BluetoothResultCallback callback) { }

	void discoverServices(BluetoothResultCallback callback) { getGattObserver()->serviceFound("", BluetoothGattService());}
	void discoverServices(const std::string &address, BluetoothResultCallback callback) { }
	// Obsolete
	void addService(const BluetoothGattService &service, BluetoothResultCallback callback) { }
	// Obsolete
	void removeService(const BluetoothUuid &uuid, BluetoothResultCallback callback) { }
	void changeCharacteristicWatchStatus(const std::string &address, const BluetoothUuid &service, const BluetoothUuid &characteristic, bool enabled, BluetoothResultCallback callback) { }
	void readCharacteristics(const std::string &address, const BluetoothUuid &service, const BluetoothUuidList &characteristics, BluetoothGattReadCharacteristicsCallback callback) { }
	void writeCharacteristic(const std::string &address, const BluetoothUuid &service, const BluetoothGattCharacteristic &characteristic, BluetoothResultCallback callback) { }

	// Obsolete
	void readCharacteristics(const BluetoothUuid &service, const BluetoothUuidList &characteristics, BluetoothGattReadCharacteristicsCallback callback) { }
	// Obsolete
	void writeCharacteristic(const BluetoothUuid &service, const BluetoothGattCharacteristic &characteristic, BluetoothResultCallback callback) { }
	BluetoothGattService getService(const std::string &address, const BluetoothUuid &uuid) { return BluetoothGattService(); }
	BluetoothGattServiceList getServices(const std::string &address) { return BluetoothGattServiceList(); }

	// Obsolete
	void readDescriptors(const BluetoothUuid& service, const BluetoothUuid &characteristic, const BluetoothUuidList &descriptors,
	                            BluetoothGattReadDescriptorsCallback callback) { }
	void readDescriptors(const std::string &address, const BluetoothUuid& service, const BluetoothUuid &characteristic, const BluetoothUuidList &descriptors,
	                            BluetoothGattReadDescriptorsCallback callback) { }
	// Obsolete
	void writeDescriptor(const BluetoothUuid &service, const BluetoothUuid &characteristic,
	                             const BluetoothGattDescriptor &descriptor, BluetoothResultCallback callback) { }
	void writeDescriptor(const std::string &address, const BluetoothUuid &service, const BluetoothUuid &characteristic,
	                             const BluetoothGattDescriptor &descriptor, BluetoothResultCallback callback) { }
};

static void test_gatt_observer(void)
{
	MockObserver observer;
	TestGattProfile testProfile1(&observer);
	TestGattProfile testProfile2;

	// If the profile wouldn't use a null object for the GATT observer internally this
	// call would crash everything with a SEGV
	testProfile2.discoverServices([](BluetoothError error) { });

	testProfile1.discoverServices([](BluetoothError error) { });
	g_assert(observer.serviceFoundCalled);

	observer.reset();

	testProfile1.BluetoothGattProfile::registerObserver(&observer);

	testProfile1.discoverServices([](BluetoothError error) { });
	g_assert(observer.serviceFoundCalled);

	observer.reset();
}

static void test_descriptor_properties(void)
{
	BluetoothGattDescriptor descriptor;

	descriptor.setUuid(BluetoothUuid("1b5a"));
	g_assert(descriptor.getUuid().toString() == std::string("1b5a"));

	BluetoothGattValue value = { 0x1a, 0x2b, 0xff };
	descriptor.setValue(value);
	g_assert(descriptor.getValue() == BluetoothGattValue({ 0x1a, 0x2b, 0xff }));

	BluetoothGattDescriptor other;

	g_assert(other.getUuid().toString() == std::string(""));
	g_assert(other.getValue() == BluetoothGattValue());

	other = descriptor;
	g_assert(other.getUuid().toString() == std::string("1b5a"));
	g_assert(other.getValue() == BluetoothGattValue({ 0x1a, 0x2b, 0xff }));
}

static void test_characteristic_properties(void)
{
	BluetoothGattCharacteristic characteristic;

	characteristic.setPermissions(BluetoothGattPermission::PERMISSION_READ | BluetoothGattPermission::PERMISSION_WRITE);
	g_assert(characteristic.getPermissions() == (BluetoothGattPermission::PERMISSION_READ | BluetoothGattPermission::PERMISSION_WRITE));
	g_assert(characteristic.isPermissionSet(BluetoothGattPermission::PERMISSION_READ));
	g_assert(!characteristic.isPermissionSet(BluetoothGattPermission::PERMISSION_WRITE_ENCRYPTED));

	characteristic.setProperties(BluetoothGattCharacteristic::PROPERTY_AUTHENTICATED_SIGNED_WRITES | BluetoothGattCharacteristic::PROPERTY_NOTIFY);
	g_assert(characteristic.getProperties() == (BluetoothGattCharacteristic::PROPERTY_AUTHENTICATED_SIGNED_WRITES | BluetoothGattCharacteristic::PROPERTY_NOTIFY));
	g_assert(characteristic.isPropertySet(BluetoothGattCharacteristic::PROPERTY_AUTHENTICATED_SIGNED_WRITES));
	g_assert(!characteristic.isPropertySet(BluetoothGattCharacteristic::PROPERTY_WRITE));

	characteristic.setProperties(BluetoothGattCharacteristic::PROPERTY_WRITE);
	g_assert(characteristic.getProperties() == BluetoothGattCharacteristic::PROPERTY_WRITE);

	characteristic.setUuid(BluetoothUuid("6b504fa0-c71f-11e4-8731-1681e6b88ec1"));
	g_assert(characteristic.getUuid() == BluetoothUuid("6b504fa0-c71f-11e4-8731-1681e6b88ec1"));

	BluetoothGattCharacteristic other;

	g_assert(other.getPermissions() == 0);
	g_assert(other.getProperties() == 0);
	g_assert(other.getUuid() == "");

	other = characteristic;

	g_assert(other.getPermissions() == (BluetoothGattPermission::PERMISSION_READ | BluetoothGattPermission::PERMISSION_WRITE));
	g_assert(other.getProperties() == BluetoothGattCharacteristic::PROPERTY_WRITE);
	g_assert(other.getUuid() == BluetoothUuid("6b504fa0-c71f-11e4-8731-1681e6b88ec1"));

	BluetoothGattDescriptor d0;

	d0.setUuid(BluetoothUuid("852c02ec-c720-11e4-8731-1681e6b88ec1"));
	d0.setValue(BluetoothGattValue({0x2a, 0xff}));
}

static void test_update_characteristic_value(void)
{
	BluetoothGattService service;
	BluetoothGattCharacteristic characteristic;
	BluetoothGattValue value = { 0x11, 0x22, 0x33 };

	characteristic.setUuid(BluetoothUuid("1234"));
	characteristic.setValue(value);
	service.addCharacteristic(characteristic);

	auto characteristics = service.getCharacteristics();
	g_assert(characteristics.at(0).getValue() == BluetoothGattValue({0x11, 0x22, 0x33}));

	value = { 0x44, 0x55, 0x66 };
	service.updateCharacteristicValue(BluetoothUuid("1234"), value);

	characteristics = service.getCharacteristics();
	g_assert(characteristics.at(0).getValue() == BluetoothGattValue({0x44, 0x55, 0x66}));
}

static void test_update_descriptor_value(void)
{
	BluetoothGattCharacteristic characteristic;
	BluetoothGattDescriptor descriptor;
	BluetoothGattValue value = { 0x11, 0x22, 0x33 };

	descriptor.setUuid(BluetoothUuid("1234"));
	descriptor.setValue(value);

	characteristic.setUuid(BluetoothUuid("5678"));
	characteristic.addDescriptor(descriptor);

	auto descriptors = characteristic.getDescriptors();
	g_assert(descriptors.at(0).getValue() == BluetoothGattValue({0x11, 0x22, 0x33}));

	value = { 0x44, 0x55, 0x66 };
	characteristic.updateDescriptorValue(BluetoothUuid("1234"), value);

	descriptors = characteristic.getDescriptors();
	g_assert(descriptors.at(0).getValue() == BluetoothGattValue({0x44, 0x55, 0x66}));

	BluetoothGattService service;
	service.addCharacteristic(characteristic);

	service.updateDescriptorValue(BluetoothUuid("5678"), BluetoothUuid("1234"), { 0x99, 0x88 });

	characteristic = service.getCharacteristic(BluetoothUuid("5678"));
	g_assert(characteristic.getDescriptor(BluetoothUuid("1234")).getValue() == BluetoothGattValue( { 0x99, 0x88 }));
}

static void test_descriptor_permissions(void)
{
	BluetoothGattDescriptor descriptor;

	g_assert(descriptor.getPermissions() == BluetoothGattPermission::PERMISSION_NONE);

	descriptor.setPermissions(BluetoothGattPermission::PERMISSION_READ | BluetoothGattPermission::PERMISSION_WRITE);
	g_assert(descriptor.isPermissionSet(BluetoothGattPermission::PERMISSION_READ | BluetoothGattPermission::PERMISSION_WRITE));
	g_assert(descriptor.isPermissionSet(BluetoothGattPermission::PERMISSION_READ));
	g_assert(descriptor.isPermissionSet(BluetoothGattPermission::PERMISSION_WRITE));
}

int main(int argc, char **argv)
{
	g_test_init(&argc, &argv, nullptr);

	g_test_add_func("/gatt/descriptor-properties", test_descriptor_properties);
	g_test_add_func("/gatt/characteristic-properties", test_characteristic_properties);
	g_test_add_func("/gatt/observer", test_gatt_observer);
	g_test_add_func("/gatt/update-characteristic-value", test_update_characteristic_value);
	g_test_add_func("/gatt/update-descriptor-value", test_update_descriptor_value);
	g_test_add_func("/gatt/descriptor-permissions", test_descriptor_permissions);

	return g_test_run();
}
