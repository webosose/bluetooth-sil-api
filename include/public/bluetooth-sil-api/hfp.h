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

#ifndef BLUETOOTH_SIL_HFP_H_
#define BLUETOOTH_SIL_HFP_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

const std::string BLUETOOTH_PROFILE_ID_HFP = "HFP";

/**
 * @brief Bluetooth HFP AT command
 *
 *        Describes HFP AT command which consists of type, command and arguments.
 */
class BluetoothHfpAtCommand
{
public:
	/**
	 * @brief AT command type.
	 *        BASIC type means that the AT command includes a single letter command
	 *        such as ATA and ATD. Anything following the single letter command
	 *        (i.e., 'A' and 'D') will be returned in 'arguments' field.
	 *        ACTION type means that the AT command includes an execution operation
	 *        used to perform an action or retrieve information/status such as "AT+FOO"
	 *        , which does not have any 'arguments' values.
	 *        READ type means that the AT command includes a read operation used to
	 *        retrieve the current settings such as "AT+FOO?", which does not have
	 *        any 'arguments' values.
	 *        SET type means that the AT command includes a set operation used to
	 *        change the settings such as "AT+FOO=".
	 *        TEST type means that the AT command a test operation used to check
	 *        whether a certain AT command is supported such as "AT+FOO=?". There
	 *        are no arguments for this command type.
	 */
	enum Type
	{
		UNKNOWN,
		BASIC,
		ACTION,
		READ,
		SET,
		TEST
	};

	/**
	 *@brief Default c'tor
	 */
	BluetoothHfpAtCommand() :
		type(UNKNOWN)
	{
	}

	/**
	 *@brief Copy c'tor
	 *
	 *@param other Other AT command to copy from
	 */
	BluetoothHfpAtCommand(const BluetoothHfpAtCommand &other) :
		type(other.getType()),
		command(other.getCommand()),
		arguments(other.getArguments())
	{
	}

	/**
	 * @brief Returns the type of AT command.
	 *
	 * @return type of the AT command
	 */
	Type getType() const { return type; }

	/**
	 * @brief Returns AT command without 'AT' string.
	 *
	 * @return AT command without 'AT' string
	 */
	std::string getCommand() const { return command; }

	/**
	 * @brief Returns the arguments of AT command.
	 *
	 * @return arguments of AT command
	 */
	std::string getArguments() const { return arguments; }

	/**
	 * @brief Set the type of AT command.
	 *
	 * @param type Type of the element
	 */
	void setType(Type type) { this->type = type; }

	/**
	 * @brief Set AT command without 'AT' string.
	 *
	 * @param command AT command without 'AT' string
	 */
	void setCommand(const std::string &command) { this->command = command; }

	/**
	 * @brief Set the arguments of AT command.
	 *
	 * @param arguments Arguments of AT command
	 */
	void setArguments(const std::string &arguments) { this->arguments = arguments; }


private:
	Type type;
	std::string command;
	std::string arguments;
};

/**
 * @brief This interface is the base to implement an observer for the HFP.
 */
class BluetoothHfpStatusObserver
{
public:
	/**
	 * The method is called when SCO connection state is changed
	 *
	 * @param address Address of the device
	 * @param state Current SCO connection state between local and the remote device
	 */
	virtual void scoStateChanged(const std::string &address, bool state) { }

	/**
	 * The method is called when a local device(AG) receives AT commands from a remote device(HF).
	 *
	 * @param address Address of the device
	 * @param atCommand Bluetooth AT command which is sent from a remote device(HF)
	 */
	virtual void atCommandReceived(const std::string &address, const BluetoothHfpAtCommand &atCommand) { }

	/**
	 * The method is called when a local device(HF) receives result codes from a remote device(AG).
	 *
	 * @param address Address of the device
	 * @param resultCode Result code which is sent from a remote device(AG)
	 */
	virtual void resultCodeReceived(const std::string &address, const std::string &resultCode) { }
};

/**
 * @brief Interface to abstract the operations for the HFP bluetooth profile.
 */
class BluetoothHfpProfile
{
public:
	/**
	 * @brief Default Constructor
	 */
	BluetoothHfpProfile() { }

	virtual ~BluetoothHfpProfile() { }

	/**
	 * @brief Register a observer for the HFP. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 * @param observer Observer instance to register.
	 */
	void registerObserver(BluetoothHfpStatusObserver *observer)
	{
		this->observer = observer;
	}

	/**
	 * @brief Open the SCO connection.
	 *
	 * @param address Address of the device
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void openSCO(const std::string &address, BluetoothResultCallback callback) = 0;

	/**
	 * @brief Close the SCO connection.
	 *
	 * @param address Address of the device
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void closeSCO(const std::string &address, BluetoothResultCallback callback) = 0;

	/**
	 * @brief Send a unsolicited result code for a remote device(HF).
	 *
	 * @param address Address of the device
	 * @param resultCode Unsolicited result code to send
	 *
	 * @return BLUETOOTH_ERROR_NONE when operation was successful another error code otherwise
	 */
	virtual BluetoothError sendResultCode(const std::string &address, const std::string &resultCode) = 0;

	/**
	 * @brief Send a AT commmand to a remote device(AG).
	 *
	 * @param address Address of the device
	 * @param atCommand AT command to send
	 *
	 * @return BLUETOOTH_ERROR_NONE when operation was successful another error code otherwise
	 */
	virtual BluetoothError sendAtCommand(const std::string &address, const BluetoothHfpAtCommand &atCommand) = 0;

protected:
	/**
	 * @brief Retrieve the HFP status observer.
	 *
	 *        If no observer was registered with registerObserver before, a NULL object
	 *        instance will be returned which is usable but will do nothing when
	 *        called.
	 *
	 * @return The HFP status observer
	 */
	BluetoothHfpStatusObserver *getHfpObserver()
	{
		static BluetoothHfpStatusObserver nullObserver;

		if (!observer)
			return &nullObserver;

		return observer;
	}

private:
	BluetoothHfpStatusObserver *observer;
};

#endif
