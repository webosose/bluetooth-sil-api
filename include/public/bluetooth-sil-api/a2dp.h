// Copyright (c) 2015-2019 LG Electronics, Inc.
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

#ifndef BLUETOOTH_SIL_A2DP_H_
#define BLUETOOTH_SIL_A2DP_H_

#ifndef BLUETOOTH_SIL_H_
	#error This header file should only be included by bluetooth-sil-api.h
#endif

const std::string BLUETOOTH_PROFILE_ID_A2DP = "A2DP";

/**
 * @brief Profile state of A2DP
 *
 */
enum BluetoothA2dpProfileState
{
	/** Audio streaming is in progress */
	PLAYING,
	/** Audio streaming is not in progress */
	NOT_PLAYING,
};

/**
 * @brief Profile state of A2DP
 *
 */
enum BluetoothA2dpAudioSocketType
{
	UNKNOWN,
	TCP,
	UDP,
};

/**
 * @brief Bluetooth SBC Codec Configuration
 *
 *        Describes SBC Codec Configuration.
 */
class BluetoothSbcConfiguration
{
public:
	/**
	 * @brief SBC Sample frequency.
	 */
	enum SampleFrequency
	{
		SAMPLE_FREQUENCY_UNKNOWN = 0,
		SAMPLE_FREQUENCY_16000 = (1 << 3),
		SAMPLE_FREQUENCY_32000 = (1 << 2),
		SAMPLE_FREQUENCY_44100 = (1 << 1),
		SAMPLE_FREQUENCY_48000 = 1
	};

	/**
	 * @brief SBC Channel mode.
	 */
	enum ChannelMode
	{
		CHANNEL_MODE_UNKNOWN = 0,
		CHANNEL_MODE_MONO = (1 << 3),
		CHANNEL_MODE_DUAL_CHANNEL = (1 << 2),
		CHANNEL_MODE_STEREO = (1 << 1),
		CHANNEL_MODE_JOINT_STEREO = 1
	};

	/**
	 * @brief SBC Block length.
	 */
	enum BlockLength
	{
		BLOCK_LENGTH_UNKNOWN = 0,
		BLOCK_LENGTH_4 = (1 << 3),
		BLOCK_LENGTH_8 = (1 << 2),
		BLOCK_LENGTH_12 = (1 << 1),
		BLOCK_LENGTH_16 = 1
	};

	/**
	 * @brief SBC Subbands.
	 */
	enum Subbands
	{
		SUBBANDS_UNKOWN = 0,
		SUBBANDS_4 = (1 << 1),
		SUBBANDS_8 = 1
	};

	/**
	 * @brief SBC Allocation method.
	 */
	enum AllocationMethod
	{
		ALLOCATION_METHOD_UNKNWON = 0,
		ALLOCATION_METHOD_SNR = (1 << 1),
		ALLOCATION_METHOD_LOUDNESS = 1
	};

	/**
	 * @brief Default c'tor
	 */
	BluetoothSbcConfiguration() :
		sampleFrequency(SAMPLE_FREQUENCY_UNKNOWN),
		channelMode(CHANNEL_MODE_UNKNOWN),
		blockLength(BLOCK_LENGTH_UNKNOWN),
		subbands(SUBBANDS_UNKOWN),
		allocationMethod(ALLOCATION_METHOD_UNKNWON),
		minBitpool(0),
		maxBitpool(0)
	{
	}

	/**
	 * @brief Copy c'tor
	 * @param other Other SBC Configuration object to copy from
	 */
	BluetoothSbcConfiguration(const BluetoothSbcConfiguration &other) :
		sampleFrequency(other.getSampleFrequency()),
		channelMode(other.getChannelMode()),
		blockLength(other.getBlockLength()),
		subbands(other.getSubbands()),
		allocationMethod(other.getAllocationMethod()),
		minBitpool(other.getMinBitpool()),
		maxBitpool(other.getMaxBitpool())
	{
	}

	/**
	 * @brief Retrieve the sample frequency of the SBC configuration
	 * @return Sample frequency of the SBC configuration
	 */
	SampleFrequency getSampleFrequency() const { return sampleFrequency; }

	/**
	 * @brief Retrieve the channel mode of the SBC configuration
	 * @return Channel mode of the SBC configuration
	 */
	ChannelMode getChannelMode() const { return channelMode; }

	/**
	 * @brief Retrieve the block length of the SBC configuration
	 * @return Block length of the SBC configuration
	 */
	BlockLength getBlockLength() const { return blockLength; }

