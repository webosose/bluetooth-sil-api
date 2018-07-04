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

#ifndef BLUETOOTH_SIL_FTP_H_
#define BLUETOOTH_SIL_FTP_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

const std::string BLUETOOTH_PROFILE_ID_FTP = "FTP";

/**
 * @brief Bluetooth FTP element
 *
 *        Describes either a folder or a file with its properties.
 */
class BluetoothFtpElement
{
public:
	/**
	 * @brief Type of an element
	 */
	enum Type
	{
		UNKNOWN,
		FOLDER,
		FILE
	};

	/**
	 * @brief Field markers used to check if a field is set or not. See
	 *        \ref{isFieldSet}.
	 */
	enum Field : uint8_t
	{
		SIZE             = (1 << 0),
		USER_PERMISSION  = (1 << 1),
		GROUP_PERMISSION = (1 << 2),
		OTHER_PERMISSION = (1 << 3),
		MODIFIED_TIME    = (1 << 4),
		ACCESSED_TIME    = (1 << 5),
		CREATED_TIME     = (1 << 6)
	};

	/**
	 * @brief Permission for an element.
	 */
	enum Permission : uint8_t
	{
		NONE = 0,
		READ = 1,
		WRITE = 2,
		DELETE = 4
	};

	/**
	 * @brief Std. C'tor
	 */
	BluetoothFtpElement() :
		fieldsSet(0),
		type(UNKNOWN),
		size(0),
		userPermission(0),
		groupPermission(0),
		otherPermission(0)
	{
	}

	/**
	 * @brief Copy c'tor
	 * @param other Other element to copy from.
	 */
	BluetoothFtpElement(const BluetoothFtpElement &other) :
		fieldsSet(other.fieldsSet),
		name(other.getName()),
		type(other.getType()),
		size(other.getSize()),
		userPermission(other.getUserPermission()),
		groupPermission(other.getGroupPermission()),
		otherPermission(other.getOtherPermission()),
		modifiedTime(other.getModifiedTime()),
		accessedTime(other.getAccessedTime()),
		createdTime(other.getCreatedTime())
	{
	}

	/**
	 * @brief Check if a field is set.
	 *
	 * @param field Type of the field to check
	 * @return True, if the field is set. False otherwise.
	 */
	bool isFieldSet(Field field)
	{
		return (fieldsSet & field) == field;
	}

	/**
	 * @brief Returns the name of the element.
	 * @return Name of the element
	 */
	std::string getName() const { return name; }

	/**
	 * @brief Returns the type of the element.
	 * @return type of the element
	 */
	Type getType() const { return type; }

	/**
	 * @brief Returns the size of the element.
	 *
	 *        Is only valid when the corresponding field is marked as set.
	 *        See \ref{isFieldSet}.
	 *
	 * @return Size of the element
	 */
	uint64_t getSize() const { return size; }

	/**
	 * @brief Returns the user permission of the element.
	 *
	 *        Is only valid when the corresponding field is marked as set.
	 *        See \ref{isFieldSet}.
	 *
	 * @return User permission of the element
	 */
	uint8_t getUserPermission() const { return userPermission; }

	/**
	 * @brief Returns the group permission of the element.
	 *
	 *        Is only valid when the corresponding field is marked as set.
	 *        See \ref{isFieldSet}.
	 *
	 * @return Group permission of the element
	 */
	uint8_t getGroupPermission() const { return groupPermission; }

	/**
	 * @brief Returns the other permission of the element.
	 *
	 *        Is only valid when the corresponding field is marked as set.
	 *        See \ref{isFieldSet}.
	 *
	 * @return Other permission of the element
	 */
	uint8_t getOtherPermission() const { return otherPermission; }

	/**
	 * @brief Returns the time the element was last modified.
	 *
	 *        Is only valid when the corresponding field is marked as set.
	 *        See \ref{isFieldSet}.
	 *
	 * @return Time the element was last modified.
	 */
	time_t getModifiedTime() const { return modifiedTime; }

	/**
	 * @brief Returns the time the element was last accessed.
	 *
	 *        Is only valid when the corresponding field is marked as set.
	 *        See \ref{isFieldSet}.
	 *
	 * @return Time the element was last accessed.
	 */
	time_t getAccessedTime() const { return accessedTime; }

	/**
	 * @brief Returns the time the element was created.
	 *
	 *        Is only valid when the corresponding field is marked as set.
	 *        See \ref{isFieldSet}.
	 *
	 * @return Time the element was created.
	 */
	time_t getCreatedTime() const { return createdTime; }

