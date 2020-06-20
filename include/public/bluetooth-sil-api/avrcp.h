// Copyright (c) 2015-2020 LG Electronics, Inc.
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

#ifndef BLUETOOTH_SIL_AVRCP_H_
#define BLUETOOTH_SIL_AVRCP_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

const std::string BLUETOOTH_PROFILE_ID_AVRCP = "AVRCP";

typedef uint64_t BluetoothAvrcpRequestId;
const BluetoothAvrcpRequestId BLUETOOTH_AVRCP_REQUEST_ID_INVALID = 0;

/**
 * @brief Equalizer setting values for bluetooth player application
 */
enum BluetoothPlayerApplicationSettingsEqualizer
{
	EQUALIZER_UNKNOWN = 0,
	EQUALIZER_OFF,
	EQUALIZER_ON
};

/**
 * @brief Repeat setting values for bluetooth player application
 */
enum BluetoothPlayerApplicationSettingsRepeat
{
	REPEAT_UNKNOWN = 0,
	REPEAT_OFF,
	REPEAT_SINGLE_TRACK,
	REPEAT_ALL_TRACKS,
	REPEAT_GROUP
};

/**
 * @brief Shuffle setting values for bluetooth player application
 */
enum BluetoothPlayerApplicationSettingsShuffle
{
	SHUFFLE_UNKNOWN = 0,
	SHUFFLE_OFF,
	SHUFFLE_ALL_TRACKS,
	SHUFFLE_GROUP
};

/**
 * @brief Scan setting values for bluetooth player application
 */
enum BluetoothPlayerApplicationSettingsScan
{
	SCAN_UNKNOWN = 0,
	SCAN_OFF,
	SCAN_ALL_TRACKS,
	SCAN_GROUP
};

/**
 * @brief Bluetooth PASS THROUGH key code for AVRCP.
 *
 *        Describes PASS THROUGH Key code for AVRCP.
 */
enum BluetoothAvrcpPassThroughKeyCode
{
	KEY_CODE_UNKNOWN,
	KEY_CODE_POWER = 0x40,
	KEY_CODE_VOLUME_UP = 0x41,
	KEY_CODE_VOLUME_DOWN = 0x42,
	KEY_CODE_MUTE = 0x43,
	KEY_CODE_PLAY = 0x44,
	KEY_CODE_STOP = 0x45,
	KEY_CODE_PAUSE = 0x46,
	KEY_CODE_REWIND = 0x48,
	KEY_CODE_FAST_FORWARD = 0x49,
	KEY_CODE_NEXT = 0x4B,
	KEY_CODE_PREVIOUS = 0x4C
};

/**
 * @brief Bluetooth PASS THROUGH key status for AVRCP.
 *
 *        Describes PASS THROUGH key status for AVRCP.
 */
enum BluetoothAvrcpPassThroughKeyStatus
{
	KEY_STATUS_UNKNOWN,
	KEY_STATUS_PRESSED,
	KEY_STATUS_RELEASED
};

/**
 * @brief Remote Feature for avrcp
 *
 *        FEATURE_NONE : AVRCP 1.0,
 *        FEATURE_METADATA :  AVRCP 1.3
 *        FEATURE_ABSOLUTE_VOLUME : AVRCP 1.3 + Supports TG role and volume sync
 *        FEATURE_BROWSE : AVRCP 1.4 and above, with Browsing support
 */
enum BluetoothAvrcpRemoteFeatures
{
	FEATURE_NONE = 0x00,
	FEATURE_METADATA = 0x01,
	FEATURE_ABSOLUTE_VOLUME = 0x02,
	FEATURE_BROWSE = 0x03
};

enum BluetoothAvrcpSupportedNotificationEvent
{
	EVENT_STATUS_CHANGED = 0x01,
	EVENT_TRACK_CHANGED = 0x02,
	EVENT_TRACK_REACHED_END = 0x03,
	EVENT_TRACK_REACHED_START = 0x04,
	EVENT_PLAYBACK_POS_CHANGED = 0x05,
	EVENT_BATTERY_STATUS_CHANGED = 0x06,
	EVENT_SYSTEM_STATUS_CHANGED = 0x07,
	EVENT_PLAYER_APPLICATION_SETTING_CHANGED = 0x08,
	EVENT_NOW_PLAYING_CHANGED = 0x09,
	EVENT_AVAILABLE_PLAYERS_CHANGED = 0x0a,
	EVENT_ADDRESSED_PLAYERS_CHANGED = 0x0b,
	EVENT_UIDS_CHANGED = 0x0c,
	EVENT_VOLUME_CHANGED = 0x0d
};