	/**
	 * @brief Retrieve the subbands of the SBC configuration
	 * @return Subbands of the SBC configuration
	 */
	Subbands getSubbands() const { return subbands; }

	/**
	 * @brief Retrieve the allocation method of the SBC configuration
	 * @return Allocation method of the SBC configuration
	 */
	AllocationMethod getAllocationMethod() const { return allocationMethod; }

	/**
	 * @brief Retrieve the minimum bitpool of the SBC configuration
	 * @return Minimum bitpool of the SBC configuration
	 */
	uint8_t getMinBitpool() const { return minBitpool; }

	/**
	 * @brief Retrieve the maximum bitpool of the SBC configuration
	 * @return Maximum bitpool of the SBC configuration
	 */
	uint8_t getMaxBitpool() const { return maxBitpool; }

	/**
	 * @brief Set the sample frequency of the SBC configuration
	 * @param Sample frequency of the SBC configuration
	 */
	void setSampleFrequency(SampleFrequency sampleFrequency) { this->sampleFrequency = sampleFrequency; }

	/**
	 * @brief Set the channel mode of the SBC configuration
	 * @param Channel mode of the SBC configuration
	 */
	void setChannelMode(ChannelMode channelMode) { this->channelMode = channelMode; }

	/**
	 * @brief Set the block length of the SBC configuration
	 * @param Block length of the SBC configuration
	 */
	void setBlockLength(BlockLength blockLength) { this->blockLength = blockLength; }

	/**
	 * @brief Set the subbands of the SBC configuration
	 * @param Subbands of the SBC configuration
	 */
	void setSubbands(Subbands subbands) { this->subbands = subbands;}

	/**
	 * @brief Set the allocation method of the SBC configuration
	 * @param Allocation method of the SBC configuration
	 */
	void setAllocationMethod(AllocationMethod allocationMethod) { this->allocationMethod = allocationMethod; }

	/**
	 * @brief Set the minimum bitpool of the SBC configuration
	 * @param Minimum bitpool of the SBC configuration
	 */
	void setMinBitpool(uint8_t minBitpool) { this->minBitpool = minBitpool; }

	/**
	 * @brief Set the maximum bitpool of the SBC configuration
	 * @param Maximum bitpool of the SBC configuration
	 */
	void setMaxBitpool(uint8_t maxBitpool) { this->maxBitpool = maxBitpool; }

private:
	SampleFrequency sampleFrequency;
	ChannelMode channelMode;
	BlockLength blockLength;
	Subbands subbands;
	AllocationMethod allocationMethod;
	uint8_t minBitpool;
	uint8_t maxBitpool;
};

/**
 * @brief Bluetooth SBC Codec Configuration
 *
 *        Describes SBC Codec Configuration.
 */
class BluetoothAptxConfiguration
{
public:
	/**
	 * @brief aptX Sample frequency.
	 */
	enum SampleFrequency
	{
		SAMPLE_FREQUENCY_UNKNOWN = 0,
		SAMPLE_FREQUENCY_16000 = 0x08,
		SAMPLE_FREQUENCY_32000 = 0x04,
		SAMPLE_FREQUENCY_44100 = 0x02,
		SAMPLE_FREQUENCY_48000 = 0x01
	};

	/**
	 * @brief SBC Channel mode.
	 */
	enum ChannelMode
	{
		CHANNEL_MODE_UNKNOWN = 0,
		CHANNEL_MODE_MONO = 0x01,
		CHANNEL_MODE_STEREO = 0x02
	};

	/**
	 * @brief Default c'tor
	 */
	BluetoothAptxConfiguration() :
		sampleFrequency(SAMPLE_FREQUENCY_UNKNOWN),
		channelMode(CHANNEL_MODE_UNKNOWN)
	{
	}

	/**
	 * @brief Copy c'tor
	 * @param other Other aptX Configuration object to copy from
	 */
	BluetoothAptxConfiguration(const BluetoothAptxConfiguration &other) :
		sampleFrequency(other.getSampleFrequency()),
		channelMode(other.getChannelMode())
	{
	}

	/**
	 * @brief Retrieve the sample frequency of the aptX configuration
	 * @return Sample frequency of the aptX configuration
	 */
	SampleFrequency getSampleFrequency() const { return sampleFrequency; }

	/**
	 * @brief Retrieve the channel mode of the aptX configuration
	 * @return Channel mode of the aptX configuration
	 */
	ChannelMode getChannelMode() const { return channelMode; }

