// Copyright (c) 2014-2021 LG Electronics, Inc.
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

#ifndef BLUETOOTH_SIL_H_
#define BLUETOOTH_SIL_H_

#include <vector>
#include <string>
#include <functional>
#include <cstdint>
#include <memory>
#include <typeinfo>
#include <stdexcept>
#include <list>

/*
 * The complete API is too large to be manageable as a single file and hence
 * has been split between the following headers.
 *
 * The order in which they are included is important as each header assumes all
 * of its dependencies are already met.
 *
 * It is not intended that these files would ever be included directly by any client.
 * Accordingly, each has a guard test and will raise a compile error if that is done.
 *
 */
#include <bluetooth-sil-api/errors.h>
#include <bluetooth-sil-api/pairing.h>
#include <bluetooth-sil-api/properties.h>
#include <bluetooth-sil-api/profile.h>
#include <bluetooth-sil-api/ftp.h>
#include <bluetooth-sil-api/opp.h>
#include <bluetooth-sil-api/a2dp.h>
#include <bluetooth-sil-api/observer.h>
#include <bluetooth-sil-api/adapter.h>
#include <bluetooth-sil-api/uuid.h>
#include <bluetooth-sil-api/gatt.h>
#include <bluetooth-sil-api/pbap.h>
#include <bluetooth-sil-api/map.h>
#include <bluetooth-sil-api/avrcp.h>
#include <bluetooth-sil-api/spp.h>
#include <bluetooth-sil-api/hfp.h>
#include <bluetooth-sil-api/pan.h>
#include <bluetooth-sil-api/hid.h>
#include <bluetooth-sil-api/mesh.h>

/**
 * @brief Current API version of the SIL
 */
const int BLUETOOTH_SIL_API_VERSION = 1;

/**
 * @brief Base class for the implementation of a SIL for the Bluetooth management daemon.
 *
 * It provides the entry point to the available Bluetooth adapters.
 *
 * The implementation of a SIL can assume that it runs within the context of
 * a GMainLoop.
 */
class BluetoothSIL
{
public:
	/**
	 * @brief Default C'tor
	 */
	BluetoothSIL() : observer(0) { }

	/**
	 * @brief Register a observer for the SIL. The observer will be notified about
	 *        any events.
	 *
	 *        A subsequent call will override the currently registered observer.
	 *
	 * @param observer Observer instance to register.
	 */
	void registerObserver(BluetoothSILStatusObserver *observer)
	{
		this->observer = observer;
	}

	/**
	 * @brief Get the default adapter
	 * @return Instance of the default Bluetooth adapter or null if no default adapter
	 *         is available.
	 */
	virtual BluetoothAdapter *getDefaultAdapter() = 0;

	/**
	 * @brief Get all available Bluetooth adapters
	 * @return List of all available Bluetooth adapters. If no adapters are available
	 *         the list will be empty.
	 */
	virtual std::vector<BluetoothAdapter *> getAdapters() = 0;

	/**
	 * Virtual destructor to prevent destructor slicing.
	 *
	 * The SIL must actually return a sub-class of BluetoothSIL. As the client is
	 * responsible for deleting the instance when done, we need to prevent slicing.
	 */

	virtual ~BluetoothSIL() {}

protected:
	BluetoothSILStatusObserver *observer;
};

/**
 * @brief Create a instance of the Bluetooth SIL interface.
 *
 *        The result of subsequent calls is unknown. Creating a SIL should be done
 *        only once.
 *
 *        The caller is responsible for freeing the allocated SIL instance.
 *
 *        If the passed version number of the API doesn't match the one with which
 *        the SIL has been implemented, NULL is returned.
 *
 * @param version    Version of the API to use. Should be BLUETOOTH_SIL_API_VERSION.
 * @param capability The pairing IO capability of the device, which indicates the
 *                   type of pairing supported.
 * @return Instance of the Bluetooth SIL.
 */
extern "C" {
	BluetoothSIL *createBluetoothSIL(const int version, BluetoothPairingIOCapability capability);
}

#endif