enum class BluetoothAvrcpItemType
{
	ITEM_TYPE_AUDIO,
	ITEM_TYPE_VIDEO,
	ITEM_TYPE_FOLDER
};
enum class BluetoothAvrcpPlayerType
{
	PLAYER_TYPE_AUDIO,
	PLAYER_TYPE_VIDEO,
	PLAYER_TYPE_AUDIO_BROADCAST,
	PLAYER_TYPE_VIDEO_BROADCAST
};

class BluetoothPlayerInfo
{
public:
	/**
	 * @brief Default constructor
	 */
	BluetoothPlayerInfo() :
	playerType(BluetoothAvrcpPlayerType::PLAYER_TYPE_AUDIO),
	browsable(false),
	searchable(false),
	addressed(false) {}

	/**
	 * @brief Copy constructor
	 */
	BluetoothPlayerInfo(const BluetoothPlayerInfo &other) :
	playerPath(other.getPath()),
	name(other.getName()),
	playerType(other.getType()),
	addressed(other.getAddressed()),
	browsable(other.getBrowsable()),
	searchable(other.getSearchable()),
	playListPath(other.getPlayListPath()) {}

	/* Get functions for the member functions */
	/**
	 * @brief Retrieve media player path
	 * @return Returns player path
	 */
	std::string getPath() const { return playerPath; }
	/**
	 * @brief Retrieve player name
	 * @return Returns player name
	 */
	std::string getName() const { return name; }
	/**
	 * @brief Retrieve play list path of the media player
	 * @return Returns play list path
	 */
	std::string getPlayListPath() const { return playListPath; }
	/**
	 * @brief Retrieve player type
	 * @return Returns player type
	 */
	BluetoothAvrcpPlayerType getType() const { return playerType; }
	/**
	 * @brief Retrieve if the player is currently addressed.
	 * @return Returns if the player is addressed or not
	 *         true : Player is addressed
	 *         false : Player is not addressed
	 */
	bool getAddressed() const { return addressed; }
	/**
	 * @browsable Retrieve if the player supports media content browsing
	 * @returns Returns if the player is browsable or not.
	 *          true : Player is browsable
	 *          false : Player is not browsable
	 */
	bool getBrowsable() const { return browsable; }
	/**
	 * @browsable Retrieve if the player supports media content searching
	 * @returns Returns if the player supports searching or not.
	 *          true : Player supports searching
	 *          false : Player does not support searching
	 */
	bool getSearchable() const { return searchable; }

	/* Set functions for the member variables */
	/**
	 * @brief Set player path
	 * @param playerPath Player path
	 */
	void setPath(const std::string &playerPath) { this->playerPath = playerPath; }
	/**
	 * @brief Set player name
	 * @param name Player name
	 */
	void setName(const std::string &name) { this->name = name; }
	/**
	 * @brief Set play list path of the media player
	 * @param playListPath Play list path
	 */
	void setPlayListPath(const std::string &playListPath) { this->playListPath = playListPath; }
	/**
	 * @brief Set player type
	 * @param playerType Player type
	 */
	void setType(const BluetoothAvrcpPlayerType playerType) { this->playerType = playerType; }
	/**
	 * @brief Set if the player is addressed
	 * @param addressed true: Player is addressed
	 *                  false: Player is not addressed
	 */
	void setAddressed(const bool addressed) { this->addressed = addressed; }
	/**
	 * @brief Set if player supports media content browsing
	 * @param browsable true: Player supports browsing
	 *                  false: Player does not support browsing
	 */
	void setBrowsable(const bool browsable) { this->browsable = browsable; }
	/**
	 * @brief Set if player supports searching
	 * @param searchable true: Player supports searching
	 *                   false: Player does not support searching
	 */
	void setSearchable(const bool searchable) { this->searchable = searchable; }

private:
	/** Media Player object path */
	std::string playerPath;
	/** Media Player Name */
	std::string name;
	/** Media Player type */
	BluetoothAvrcpPlayerType playerType;
	/** True: If media player is addressed, False: If media player is not addressed */
	bool addressed;
	/** True: If media player is browsable, False: If media player is not browsable */
	bool browsable;
	/** True: If media player is searchable, False: If media player is not searchable */
	bool searchable;
	/** Now playing list path of the media player */
	std::string playListPath;
};

