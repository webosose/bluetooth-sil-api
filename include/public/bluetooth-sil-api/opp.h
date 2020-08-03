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

#ifndef BLUETOOTH_SIL_OPP_H_
#define BLUETOOTH_SIL_OPP_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

const std::string BLUETOOTH_PROFILE_ID_OPP = "OPP";

typedef uint64_t BluetoothOppTransferId;
const BluetoothOppTransferId BLUETOOTH_OPP_TRANSFER_ID_INVALID = 0;

/**
 * @brief Callback used to indicate the transfer status of a file to or from a remote device
 *
 *        This call back is called to indicate the status of a transfer. The parameter
 *        \ref{bytesTransferred} indicates the number of bytes already transfered and the
 *        parameter \ref{finished} indicates if the transfer has finished or not. The callback
 *        is meant to be called multiple times. The last call is either when an error not equal
 *        to BLUETOOTH_ERROR_NONE is supplied or the parameter \ref{finished} switched to true.
 *
 * @param bytesTransferred Number of bytes already transferred
 * @param totalSize Number of bytes send file
 * @param finished Indicates wether the transfer is still running or finished. Will be false
 *                 as long as the transfer is running and switches to true when the transfer
 *                 is done.
 */
typedef std::function<void(BluetoothError, uint64_t bytesTransferred, uint64_t totalSize, bool finished)>
BluetoothOppTransferResultCallback;

/**
 * @brief This interface is the base to implement an observer for the OPP profile.
 */
class BluetoothOppStatusObserver
{
public:
	/**
	 * @brief The method is called when the device has to display a request transfer confirmation
	 *        dialog for a specific device. The user has to confirm that the acception or
	 *        rejection the same on both devices.
	 *
	 * @param transferId Id of the incoming transfer
	 * @param adapterAddress Address of the Adapter
	 * @param address Address of the device
	 * @param deviceName deviceName of the device
	 * @param fileName fileName of the push file
	 * @param fileSize fileSize of the push file
	*/
	virtual void transferConfirmationRequested(BluetoothOppTransferId transferId, const std::string &adapterAddress, const std::string &address,
	                                         const std::string &deviceName, const std::string &fileName,
	                                         uint64_t fileSize) { }

	/**
	 * @brief This method is called by stack when it receives a file from remote device to
	 *        let the service know the transfer status.
	 *
	 * @param transferId Id of the transfer whose state has changed
	 * @param transferred Number of bytes received file
	 * @param finished Indicates wether the receiving bytes is still running or finished
	*/
	virtual void transferStateChanged(BluetoothOppTransferId transferId, uint64_t transferred, bool finished) { }
};

/**
 * @brief Interface to abstract the operations for the OPP bluetooth profile.
 */
class BluetoothOppProfile
{
public:
	/**
	 * @brief Default C'tor
	 */
	 BluetoothOppProfile() : observer(0) { }

	virtual ~BluetoothOppProfile() { }

	/**
	 * @brief Register a observer for the OPP profile. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 * @param observer Observer instance to register.
	 */
	void registerObserver(BluetoothOppStatusObserver *observer)
	{
		this->observer = observer;
	}

	/**
	 *
	 * @param address Address of the remote device
	 * @param sourcePath Local path of the file which should be pushed
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual BluetoothOppTransferId pushFile(const std::string &address, const std::string &sourcePath, BluetoothOppTransferResultCallback callback) = 0;

	/**
	 * @brief Cancel an ongoing OPP transfer
	 *
	 * @param id Id of the transfer to cancel.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void cancelTransfer(BluetoothOppTransferId id, BluetoothResultCallback callback) = 0;

	/**
	 * @brief Accept the object push request with a remote device or reject the request. Should
	 *        be only called after the SIL asked for it by calling the displayTransferConfirmation
	 *        method of the registered observer which is the BluetoothOppStatusObserver class.
	 *        The displayTransferConfirmation is the signal of pushFile.
	 *
	 *        This method is available only for a server.
	 *
	 * @param transferId Id of the transfer
	 * @param accept True to accept the object push request or False to reject.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void supplyTransferConfirmation(BluetoothOppTransferId transferId, bool accept, BluetoothResultCallback callback) = 0;

protected:
	/**
	 * @brief Retrieve the OPP status observer
	 *
	 *        If no observer was registered with registerObserver before a NULL object
	 *        instance will be returned which is usable but will doing nothing when
	 *        called.
	 *
	 * @return The OPP status observer
	 */
	BluetoothOppStatusObserver* getOppObserver()
	{
		static BluetoothOppStatusObserver nullObserver;

		if (!observer)
			return &nullObserver;

		return observer;
	}

private:
	BluetoothOppStatusObserver *observer;
};

#endif
