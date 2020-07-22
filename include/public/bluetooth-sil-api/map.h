// Copyright (c) 2020 LG Electronics, Inc.
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

#ifndef BLUETOOTH_SIL_MAP_H_
#define BLUETOOTH_SIL_MAP_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

const std::string BLUETOOTH_PROFILE_ID_MAP = "MAP";

/**
 * Callback to indicate the status of an asynchronous operation.
 *
 * The definition of this type makes it explicit that all callbacks are
 * invoked with a BluetoothError value, enforcing the contract that only
 * documented error values will be returned.
 *
 */
typedef std::function<void(BluetoothError, const std::string& )> BluetoothMapCallback;

typedef std::function<void(BluetoothError, std::list<std::string>filters)>
BluetoothMapListFiltersResultCallback;

typedef std::function<void(BluetoothError, std::vector<std::string>&)>BluetoothMapGetFoldersCallback;

/**
 * @brief This interface is the base to implement an observer for the MAP profile.
 */
class BluetoothMapStatusObserver
{
public:
	/**
	 * @brief The method is called when a MAP specific property changes
	 *
	 * @param properties List of properties which have changed.
	 */
	virtual void profilePropertiesChanged(const std::string &adapterAddress, const std::string &address) { }
};

/**
 * @brief The BluetoothMapProperty class abstracts several properties of message or filters of getMessageList.
 */
class BluetoothMapProperty
{
public:
	enum Type
	{
		/**
		 * @brief Property does not contain any information.
		 **/
		EMPTY,
		/**
		 * @brief start offSet for getMessageList.
		 *
		 *        Type: uint16_t
		 *        Access: Service (write) , SIL(Read)
		 **/
		STARTOFFSET,
		/**
		 * @brief maxnumber of messages needs to be downloaded for getMessageList.
		 *
		 *        Type: uint16_t
		 *        Access: Service (write) , SIL(Read)
		 **/
		MAXCOUNT,
		/**
		 * @brief Folder which the message belongs to.
		 *
		 *        Type: sting
		 *        Access: Service (Read) , SIL(write)
		 **/
		FOLDER,
		/**
		 * @brief max length of subject , used in getMessageList.
		 *
		 *        Type: uint8_t
		 *        Access: Service (write) , SIL(Read)
		 **/
		SUBJECTLENGTH,
		/**
		 * @brief message fields those needs to be downloaded for getMessageList.
		 *
		 *        Type: std::vector<std::string>
		 *        Access: Service (write) , SIL(Read)
		 **/
		FIELDS,
		/**
		 * @brief Message Types needs to be downloaded , used in getMessageList.
		 *
		 *        Type: std::vector<std::string>
		 *        Access: Service (write) , SIL(Read)
		 **/
		MESSAGETYPES,
		/**
		 * @brief Period from which messages needs to be downloaded , used in getMessageList.
		 *
		 *        Type: std::string
		 *        Access: Service (write) , SIL(Read)
		 **/
		PERIODBEGIN,
		/**
		 * @brief Period till which messages needs to be downloaded , used in getMessageList.
		 *
		 *        Type: std::string
		 *        Access: Service (write) , SIL(Read)
		 **/
		PERIODEND,
		/**
		 * @brief Message status . It is also used as filter for getMessageList
		 *
		 *        Type: Boolean
		 *        Access: Service (Read/write) , SIL(Read/write)
		 **/
		READ,
		/**
		 * @brief Message recipient name. It is also used as filter in getMessageList
		 *
		 *        Type: std::string
		 *        Access: Service (Read/write), SILs(Read/write)
		 **/
		RECIPIENT,
		/**
		 * @brief Message recipient address.
		 *
		 *        Type: Boolean
		 *        Access: Service (Read) , SIL(write)
		 **/
		RECIPIENTADDRESS,
		/**
		 * @brief Message sender name. It is also used as filter in getMessageList.
		 *
		 *        Type: std::string
		 *        Access: Service (Read/write), SILs(Read/write)
		 **/
		SENDER,
		/**
		 * @brief Message sender address.
		 *
		 *        Type: Boolean
		 *        Access: Service (Read) , SIL(write)
		 **/
		SENDERADDRESS,
		/**
		 * @brief Message Reply-To address.
		 *
		 *        Type: std::string
		 *        Access: Service (Read), SIL(write)
		 **/
		REPLYTO,
		/**
		 * @brief Message priority flag. It is also used as filter in getMessageList
		 *
		 *        Type: Boolean
		 *        Access: Service (Read/write) , SIL(Read/write)
		 **/
		PRIORITY,
		/**
		 * @brief Message subject.
		 *
		 *        Type: Boolean
		 *        Access: Service (Read) , SIL(write)
		 **/
		SUBJECT,
		/**
		 * @brief Message timestamp , indicates when the message is sent or received.
		 *
		 *        Type: std::string
		 *        Access: Service (Read) , SIL(write)
		 **/
		TIMESTAMP,
		/**
		 * @brief Message size in bytes.
		 *
		 *        Type: uint64_t
		 *        Access: Service (Read) , SIL(write)
		 **/
		SIZE,
		/**
		 * @brief Message text flag.
		 *
		 *        Type: boolean
		 *        Access: Service (Read) , SIL(write)
		 **/
		TEXTTYPE,
		/**
		 * @brief Message status.
		 *
		 *        Type: std::string
		 *        Access: Service (Read) , SIL(write)
		 **/
		STATUS,
		/**
		 * @brief Message overall attachment size in bytes.
		 *
		 *        Type: uint64_t
		 *        Access: Service (Read) , SIL(write)
		 **/
		ATTACHMENTSIZE,
		/**
		 * @brief Message protected flag.
		 *
		 *        Type: boolean
		 *        Access: Service (Read) , SIL(write)
		 **/
		PROTECTED,
		/**
		 * @brief Message sent flag.
		 *
		 *        Type: boolean
		 *        Access: Service (Read) , SIL(write)
		 **/
		SENT
	};