/**
 * @brief Bluetooth media meta data
 *
 *        Describes media meta data.
 */
class BluetoothMediaMetaData
{
public:
	/**
	 * @brief Default c'tor
	 */
	BluetoothMediaMetaData() :
		trackNumber(0),
		trackCount(0),
		duration(0)
	{
	}

	/**
	 * @brief Copy c'tor
	 * @param other Other media meta data object to copy from
	 */
	BluetoothMediaMetaData(const BluetoothMediaMetaData &other) :
		title(other.getTitle()),
		artist(other.getArtist()),
		album(other.getAlbum()),
		genre(other.getGenre()),
		trackNumber(other.getTrackNumber()),
		trackCount(other.getTrackCount()),
		duration(other.getDuration())
	{
	}

	/**
	 * @brief Retrieve the title of the media meta data
	 * @return Title of the media meta data
	 */
	std::string getTitle() const { return title; }

	/**
	 * @brief Retrieve the artist of the media meta data
	 * @return Artist of the media meta data
	 */
	std::string getArtist() const { return artist; }

	/**
	 * @brief Retrieve the album of the media meta data
	 * @return Album of the media meta data
	 */
	std::string getAlbum() const { return album; }

	/**
	 * @brief Retrieve the genre of the media meta data
	 * @return Genre of the media meta data
	 */
	std::string getGenre() const { return genre; }

	/**
	 * @brief Retrieve the track number of the media meta data
	 * @return Track number of the media meta data
	 */
	uint64_t getTrackNumber() const { return trackNumber; }

	/**
	 * @brief Retrieve the track count of the media meta data
	 * @return Track count of the media meta data
	 */
	uint64_t getTrackCount() const { return trackCount; }

	/**
	 * @brief Retrieve the duration of the media meta data
	 * @return Duration of the media meta data
	 */
	uint64_t getDuration() const { return duration; }

	/**
	 * @brief Set the title for media meta data
	 * @param title Title of the media meta data
	 */
	void setTitle(const std::string &title) { this->title = title; }

	/**
	 * @brief Set the artist for media meta data
	 * @param artist Artist of the media meta data
	 */
	void setArtist(const std::string &artist) { this->artist = artist; }

	/**
	 * @brief Set the album for media meta data
	 * @param album Album of the media meta data
	 */
	void setAlbum(const std::string &album) { this->album = album; }

	/**
	 * @brief Set the genre for media meta data
	 * @param genre Genre of the media meta data
	 */
	void setGenre(const std::string &genre) { this->genre = genre; }

	/**
	 * @brief Set the track number for media meta data
	 * @param tracknumber Track number of the media meta data
	 */
	void setTrackNumber(uint64_t trackNumber) { this->trackNumber = trackNumber; }

	/**
	 * @brief Set the track count for media meta data
	 * @param trackCount Track count of the media meta data
	 */
	void setTrackCount(uint64_t trackCount) { this->trackCount = trackCount; }

	/**
	 * @brief Set the duration for media meta data
	 * @param duration Duration of the media meta data
	 */
	void setDuration(uint64_t duration) { this->duration = duration; }

private:
	std::string title;
	std::string artist;
	std::string album;
	std::string genre;
	uint64_t trackNumber;
	uint64_t trackCount;
	uint64_t duration;
};

class BluetoothFolderItem
{
public:
	/**
	 * @brief Default constructor
	 */
	BluetoothFolderItem() :
	type(BluetoothAvrcpItemType::ITEM_TYPE_AUDIO),
	playable(false)
	{

	}