	/**
	 * @brief Set the name of the element
	 *
	 *        Is available for all element types.
	 *
	 * @param name Name of the element
	 */
	void setName(const std::string &name) { this->name = name; }

	/**
	 * @brief Set the type of the element.
	 *
	 *        Is available for all element types.
	 *
	 * @param type Type of the element
	 */
	void setType(Type type) { this->type = type; }

	/**
	 * @brief Set the size of the element.
	 *
	 * @param size Size of the element
	 */
	void setSize(uint64_t size) { this->size = size; fieldsSet |= SIZE; }

	/**
	 * @brief Set the user permission.
	 *
	 * @param permission Bit set for the user permission.
	 */
	void setUserPermission(uint8_t permission) { userPermission = permission; fieldsSet |= USER_PERMISSION; }

	/**
	 * @brief Set the group permission.
	 *
	 * @param permission Bit set for the group permission.
	 */
	void setGroupPermission(uint8_t permission) { groupPermission = permission; fieldsSet |= GROUP_PERMISSION; }

	/**
	 * @brief Set the other permission.
	 *
	 * @param permission Bit set for the other permission.
	 */
	void setOtherPermission(uint8_t permission) { otherPermission = permission; fieldsSet |= OTHER_PERMISSION; }

	/**
	 * @brief Set the time the element was last modified.
	 *
	 * @param time Time when the element was last modified.
	 */
	void setModifiedTime(time_t time) { modifiedTime = time; fieldsSet |= MODIFIED_TIME; }

	/**
	 * @brief Set the size of the element.
	 *
	 * @param time Time when the element was last accessed.
	 */
	void setAccessedTime(time_t time) { accessedTime = time; fieldsSet |= ACCESSED_TIME; }

	/**
	 * @brief Set the size of the element.
	 *
	 * @param time Time when the element was created.
	 */
	void setCreatedTime(time_t time) { createdTime = time; fieldsSet |= CREATED_TIME; }

private:
	uint8_t fieldsSet;
	std::string name;
	Type type;
	uint64_t size;
	uint8_t userPermission;
	uint8_t groupPermission;
	uint8_t otherPermission;
	time_t modifiedTime;
	time_t accessedTime;
	time_t createdTime;
};


typedef uint64_t BluetoothFtpTransferId;

const BluetoothFtpTransferId BLUETOOTH_FTP_TRANSFER_ID_INVALID = 0;

/**
 * @brief Callback to return a list of files requested over the FTP profile
 */
typedef std::function<void(BluetoothError, const std::vector<BluetoothFtpElement> element)>
BluetoothFtpListFolderResultCallback;

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
 * @param finished Indicates wether the transfer is still running or finished. Will be false
 *                 as long as the transfer is running and switches to true when the transfer
 *                 is done.
 */
typedef std::function<void(BluetoothError, uint64_t bytesTransferred, bool finished)>
BluetoothFtpTransferResultCallback;


/**
 * @brief Interface to abstract the operations for the FTP bluetooth profile.
 */
class BluetoothFtpProfile
{
public:
	virtual ~BluetoothFtpProfile() { }

	/**
	 * @brief List a folder on a remote device. The root folder is always '/'.
	 *
	 * @param address Address of the remote device
	 * @param path Path to list elements for
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void listFolder(const std::string &address, const std::string &path,
							BluetoothFtpListFolderResultCallback callback) = 0;

	/**
	 * @brief Pull a file from a remote device.
	 *
	 * @param address Address of the remote device
	 * @param sourcePath Path of the file which should be pulled
	 * @param targetPath Path to place where the fetched file should be placed locally.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 * @return Id of the transfer or BLUETOOTH_FTP_TRANSFER_ID_INVALID when the transfer could not be started.
	 */
	virtual BluetoothFtpTransferId pullFile(const std::string &address, const std::string &sourcePath,
							 const std::string &targetPath, BluetoothFtpTransferResultCallback callback) = 0;

	/**
	 * @brief Push a local file to a remote device.
	 *
	 * @param address Address of the remote device
	 * @param sourcePath Local path of the file which should be pushed
	 * @param targetPath Path on the remote device where the send file should be stored
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 * @return Id of the transfer or BLUETOOTH_FTP_TRANSFER_ID_INVALID when the transfer could not be started.
	 */
	virtual BluetoothFtpTransferId pushFile(const std::string &address, const std::string &sourcePath,
							 const std::string &targetPath, BluetoothFtpTransferResultCallback callback) = 0;

	/**
	 * @brief Cancel an ongoing FTP transfer
	 *
	 * @param id Id of the transfer to cancel.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void cancelTransfer(BluetoothFtpTransferId id, BluetoothResultCallback callback) = 0;
};

#endif