	/**
	 * @brief Initialize an empty property
	 *
	 *        The type of the property will be set to EMPTY.
	 */
	BluetoothMapProperty() :
		type(EMPTY),
		impl(0)
	{
	}

	/**
	 * @brief Initialize the property with a type
	 * @param type Type of the property
	 */
	BluetoothMapProperty(Type type) :
		type(type),
		impl(0)
	{
	}

	/**
	 * @brief Copy constructor
	 * @param other Other property to create a copy from
	 */
	BluetoothMapProperty(const BluetoothMapProperty &other) :
		type(other.type),
		impl(other.impl),
		typeName(other.typeName)
	{
	}

	/**
	 * @brief Initialize the property with a type and a value
	 * @param type Type of the property
	 * @param value Value of the property
	 */
	template<class T>
	BluetoothMapProperty(Type type, T value) :
		type(type),
		impl(new PropertyImpl<T>(value)),
		typeName(typeid(T).name()) { }

	/**
	 * @brief Get the type of the property
	 * @return Type of the property
	 */
	Type getType() const
	{
		return type;
	}

	/**
	 * @brief Get the value of the property
	 * @return Value of the property
	 */
	template<class T>
	T getValue() const
	{
		std::string requestedType = typeid(T).name();

		if (requestedType != typeName)
		{
			throw std::logic_error("Non-matching types");
		}

		return dynamic_cast<PropertyImpl<T>*>(impl.get())->getValue();
	}

	/**
	 * @brief Set the value of the property
	 * @param value New value of the property
	 */
	template<class T>
	void setValue(T value)
	{
		impl.reset(new PropertyImpl<T>(value));
		typeName = typeid(T).name();
	}

private:
	struct BasePropertyImpl
	{
		virtual ~BasePropertyImpl() { }
	};

	template<class T>
	struct PropertyImpl : public BasePropertyImpl
	{
		PropertyImpl(T value) : value(value) { }
		~PropertyImpl() { }

		T getValue() const
		{
			return value;
		}

		T value;
	};

private:
	Type type;
	std::shared_ptr<BasePropertyImpl> impl;
	std::string typeName;
};