	/**
	 * @brief Copy constructor
	 */
	BluetoothFolderItem(const BluetoothFolderItem &other) :
	name(other.getName()),
	itemPath(other.getPath()),
	type(other.getType()),
	playable(other.getPlayable()),
	metadata(other.getMetadata())
	{}
	/* Get functions for the member variables */
	/**
	 * @brief Retrieve the item name
	 * @return Item name
	 */
	std::string getName() const { return name; }
	/**
	 * @brief Retrieve item path
	 * @return Item path
	 */
	std::string getPath() const { return itemPath; }
	/**
	 * @brief Retrieve item type
	 * @returns Item type
	 */
	BluetoothAvrcpItemType getType() const { return type; }
	/**
	 * @brief Retrieve metadata of the item. Metadata is avalid if the item
	 * type is Audio or Video.
	 * @return Metadata of the item
	 */
	BluetoothMediaMetaData getMetadata() const { return metadata; }
	/**
	 * @brief Retrieve if the item is playable
	 * @return true: Item is playable
	 *         false: Item is not playable
	 */
	bool getPlayable() const { return playable; }

	/* Set functions for the member variables */
	/**
	 * @brief Set item name
	 * @param name  Name of the item
	 */
	void setName(const std::string &name) { this->name = name; }
	/**
	 * @brief Set item path
	 * @param itemPath Item path
	 */
	void setPath(const std::string &itemPath) { this->itemPath = itemPath; }
	/**
	 * @brief Set item type
	 * @param type Item type
	 */
	void setType(const BluetoothAvrcpItemType &type) { this->type = type; }
	/**
	 * @brief Set item metadata
	 * @param metadata Metadata of the item
	 */
	void setMetadata(const BluetoothMediaMetaData &metadata) { this->metadata = metadata; }
	/**
	 * @brief Set if the item is playable
	 * @param playable true: Item is playable
	 *                 false: Item is not playable
	 */
	void setPlayable(const bool playable) { this->playable = playable; }

private:
	/* Displayable item name*/
	std::string name;
	/* Item path */
	std::string itemPath;
	/* Item type */
	BluetoothAvrcpItemType type;
	/* If the item is playable */
	bool playable;
	/* Metadata of the item. Available if the type is audio or video */
	BluetoothMediaMetaData metadata;
};

/**
 * @brief Bluetooth media play status
 *
 *        Describes media play status.
 */
class BluetoothMediaPlayStatus
{
public:
	/**
	 * @brief Media play status
	 */
	enum MediaPlayStatus
	{
		MEDIA_PLAYSTATUS_STOPPED = 0,
		MEDIA_PLAYSTATUS_PLAYING,
		MEDIA_PLAYSTATUS_PAUSED,
		MEDIA_PLAYSTATUS_FWD_SEEK,
		MEDIA_PLAYSTATUS_REV_SEEK,
		MEDIA_PLAYSTATUS_ERROR
	};

	/**
	 * @brief Default c'tor
	 */
	BluetoothMediaPlayStatus() :
		duration(0),
		position(0),
		status(MEDIA_PLAYSTATUS_STOPPED)
	{
	}

	/**
	 * @brief Copy c'tor
	 * @param other Other media play status object to copy from
	 */
	BluetoothMediaPlayStatus(const BluetoothMediaPlayStatus &other) :
		duration(other.getDuration()),
		position(other.getPosition()),
		status(other.getStatus())
	{
	}

	/**
	 * @brief Retrieve the duration of the media play status
	 * @return Duration of the media play status
	 */
	uint64_t getDuration() const { return duration; }

	/**
	 * @brief Retrieve the position of the media play status
	 * @return Position of the media play status
	 */
	uint64_t getPosition() const { return position; }

	/**
	 * @brief Retrieve the status of the media play status
	 * @return Status of the media play status
	 */
	MediaPlayStatus getStatus() const { return status; }

	/**
	 * @brief Set the duration for media play status
	 * @param duration Duration of the media play status
	 */
	void setDuration(uint64_t duration) { this->duration = duration; }

	/**
	 * @brief Set the position for media play status
	 * @param position Position of the media play status
	 */
	void setPosition(uint64_t position) { this->position = position; }

	/**
	 * @brief Set the status for media play status
	 * @param status Status of the media play status
	 */
	void setStatus(MediaPlayStatus status) { this->status = status; }

private:
	uint64_t duration;
	uint64_t position;
	MediaPlayStatus status;
};

