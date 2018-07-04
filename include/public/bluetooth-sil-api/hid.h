// Copyright (c) 2016-2018 LG Electronics, Inc.
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

#ifndef BLUETOOTH_SIL_HID_H_
#define BLUETOOTH_SIL_HID_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

const std::string BLUETOOTH_PROFILE_ID_HID = "HID";

enum HidReportType
{
	HID_REPORT_INPUT = 1,
	HID_REPORT_OUTPUT,
	HID_REPORT_FEATURE,
};

/**
 * @brief Callback used to receive data from a remote device
 *
 * @param error Error code
 * @param data Received data
 * @param size Number of data
 */
typedef std::function<void(BluetoothError, const uint8_t *, size_t)> BluetoothHidGetCallback;

/**
 * @brief This interface is the base to implement an observer for the HID profile.
 */
class BluetoothHidStatusObserver
{
};

/**
 * @brief Interface to abstract the operations for the HID bluetooth profile.
 */
class BluetoothHidProfile
{
public:
	/**
	 * @brief Register a observer for the HID profile. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 * @param observer Observer instance to register.
	 */
	void registerObserver(BluetoothHidStatusObserver *observer)
	{
		this->observer = observer;
	}

	/**
	 * @brief Set L2CAP priority
	 *
	 * @param address Address of the device to send to.
	 * @param high True to set high, false to set normal
	 *
	 * @return error code.
	 */
	virtual BluetoothError setPriorityHigh(const std::string &address, bool high) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Send data to a remote device.
	 *
	 * @param address Address of the device to send to.
	 * @param data Data sequence packed by vendor specific way.
	 * @param size Length of data.
	 *
	 * @return error code.
	 */
	virtual BluetoothError sendData(const std::string &address, uint8_t *data, size_t size) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Get report from a remote device
	 *
	 * @param address Address of the device to receive from.
	 * @param type Report type.
	 * @param reportId Id of report to get
	 * @param size Length of report to get.
	 * @param callback Callback function which is called when the operation is done or has failed.
	 */
	virtual void getReport(const std::string &address, HidReportType type, uint8_t reportId, size_t size, BluetoothHidGetCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, 0, 0);
	}

	/**
	 * @brief Set report to a remote device
	 *
	 * @param address Address of the device to send to.
	 * @param type Report type.
	 * @param report Report data with report id at first byte.
	 * @param size Length of report.
	 * @param callback Callback function which is called when the operation is done or has failed.
	 */
	virtual void setReport(const std::string &address, HidReportType type, const uint8_t *report, size_t size, BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}
#ifdef USE_CMN_CONFIG
	/**
	 * @brief Save device information to non-volatile configuration.
	 *
	 * @param address Address of the device to save the information for.
	 * @param group Name of group.
	 */
	virtual bool saveNvConfig(const std::string &address, const std::string &group) = 0;

	/**
	 * @brief Remove device information from non-volatile configuration.
	 *
	 * @param group Name of group.
	 */
	virtual bool removeNvConfig(const std::string &group) = 0;
#endif
protected:
	/**
	 * @brief Retrieve the HID status observer
	 *
	 *        If no observer was registered with registerObserver before, a NULL object
	 *        instance will be returned which is usable but will do nothing when
	 *        called.
	 *
	 * @return The HID status observer
	 */
	BluetoothHidStatusObserver *getHidObserver()
	{
		static BluetoothHidStatusObserver nullObserver;

		if (!observer)
			return &nullObserver;

		return observer;
	}

private:
	BluetoothHidStatusObserver *observer;
};

#endif