typedef std::vector<BluetoothMapProperty> BluetoothMapPropertiesList;

typedef std::vector<std::pair<std::string, BluetoothMapPropertiesList>> BluetoothMessageList;

typedef std::function<void(BluetoothError, BluetoothMessageList &)>BluetoothMapGetMessageListCallback;

/**
 * @brief Interface to abstract the operations for the MAP bluetooth profile.
 */
class BluetoothMapProfile
{
public:
	BluetoothMapProfile() { }

	virtual ~BluetoothMapProfile() { }

	/**
	 * @brief Register a observer for the MAP profile. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 * @param observer Observer instance to register.
	 */
	void registerObserver(BluetoothMapStatusObserver *observer)
	{
		this->observer = observer;
	}

	/**
	 * @brief Connect with a MAP profile on a specific remote device.
	 *
	 * @param address Address of the device to connect to.
	 * @param instanceName Indicate specific MAS instance of MSE to which connection should be initiated.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed with session Path.
	 */
	virtual void connect(const std::string &address, const std::string &instanceName, BluetoothMapCallback callback) = 0;

	/**
	 * @brief Disconnect a remote device from the MAP profile if connected.
	 *
	 *        If not connected the callback will return immediately and the operation
	 *        marked as succeeded.
	 *
	 * @param sessionKey Unique identifier of device Address of the remote device to disconnect
	 * @param sessionId Unique identifier of a session which was returned from map/connect.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void disconnect(const std::string &sessionKey, const std::string &sessionId, BluetoothMapCallback callback) = 0;

	/**
	 * @brief To get folder list of a connected MAS session.
	 *
	 * @param sessionKey Unique identifier of connected MAP insatnce(address_instanceName)
	 * @param sessionId Unique identifier of a session which was returned from map/connect.
	 * @param startOffset start index of folder listing.
	 * @param maxCount max number of items which can be listed.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void getFolderList(const std::string &sessionKey, const std::string &sessionId, const uint16_t &startOffset, const uint16_t &maxCount, BluetoothMapGetFoldersCallback callback) = 0;

	/**
	 * @brief To get the list of supported filters field.
	 *
	 *        This method is only for MCE to get the list of filters.
	 *
	 * @param sessionKey Unique identifier of connected MAP insatnce(address_instanceName)
	 * @param sessionId Unique identifier of a session which was returned from map/connect.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed with list of supported filters.
	 */
	virtual void getMessageFilters(const std::string &sessionKey, const std::string &sessionId, BluetoothMapListFiltersResultCallback callback) = 0;

	/**
	 * @brief To set folder of a connected MAS session.
	 *
	 * @param sessionKey Unique identifier of connected MAP insatnce(address_instanceName)
	 * @param sessionId Unique identifier of a session which was returned from map/connect.
	 * @param folder name of the folder or path to the folder.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	 virtual void setFolder(const std::string &sessionKey, const std::string &sessionId, const std::string &folder, BluetoothResultCallback callback) = 0;

	/**
	 * @brief To Get Message list of a connected MAS session.
	 *
	 * @param sessionKey Unique identifier of connected MAP insatnce(address_instanceName)
	 * @param sessionId Unique identifier of a session which was returned from map/connect.
	 * @param folder name of the folder or path to the folder.
	 * @param filters list of filters applicable for this application.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	 virtual void getMessageList(const std::string &sessionKey, const std::string &sessionId, const std::string &folder, const BluetoothMapPropertiesList &filters, BluetoothMapGetMessageListCallback callback) = 0;

protected:
	/**
	 * @brief Retrieve the MAP status observer
	 *
	 *        If no observer was registered with registerObserver before a NULL object
	 *        instance will be returned which is usable but will doing nothing when
	 *        called.
	 *
	 * @return The MAP status observer
	 */
	BluetoothMapStatusObserver *getMapObserver()
	{
		static BluetoothMapStatusObserver nullObserver;

		if (!observer)
			return &nullObserver;

		return observer;
	}

private:
	BluetoothMapStatusObserver *observer;

};
#endif //BLUETOOTH_SIL_MAP_H_