class BluetoothPlayerApplicationSettingsProperty
{
public:
	enum Type
	{
		/**
		 * @brief Property does not contain any information.
		 **/
		EMPTY,
		/**
		* @brief Equalizer.
		*
		*        Type: Type: std::uint32_t (matches enum BluetoothPlayerApplicationSettingsEqualizer)
		**/
		EQUALIZER,
		/**
		* @brief Repeat
		*
		*        Type: Type: std::uint32_t (matches enum BluetoothPlayerApplicationSettingsRepeat)
		**/
		REPEAT,
		/**
		* @brief Shuffle
		*
		*        Type: Type: std::uint32_t (matches enum BluetoothPlayerApplicationSettingsShuffle)
		**/
		SHUFFLE,
		/**
		* @brief Scan
		*
		*        Type: Type: std::uint32_t (matches enum BluetoothPlayerApplicationSettingsScan)
		**/
		SCAN
	};

	/**
	 * @brief Initialize an empty property
	 *
	 *        The type of the property will be set to EMPTY.
	 */
	BluetoothPlayerApplicationSettingsProperty() :
		type(EMPTY),
		impl(0)
	{
	}

	/**
	 * @brief Initialize the property with a type
	 * @param type Type of the property
	 */
	BluetoothPlayerApplicationSettingsProperty(Type type) :
		type(type),
		impl(0)
	{
	}

	/**
	 * @brief Copy constructor
	 * @param other Other property to create a copy from
	 */
	BluetoothPlayerApplicationSettingsProperty(const BluetoothPlayerApplicationSettingsProperty &other) :
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
	BluetoothPlayerApplicationSettingsProperty(Type type, T value) :
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

/**
 * @brief List of bluetooth player application settings.
 */
typedef std::vector<BluetoothPlayerApplicationSettingsProperty> BluetoothPlayerApplicationSettingsPropertiesList;

/**
 * @brief List of bluetooth avrcp supported notification event.
 */
typedef std::vector<BluetoothAvrcpSupportedNotificationEvent> BluetoothAvrcpSupportedNotificationEventList;

/**
 * @brief List of items in the current browsing folder.
 */
typedef std::list<BluetoothFolderItem>  BluetoothFolderItemList;

/**
 * @brief List of available players received from AVRCP TG
 */
typedef std::list<BluetoothPlayerInfo> BluetothPlayerInfoList;


/**
 * @brief Callback to return a list of properties asynchronously.
 */
typedef std::function<void(BluetoothError, const BluetoothPlayerApplicationSettingsPropertiesList &)>
BluetoothPlayerApplicationSettingsPropertiesResultCallback;


/**
 * @brief Callback to return a single property asynchronously.
 */
typedef std::function<void(BluetoothError, const BluetoothPlayerApplicationSettingsProperty &prop)>
BluetoothPlayerApplicationSettingsPropertyResultCallback;

/**
 * @brief Callback to return the total number of items in the current folder asynchronously.
 */
typedef std::function<void(BluetoothError, const uint32_t numberOfItems)>
BluetoothAvrcpBrowseTotalNumberOfItemsCallback;

/**
 * @brief Callback to return the search result path asynchronously.
 */
typedef std::function<void(BluetoothError, const std::string searchListPath)>
BluetoothAvrcpBrowseSearchListCallback;

/**
 * @brief Callback to return the items in the current folder asynchronously.
 */
typedef std::function<void(BluetoothError, const BluetoothFolderItemList &folderItems)>
BluetoothAvrcpBrowseFolderItemsCallback;

/**
 * @brief This interface is the base to implement an observer for the Bluetooth
 *        AVRCP profile to get notifications from the profile when something has changed.
 */
class BluetoothAvrcpStatusObserver
{
public:
	/**
	 * @brief This method is called when a remote device requests for media meta data.
	 *
	 * @param requestId Id of the media meta data request
	 * @param address Address of remote device
	 */
	virtual void mediaMetaDataRequested(BluetoothAvrcpRequestId requestId, const std::string &address) {}

	/**
	 * @brief This method is called when a remote device requests for media play status.
	 *
	 * @param requestId Id of the media play status request
	 * @param address Address of remote device
	 */
	virtual void mediaPlayStatusRequested(BluetoothAvrcpRequestId requestId, const std::string &address) {}

	/**
	 * @brief This method is called when a local device(CT) receives media meta data from a remote device(TG).
	 *
	 * @param metaData Meta data of the media
	 * @param address Address of remote device
	 */
	virtual void mediaDataReceived(const BluetoothMediaMetaData &metaData, const std::string &address) {}