	/**
	 * @brief Set the sample frequency of the aptX configuration
	 * @param Sample frequency of the aptX configuration
	 */
	void setSampleFrequency(SampleFrequency sampleFrequency) { this->sampleFrequency = sampleFrequency; }

	/**
	 * @brief Set the channel mode of the aptX configuration
	 * @param Channel mode of the aptX configuration
	 */
	void setChannelMode(ChannelMode channelMode) { this->channelMode = channelMode; }

private:
	SampleFrequency sampleFrequency;
	ChannelMode channelMode;
};

/**
 * @brief This interface is the base to implement an observer for the A2DP profile.
 */
class BluetoothA2dpStatusObserver
{
public:
	/**
	 * @brief The method is called when the profile state is changed
	 *
	 * @param address Address of the device
	 * @param state current playing state of the A2DP profile.
	*/
	virtual void stateChanged(std::string address, BluetoothA2dpProfileState state) { }

	/**
	 * @brief The method is called when the profile state is changed
	 * @param adapterAddress address of adapter
	 * @param address Address of the device
	 * @param state current playing state of the A2DP profile.
	*/
	virtual void stateChanged(std::string adapterAddress, std::string address, BluetoothA2dpProfileState state) { }

	/**
	 * @brief The method is called when A2DP audio socket is created
	 *
	 * @param address Address of the remote device
	 * @param path Audio socket path
	 * @param type Audio socket type
	 * @param isIn Whether audio stream is in or out from remote device to local device
	*/
	virtual void audioSocketCreated(const std::string &address, const std::string &path, BluetoothA2dpAudioSocketType type, bool isIn) { }

	/**
	 * @brief The method is called when A2DP audio socket is destroyed
	 *
	 * @param address Address of the remote device
	 * @param path Audio socket path
	 * @param type Audio socket type
	 * @param isIn Whether audio stream is in or out from remote device to local device
	*/
	virtual void audioSocketDestroyed(const std::string &address, const std::string &path, BluetoothA2dpAudioSocketType type, bool isIn) { }

	/**
	 * @brief The method is called when A2DP SBC Configuration is changed.
	 *
	 * @param address Address of the remote device
	 * @param sbcConfiguration SBC configuration
	*/
	virtual void sbcConfigurationChanged(const std::string &address, const BluetoothSbcConfiguration &sbcConfiguration) { }

	/**
	 * @brief The method is called when A2DP aptX Configuration is changed.
	 *
	 * @param address Address of the remote device
	 * @param sbcConfiguration aptX configuration
	*/
	virtual void aptxConfigurationChanged(const std::string &address, const BluetoothAptxConfiguration &aptxConfiguration) { }
};

/**
 * @brief Interface to abstract the operations for the A2DP bluetooth profile.
 */
class BluetoothA2dpProfile
{
public:
	BluetoothA2dpProfile() : observer(0) { }

	virtual ~BluetoothA2dpProfile() { }

	/**
	 * @brief Register a observer for the A2DP profile. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 * @param observer Observer instance to register.
	 */
	void registerObserver(BluetoothA2dpStatusObserver *observer)
	{
		this->observer = observer;
	}

	/**
	 * @brief Start audio streaming via A2DP
	 *
	 * @param Address of the device
	 * @return BLUETOOTH_ERROR_NONE when operation was successful another error code otherwise
	 */
	virtual BluetoothError startStreaming(const std::string &address) = 0;

	/**
	 * @brief Stop audio streaming via A2DP
	 *
	 * @param Address of the device
	 * @return BLUETOOTH_ERROR_NONE when operation was successful another error code otherwise
	 */
	virtual BluetoothError stopStreaming(const std::string &address) = 0;

	/**
	 * @brief Stop audio streaming via A2DP
	 *
	 * @param address Address of the device
	 * @param bitpool Bitpool of SBC Encoder Settings
	 * @return BLUETOOTH_ERROR_NONE when operation was successful another error code otherwise
	 */
	virtual BluetoothError setSbcEncoderBitpool(const std::string &address, uint8_t bitpool) { return BLUETOOTH_ERROR_UNSUPPORTED; }

protected:
	/**
	 * @brief Retrieve the A2DP status observer
	 *
	 *        If no observer was registered with registerObserver before, a NULL object
	 *        instance will be returned which is usable but will do nothing when
	 *        called.
	 *
	 * @return The A2DP status observer
	 */
	BluetoothA2dpStatusObserver *getA2dpObserver()
	{
		static BluetoothA2dpStatusObserver nullObserver;

		if (!observer)
			return &nullObserver;

		return observer;
	}

private:
	BluetoothA2dpStatusObserver *observer;
};

#endif