	/**
	 * @brief This method is called when a local device(CT) receives media meta data from a remote device(TG).
	 *
	 * @param metaData Meta data of the media
	 * @param adapterAddress Adapater address of the local device
	 * @param address Address of remote device
	 */
	virtual void mediaDataReceived(const BluetoothMediaMetaData &metaData, const std::string &adapterAddress,
		const std::string &address) {}

	/**
	 * @brief This method is called when a local device(CT) receives media play status from a remote device(TG).
	 *
	 * @param playStatus Play status of the media
	 * @param address Address of remote device
	 */
	virtual void mediaPlayStatusReceived(const BluetoothMediaPlayStatus &playStatus, const std::string &address) {}

	/**
	 * @brief This method is called when a local device(CT) receives media play status from a remote device(TG).
	 *
	 * @param playStatus Play status of the media
	 * @param adapterAddress Adapater address of the local device
	 * @param address Address of remote device
	 */
	virtual void mediaPlayStatusReceived(const BluetoothMediaPlayStatus &playStatus, const std::string &adapterAddress,
		const std::string &address) {}

	/**
	 * @brief This method is called when the volume has been changed locally on the TG, or what the actual volume
	 *        level is following use of relative volume commands.
	 *
	 * @param volume The changed volume level, range 0 ~ 127.
	 * @param address Address of remote device
	 */
	virtual void volumeChanged(int volume, const std::string &adapterAddress ,const std::string &address) {}

	/**
	 * @brief This method is called when a local device receives PASS THROUGH command from a remote device.
	 *        from a remote device.
	 *
	 * @param features The remote features
	 * @param adapterAddress address of adapter
	 * @param address Address of remote device
	 */
	virtual void passThroughCommandReceived(BluetoothAvrcpPassThroughKeyCode keyCode, BluetoothAvrcpPassThroughKeyStatus keyStatus,
											const std::string &adapterAddress, const std::string &address) {}

	/**
	 * @brief This method is called when a local device is connected with a remote device and receives remote features
	 *        from a remote device.
	 *
	 * @param features The remote feautures
	 * @param address Address of remote device
	 */
	virtual void remoteFeaturesReceived(BluetoothAvrcpRemoteFeatures features, const std::string &address) {}

	/**
	 * @brief This method is called when a local device is connected with a remote device and receives remote features
	 *        from a remote device.
	 *
	 * @param features The remote feautures
	 * @param address Address of remote device
	 * @param role Role of remote device. Should be one of "CT" or "TG".(CT = Controller, TG = Target)
	 */
	virtual void remoteFeaturesReceived(BluetoothAvrcpRemoteFeatures features, const std::string &address, const std::string &role) {}

	/**
	 * @brief This method is called when a local device is connected with a remote device and receives remote features
	 * 	from a remote device.
	 * @param features The remote feautures
	 * @param adapterAddress Address of adapter
	 * @param address Address of remote device
	 * @param role Role of remote device. Should be one of "CT" or "TG".(CT = Controller, TG = Target)
	 */

	virtual void remoteFeaturesReceived(BluetoothAvrcpRemoteFeatures features, const std::string &adapterAddress, const std::string &address, const std::string &role) {}
	/**
	 * @brief This method is called when a local device is connected with a remote device and receives supported notification events
	 *        from a remote device.
	 *
	 * @param events List of supported notification events.
	 * @param address Address of remote device
	 */
	virtual void supportedNotificationEventsReceived(const BluetoothAvrcpSupportedNotificationEventList &events, const std::string &address) {}

	/**
	 * @brief This method is called when a local device is connected with a remote device and receives supported notification events
	 *        from a remote device.
	 *
	 * @param events List of supported notification events.
	 * @param adapterAddress Address of adapter
	 * @param address Address of remote device
	 */
	virtual void supportedNotificationEventsReceived(const BluetoothAvrcpSupportedNotificationEventList &events,
			const std::string &adapterAddress, const std::string &address) {}

	/**
	 * @brief This method is called when the local device (CT) receives the player application settings from remote device (TG)
	 *
	 * @param properties Player application properties changed
	 * @param adapterAddress Adapater address of the local device
	 * @param address Address of remote device
	 */
	virtual void playerApplicationSettingsReceived(const BluetoothPlayerApplicationSettingsPropertiesList &properties,
		const std::string &adapterAddress, const std::string &address) {};

	/**
	 * @brief This method is called when local device (CT) receives the player list from remote device (TG)
	 *
	 * @param playerInfoList Player information
	 * @param adapterAddress Adapater address of the local device
	 * @param address Address of remote device
	 */
	virtual void playerInfoReceived(const BluetothPlayerInfoList &playerInfoList, const std::string &adapterAddress, const std::string &address) {};

	/**
	 * @brief This method is called whenever browsing folder changes.
	 *
	 * @param currentFolder Current browsing folder
	 * @param adapterAddress Adapater address of the local device
	 * @param address Address of remote device
	 */
	virtual void currentFolderReceived(const std::string currentFolder,
									   const std::string &adapterAddress, const std::string &address){};
};

/**
 * @brief Interface to abstract the operations for the AVRCP bluetooth profile.
 */
class BluetoothAvrcpProfile
{
public:
	BluetoothAvrcpProfile() { }

	virtual ~BluetoothAvrcpProfile() { }

	/**
	 * @brief Register a observer for the AVRCP profile. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 * @param observer Observer instance to register.
	 */
	void registerObserver(BluetoothAvrcpStatusObserver *observer)
	{
		this->observer = observer;
	}

	/**
	 * @brief This method is called when TG(target) sends media meta data to CT(controller).
	 *
	 * @param requestId Id of the media meta data request
	 * @param metaData Meta data of the media
	 */
	virtual void supplyMediaMetaData(BluetoothAvrcpRequestId requestId, const BluetoothMediaMetaData &metaData, BluetoothResultCallback callback) = 0;

	/**
	 * @brief This method is called when TG(target) sends media play status to CT(controller).
	 *
	 * @param requestId Id of the media play status request
	 * @param playStatus Play status of the media
	 */
	virtual void supplyMediaPlayStatus(BluetoothAvrcpRequestId requestId, const BluetoothMediaPlayStatus &playStatus, BluetoothResultCallback callback) = 0;

#ifdef BTMNGR_SUPPORT_AV_FEAT_RCCT
	/**
	 * @brief This method is called to send remote command.
	 *
	 * @param address Address of remote device
	 * @param key Key operated
	 * @param value Key status(true for pressed, false for released) or Key value
	 */
	virtual BluetoothError sendRemoteCommand(const std::string &address, int key, int value) = 0;
#endif

	/**
	 * @brief This method is called to send PASS THROUGH command.
	 *
	 * @param address Address of remote device
	 * @param keyCode Key code of PASS THROUGH command
	 * @param keyStatus Key status of PASS THROUGH command
	 */

	virtual BluetoothError sendPassThroughCommand(const std::string &address, BluetoothAvrcpPassThroughKeyCode keyCode, BluetoothAvrcpPassThroughKeyStatus keyStatus) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Perform a request to the SIL to retrieve all properties of the Bluetooth
	 *        player application settings. The result of the operation is handed back with the supplied
	 *        callback function.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after getPlayerApplicationSettingsProperties has returned to the caller
	 *        or before.
	 *
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void getPlayerApplicationSettingsProperties(BluetoothPlayerApplicationSettingsPropertiesResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, std::vector<BluetoothPlayerApplicationSettingsProperty>());
	}

	/**
	 * @brief Retrieve a specific property of the Bluetooth player application settings.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after getPlayerApplicationSettingsProperty has returned to the caller
	 *        or before.
	 *
	 * @param type Property of the player application settings to retrieve from the SIL.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void getPlayerApplicationSettingsProperty(BluetoothPlayerApplicationSettingsProperty::Type type,
													BluetoothPlayerApplicationSettingsPropertyResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED, BluetoothPlayerApplicationSettingsProperty(BluetoothPlayerApplicationSettingsProperty::Type::EMPTY));
	}

	/**
	 * @brief Set a specific property of the Bluetooth player application settings.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after setPlayerApplicationSettingsProperty has returned to the caller
	 *        or before.
	 *
	 * @param property Property which should be set.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void setPlayerApplicationSettingsProperty(const BluetoothPlayerApplicationSettingsProperty &property,
													BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Set one or more properties of the Bluetooth player application settings.
	 *
	 *        NOTE: As this is a asynchronous operation the callback function can
	 *        be called either after setPlayerApplicationSettingsPropertiess has returned to the caller
	 *        or before.
	 *
	 * @param properties Properties which should be set.
	 * @param callback Callback function which is called when the operation is done or
	 *        has failed.
	 */
	virtual void setPlayerApplicationSettingsProperties(const BluetoothPlayerApplicationSettingsPropertiesList &properties,
													BluetoothResultCallback callback)
	{
		if (callback) callback(BLUETOOTH_ERROR_UNSUPPORTED);
	}

	/**
	 * @brief Set the absolute volume on the target device.
	 *
	 * @param address Address of remote device
	 * @param volume The requested volume level in percentage, range 0 ~ 127.
	 */
	virtual BluetoothError setAbsoluteVolume(const std::string &address, int volume) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * Browsing APIs
	 */
	/**
	 * @brief Gets the total number of items in the current folder
	 *
	 * @param callback Callback function which is called to return number of items asynchronously in case of
	 *                 success or to inform the error when operation is failed.
	 */
	virtual void getNumberOfItems(BluetoothAvrcpBrowseTotalNumberOfItemsCallback callback)
	{
		if (callback)
			callback(BLUETOOTH_ERROR_UNSUPPORTED, 0);
	}
	/**
	 * @brief Gets the items in the current folder
	 *
	 * @param startIndex The offset within the listing of the item, which
	 *                   should be the first returned item.
	 *                   The first element in the listing is at offset 0
	 * @param endIndex The offset within the listing of the item, which should
	 *                 be the final returned item. endIndex shoul be greater
	 *                 than startIndex and
	 *                 less than total number of items in the current folder
	 * @param callback Callback function which is called to return the items asynchronously in case of
	 *                 success or to inform the error when operation is failed. The list items sent in
	 *                 the callback will be freed once the callback returns.
	 */
	virtual void getFolderItems(uint32_t startIndex, uint32_t endIndex,
								BluetoothAvrcpBrowseFolderItemsCallback callback)
	{
		BluetoothFolderItemList folderItems;
		if (callback)
			callback(BLUETOOTH_ERROR_UNSUPPORTED, folderItems);
	}
	/**
	 * @brief Issues command to play the browsed item
	 *
	 * @param itemPath Object path of the audio/video item to play
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_AVRCP_ITEM_NOT_PLAYABLE
	 *                          BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 *                          BLUETOOTH_ERROR_NOT_ALLOWED
	 */
	virtual BluetoothError playItem(const std::string &itemPath) { return BLUETOOTH_ERROR_UNSUPPORTED; }

	/**
	 * @brief Changes the current folder for browsing. This API can be called
	 * with path of the folder to change to, that is one level up or down to
	 * the current folder
	 *
	 * @param itemPath Object path of the folder item to change to
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_AVRCP_NOT_A_FOLDER
	 *                          BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 *                          BLUETOOTH_ERROR_NOT_ALLOWED
	 */
	virtual BluetoothError changePath(const std::string &itemPath) { return BLUETOOTH_ERROR_UNSUPPORTED; }
	/**
	 * @brief Adds the playable item to the now playing list.
	 *
	 * @param itemPath Object path playable audio/video item to add to now
	 * playing list
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_AVRCP_ITEM_NOT_PLAYABLE
	 *                          BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 *                          BLUETOOTH_ERROR_NOT_ALLOWED
	 */
	virtual BluetoothError addToNowPlaying(const std::string &itemPath) { return BLUETOOTH_ERROR_UNSUPPORTED; }
	/**
	 * @brief Searches the searchString in the current folder and its subfolders
	 *
	 * @param searchString String to search
	 * @param callback Callback function which is called to return the path to the search list on success
	 *                 or to return error in case of failure.
	 */
	virtual void search(const std::string &searchString,
						BluetoothAvrcpBrowseSearchListCallback callback)
	{
		if (callback)
			callback(BLUETOOTH_ERROR_UNSUPPORTED, "");
	}

protected:
	/**
	 * @brief Retrieve the AVRCP status observer
	 *
	 *        If no observer was registered with registerObserver before a NULL object
	 *        instance will be returned which is usable but will doing nothing when
	 *        called.
	 *
	 * @return The AVRCP status observer
	 */
	BluetoothAvrcpStatusObserver* getAvrcpObserver()
	{
		static BluetoothAvrcpStatusObserver nullObserver;

		if (!observer)
			return &nullObserver;

		return observer;
	}

private:
	BluetoothAvrcpStatusObserver *observer;
};

#endif
