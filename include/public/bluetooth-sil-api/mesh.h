// Copyright (c) 2021 LG Electronics, Inc.
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

#ifndef BLUETOOTH_SIL_MESH_H_
#define BLUETOOTH_SIL_MESH_H_

#ifndef BLUETOOTH_SIL_H_
#error This header file should only be included by bluetooth-sil-api.h
#endif //BLUETOOTH_SIL_H_

const std::string BLUETOOTH_PROFILE_ID_MESH = "MESH";

/**
 * @brief Class representing the network key. This information is provided when
 * mesh info is retrieved.
 */
class BleMeshNetKeys
{
public:
	uint16_t getIndex() const { return index; }
	bool getKeyRefresh() const { return keyRefresh; }

	void setIndex(uint16_t index) { this->index = index; }
	void setKeyRefresh(bool keyRefresh) { this->keyRefresh = keyRefresh; }
private:
	/** @brief Network key index */
	uint16_t index;
	/** @brief When true, indicates that the specified net key
	 * is in the middle of a key refresh procedure.
	 */
	bool keyRefresh;
};

/**
 * @brief Class representing the application key. This information is provided when
 * mesh info is retrieved.
 */
class BleMeshAppKeys
{
public:
	/* Accesssor and modifier functions for private variables */
	uint16_t getIndex() const { return index; }
	uint16_t getBoundNetKeyIndex() const { return boundNetKeyIndex; }
	void setIndex(uint16_t index) { this->index = index; }
	void setBoundNetKeyIndex(uint16_t boundNetKeyIndex) { this->boundNetKeyIndex = boundNetKeyIndex; }
private:
	/** @brief Application key index */
	uint16_t index;
	/** @brief Network key index to which the app key is bound to */
	uint16_t boundNetKeyIndex;
};

/**
 * @brief Class representing the provisioner information. This information is
 * provided when mesh info is retrieved
 */
class BleMeshProvisioner
{
public:
	/* Accesssor and modifier functions for private variables */
	std::string getName() const { return name; }
	uint16_t getUnicastAddress() const { return unicastAddress; }
	uint16_t getLowAddress() const { return lowAddress; }
	uint16_t getHighAddress() const { return highAddress; }
	void setName(const std::string &name) { this->name = name; }
	void setUnicastAddress(uint16_t unicastAddress) { this->unicastAddress = unicastAddress; }
	void setLowAddress(uint16_t lowAddress) { this->lowAddress = lowAddress; }
	void setHighAddress(uint16_t highAddress) { this->highAddress = highAddress; }
private:
	/** @brief Name of the provisioner */
	std::string name;
	/** @brief Unicast address of the provisioner */
	uint16_t unicastAddress;
	/** @brief Lowest unicast address in the address range that provisioner will
	 * assign to the mesh nodes
	 */
	uint16_t lowAddress;
	/** @brief Highest unicast address in the address range that provisioner will
	 * assign to the mesh nodes
	 */
	uint16_t highAddress;
};

/**
 * @brief Class representing the mesh network information. This information is
 * provided when mesh info is retrieved
 */
class BleMeshInfo
{
public:
	BleMeshInfo() : ivIndex(0) { }
	/* Accesssor and modifier functions for private variables */
	std::string getMeshName() const { return meshName; }
	uint32_t getIvIndex() const { return ivIndex; }
	std::vector<BleMeshNetKeys> getNetKeys() const { return netKeys; }
	std::vector<BleMeshAppKeys> getAppKeys() const { return appKeys; }
	std::vector<BleMeshProvisioner> getProvisioners() const { return provisioners; }

	void setMeshName(const std::string &meshName) { this->meshName = meshName; }
	void setIvIndex(uint32_t ivIndex) { this->ivIndex = ivIndex; }
	void setNetKeys(std::vector<BleMeshNetKeys> &netKeys) { this->netKeys = netKeys; }
	void setAppKeys(std::vector<BleMeshAppKeys> &appKeys) { this->appKeys = appKeys; }
	void setProvisioners(std::vector<BleMeshProvisioner> &appKeys) { this->provisioners = provisioners; }
private:
	/** @brief Mesh network name */
	std::string meshName;
	/** @brief The IV Index is a 32-bit value that is a shared network resource
	 * It starts with 0x00000000. The IV Index provides entropy for the nonce used
	 * for the authenticated encryption (AES-CCM) in both the application and
	 * network layers. Therefore, it must be changed often enough to avoid
	 * repeated use of sequence numbers in the nonce.
	 */
	uint32_t ivIndex;
	/** @brief Available network key indexes and corresponding information */
	std::vector<BleMeshNetKeys> netKeys;
	/** @brief Available application key indexes and corresponding information  */
	std::vector<BleMeshAppKeys> appKeys;
	/** @brief Provisioner information */
	std::vector<BleMeshProvisioner> provisioners;
};

/**
 * @brief Class representing the features supported by a node
 */
class BleMeshFeature
{
public:
	BleMeshFeature() :
	relaySupport(0),
	proxySupport(0),
	friendSupport(0),
	lowPowerSupport(0)
	{}
	BleMeshFeature(bool relaySupport, bool proxySupport, bool friendSupport,
					bool lowPowerSupport)
	{
		this->relaySupport = relaySupport;
		this->proxySupport = proxySupport;
		this->friendSupport = friendSupport;
		this->lowPowerSupport = lowPowerSupport;
	}
	/* Accesssor and modifier functions for private variables */
	bool getRelaySupport() const { return relaySupport; }
	bool getProxySupport() const { return proxySupport; }
	bool getFriendSupport() const { return friendSupport; }
	bool getLowPowerSupport() const { return lowPowerSupport; }

	void setRelaySupport(bool relaySupport) { this->relaySupport = relaySupport; }
	void setProxySupport(bool proxySupport) { this->proxySupport = proxySupport; }
	void setFriendSupport(bool friendSupport) { this->friendSupport = friendSupport; }
	void setLowPowerSupport(bool lowPowerSupport) { this->lowPowerSupport = lowPowerSupport; }

private:
	/** @brief If node supports relay feature */
	bool relaySupport;
	/** @brief If node supports proxy feature */
	bool proxySupport;
	/** @brief If node supports friend feature */
	bool friendSupport;
	/** @brief If node supports low power feature */
	bool lowPowerSupport;
};

/**
 * @brief Class representing the Relay status of a node
 */
class BleMeshRelayStatus
{
public:
	BleMeshRelayStatus() :
	relay(0),
	relayRetransmitCount(0),
	relayRetransmitIntervalSteps(0)
	{}
	/* Accesssor and modifier functions for private variables */
	uint8_t getRelay() const { return relay; }
	uint8_t getrelayRetransmitCount() const { return relayRetransmitCount; }
	uint8_t getRelayRetransmitIntervalSteps() const { return relayRetransmitIntervalSteps; }

	void setRelay(uint8_t relay) { this->relay = relay; }
	void setRelayRetransmitCount(uint8_t relayRetransmitCount)
	{
		this->relayRetransmitCount = relayRetransmitCount;
	}
	void setRelayRetransmitIntervalSteps(uint8_t relayRetransmitIntervalSteps)
	{
		this->relayRetransmitIntervalSteps = relayRetransmitIntervalSteps;
	}

private:
	/** @brief Current Relay state for the node
	 * Values can be, 0x00 - The node support Relay feature that is disabled
	 * 0x01 - The node supports Relay feature that is enabled
	 * 0x02 - Relay feature is not supported
	 */
	uint8_t relay;
	/** @brief Number of retransmissions on advertising bearer for each Network
	 * PDU relayed by the node
	 */
	uint8_t relayRetransmitCount;
	/** @brief Number of 10-millisecond steps between retransmissions */
	uint8_t relayRetransmitIntervalSteps;
};

/** @brief Class representing the Element in a node. Element is an addressable
 * entity within a device. A device is required to have at least one element.
 */
class BleMeshElement
{
public:
	BleMeshElement() :
	loc(0),
	numS(0),
	numV(0)
	{}

	BleMeshElement(uint16_t loc, uint8_t numS, std::vector<uint32_t> &sigModelIds,
					uint8_t numV, std::vector<uint32_t> &vendorModelIds)
	{
		this->loc = loc;
		this->numS = numS;
		this->sigModelIds = sigModelIds;
		this->numV = numV;
		this->vendorModelIds = vendorModelIds;
	}
	/* Accesssor and modifier functions for private variables */
	uint16_t getLoc() const { return loc; }
	uint16_t getNumS() const { return numS; }
	std::vector<uint32_t> getSigModelIds() const { return sigModelIds; }
	uint16_t getNumV() const { return numV; }
	std::vector<uint32_t> getVendorModelIds() const { return vendorModelIds; }

	void setLoc(uint16_t loc) { this->loc = loc; }
	void setNumS(uint8_t numS) { this->numS = numS; }
	void setSigModelIds(std::vector<uint32_t> &sigModelIds) { this->sigModelIds = sigModelIds; }
	void setNumV(uint8_t numV) { this->numV = numV; }
	void setVendorModelIds(std::vector<uint32_t> &vendorModelIds) { this->vendorModelIds = vendorModelIds; }

private:
	/** @brief Location descriptor */
	uint16_t loc;
	/** @brief Number of SIG Model IDs in this element */
	uint8_t numS;
	/** @brief SIG Model IDs */
	std::vector<uint32_t> sigModelIds;
	/** @brief Number of Vendor Model IDs in this element */
	uint8_t numV;
	/** @brief Vendor Model IDs */
	std::vector<uint32_t> vendorModelIds;
};

/** @brief Class representing the composition data of a node,
 * Contains information about a node, the elements it includes, and the supported models
 * This information is provided when getCompositionData API is called.
 */
class BleMeshCompositionData
{
public:
	BleMeshCompositionData() :
	companyId(0),
	productId(0),
	versionId(0),
	numRplEnteries(0)
	{}
	BleMeshCompositionData(uint16_t companyId, uint16_t productId, uint16_t versionId,
							uint16_t numRplEnteries, BleMeshFeature &features,
							std::vector<BleMeshElement> &elements)
	{
		this->companyId = companyId;
		this->productId = productId;
		this->versionId = versionId;
		this->numRplEnteries = numRplEnteries;
		this->features = features;
		this->elements = elements;
	}
	/* Accesssor and modifier functions for private variables */
	uint16_t getCompanyId() const { return companyId; }
	uint16_t getProductId() const { return productId; }
	uint16_t getVersionId() const { return versionId; }
	uint16_t getNumRplEnteries() const { return numRplEnteries; }
	BleMeshFeature getFeatures() const { return features; }
	std::vector<BleMeshElement> getElements() const { return elements; }

	void setCompanyId(uint16_t cid) { this->companyId = cid; }
	void setProductId(uint16_t pid) { this->productId = pid; }
	void setVersionId(uint16_t vid) { this->versionId = vid; }
	void setNumRplEnteries(uint16_t crpl) { this->numRplEnteries = crpl; }
	void setFeatures(BleMeshFeature &features) { this->features = features; }
	void setElements(std::vector<BleMeshElement> &elements) { this->elements = elements; }

private:
	/** @brief Company identifier assigned by the Bluetooth SIG */
	uint16_t companyId;
	/** @brief Vendor-assigned identifier */
	uint16_t productId;
	/** @brief Vendor-assigned version identifier */
	uint16_t versionId;
	/** @brief Minimum number of replay protection list entries in a device */
	uint16_t numRplEnteries;
	/** @brief Contains information indicating the device features */
	BleMeshFeature features;
	/** @brief Contains a sequence of element descriptions */
	std::vector<BleMeshElement> elements;
};

/**
 * @brief Class representing the configuration data that can be retrieved from a node
 */
class BleMeshConfiguration
{
public:

	/* Accesssor and modifier functions for private variables */
	std::string getConfig() const { return config; }
	std::vector<uint16_t> getAppKeyIndexes() const { return appKeyIndexes; }
	uint8_t getGattProxyState() const { return gattProxyState; }
	uint8_t getTTL() const { return ttl; }
	BleMeshRelayStatus getRelayStatus() const { return relayStatus; }
	BleMeshCompositionData getCompositionData() const { return compositionData; }
	bool getOnOffState() const { return onOffState; }

	void setConfig(const std::string config) { this->config = config; }
	void setAppKeyIndexes(std::vector<uint16_t> &appKeyIndexes) { this->appKeyIndexes = appKeyIndexes; }
	void setGattProxyState(uint8_t gattProxyState) { this->gattProxyState = gattProxyState; }
	void setRelayStatus(BleMeshRelayStatus relayStatus) { this->relayStatus = relayStatus; }
	void setTTL(uint8_t ttl) { this->ttl = ttl; }
	void setCompositionData(BleMeshCompositionData compositionData) { this->compositionData = compositionData; }
	void setOnOffState(bool onOffState) { this->onOffState = onOffState; }

private:
	/** Configuration to get. Values can be,
	 * APPKEYINDEX
	 * DEFAULT_TTL
	 * GATT_PROXY
	 * RELAY
	 */
	std::string config;
	/** A list of AppKey indexes that are bound to the NetKey identified by
	 * netKeyIndex passed during configGet API call
	 * Note: This information will be sent when config is APPKEYINDEX
	 */
	std::vector<uint16_t> appKeyIndexes;

	/** GATT proxy state
	 * Value can be, 0x00 - The Proxy feature is supported and disabled
	 * 0x01 - The Proxy feature is supported and enabled
	 * 0x02 - The Proxy feature is not supported
	 * Note: This information will be sent when config is GATT_PROXY
	 */
	uint8_t gattProxyState;

	/** TTL
	 *Values will be in the range: 0x00, 0x02–0x7F
	 */
	uint8_t ttl;

	/** Current Relay and Relay Retransmit states of a node
	 * Note: This information will be sent when config is RELAY
	 */
	BleMeshRelayStatus relayStatus;
	/**
	 * Composition data of a node.
	 */
	BleMeshCompositionData compositionData;

        /** onOffState
         * onOff State received from remote device if supported
         */
        bool onOffState;
};

/** @brief Class representing the payload for send() API when the command in send
 * API is "passthrough"
 */
class BleMeshPayloadPassthrough
{
public:
	/** @brief Byte array represneting payload that is in the packet format as
	 * mentioned in the specification. Underlying sil implementaion or stack
	 * does not do any processing of the payload. It just encrypts and sends */
	std::vector<uint8_t> value;
};

/** @brief Class representing the payload for send() API when the command is "onoff" */
class BleMeshPayloadOnOff
{
public:
	/** @brief Value representing on/off state.
	 * value = true - ON
	 * value = false - OFF
	 */
	bool value;
};

/** @brief Class representing the payload for send() API */
class BleMeshPayload
{
public:
	/* Accesssor and modifier functions for private variables */
	BleMeshPayloadPassthrough getPayloadPassthrough() const { return payloadPassthrough; }
	BleMeshPayloadOnOff getPayloadOnOff() const { return payloadOnOff; }
	void setPayloadPassthrough(BleMeshPayloadPassthrough &payloadPassthrough) { this->payloadPassthrough = payloadPassthrough; }
	void setPayloadOnOff(BleMeshPayloadOnOff &payloadOnOff) { this->payloadOnOff = payloadOnOff; }
private:
	/** @brief Payload for "passthrough" command */
	BleMeshPayloadPassthrough payloadPassthrough;
	/** @brief Payload for "onoff" command */
	BleMeshPayloadOnOff payloadOnOff;
};

/* Callback function types */
/** @brief Callback to provide mesh network information when getMeshInfo is called */
typedef std::function<void(BluetoothError, BleMeshInfo &meshInfo)> BleMeshInfoCallback;

/** @brief Class representing the Observer APIs which the bluetooth2 service must implement
 * in order to get corresponding information from underlying stack via sil implementation
 */
class BluetoothMeshObserver
{
public:
	/**
	 * @brief This method is called when messages addressed to the application is received.
	 *
	 * @param adapterAddress Adapter Address
	 * @param srcAddress Source element address
	 * @param destAddress Destination address
	 * @param data Incoming message
	 */
	virtual void modelDataReceived(const std::string &adapterAddress,
								   uint16_t srcAddress, uint16_t destAddress,
								   uint16_t appKey, uint8_t data[], uint32_t datalen) {}

	/**
	 * @brief This method is called when any unprovisioned device is discovered.
	 *
	 * @param adapterAddress Adapter Address
	 * @param rssi Measurement of the signal strength of the received unprovisioned beacon
	 * @param uuid UUID of the device discovered.
	 * @param name Name of the device discovered.
	 */
	virtual void scanResult(const std::string &adapterAddress, const int16_t rssi, const std::string &uuid, const std::string &name = "") {}

	/**
	 * @brief This method is called when any unprovisioned device is discovered.
	 *
	 * @param adapterAddress Adapter Address
	 * @param configuration Class representing the configuration data that can be retrieved from a node
	 * @param error Success or failure of provisioning
	 */
	virtual void modelConfigResult(const std::string &adapterAddress, BleMeshConfiguration &configuration, BluetoothError error) {}

	/**
	 * @brief This method is called when any onOff model status is received.
	 *
	 * @param adapterAddress Adapter Address
	 * @param onOffState received state
	 * @param error Success or failure
	 */
	virtual void modelSetOnOffResult(const std::string &adapterAddress, bool onOffState, BluetoothError error) {}

	/**
	 * @brief This method is called when any unprovisioned device is discovered.
	 *
	 * @param adapterAddress Adapter Address
	 * @param networkId networkId of created network
	 */
	virtual void updateNetworkId(const std::string &adapterAddress, const uint64_t networkId = 0) {}

	/**
	 * @brief This method is called when provisioning of a device is success or failed.
	 *
	 * @param error Success or failure of provisioning
	 * @param adapterAddress Adapter Address
	 * @param request Indicates the type of user interaction required to complete provisioning
	 *                Possible values are:
	 *                displayString : Display the string from "stringToDisplay"
	 *                                field and does not require any additional
	 *                                input locally. For instance: "Enter "ABCDE" on remote device".
	 *                displayNumeric : Display the number from "numberToDisplay"
	 *                                 field in the way requested in "numberDisplayType " field
	 *                promptNumeric : Supply the decimal value between 1-99999999.
	 *                                Which decimal value to supply is indicated by
	 *                                "promptType" field. supplyProvisioningNumeric()
	 *                                API can be used to supply the required data.
	 *                promptStatic : Supply 16 octet byte array, as an Out-of-Band
	 *                               authentication. supplyProvisioningOob()
	 *                               API can be used to supply the required data
	 *                endProvision : Indicates the end of provisioning
	 * @param stringToDisplay String to display to the user when "request"="displayString"
	 * @param numberToDisplay Number to display when "request"="displayNumeric"
	 * @param numberDisplayType Way the number should be displayed when "request"="displayNumeric"
	 *                          Possible values are:
	 *                          blink : Locally blink LED
	 *                          beep : Locally make a noise
	 *                          vibrate : Locally vibrate
	 *                          out-numeric : Display value to enter remotely
	 *                          push : Request pushes on remote button
	 *                          twist : Request twists on remote knob
	 * @param promptType Indicates the input method, i.e. how the number to
	 *                   supply is shown when "request"="promptNumeric".
	 *                   Possible values are:
	 *                   blink : Enter times remote LED blinked
	 *                   beep : Enter times remote device beeped
	 *                   vibrate : Enter times remote device vibrated
	 *                   in-numeric : Enter remotely displayed value
	 *                   push : Push local button requested times
	 *                   twist : Twist local knob remotely requested times
	 * @param unicastAddress Unicast address of the node provisioned when the
	 *                       provisioning is success and "request"="endProvision"
	 * @param count  number of unicast addresses assigned
	 *	to the new node.
	 * @param uuid Device uuid
	 */
	virtual void provisionResult(BluetoothError error, const std::string &adapterAddress,
								 const std::string &request = "",
								 const std::string &stringToDisplay = "",
								 const uint32_t numberToDisplay = 0,
								 const std::string &numberDisplayType ="",
								 const std::string &promptType = "",
								 uint16_t unicastAddress = 0,
								 uint8_t count = 0,
								 const std::string &uuid = "") {}
};

/** Class representing the APIs that bluetooth2 service can call to support BLE Mesh.
 * These APIs must be implemented bu the sil-implementation for corresponding bluetooth stack
 */
class BluetoothMeshProfile
{
public:
	BluetoothMeshProfile() : observer(0) { }

	virtual ~BluetoothMeshProfile() {}

	/**
	 * @brief Register a observer for the Mesh profile. The observer will be notified about
	 *        results from requested operations or any events.
	 *
	 * @param observer Observer instance to register.
	 */
	void registerObserver(BluetoothMeshObserver *observer)
	{
		this->observer = observer;
	}
	/**
	 * @brief This API is called to become a Provisioner node, and a Configuration
	 * Client on a newly created Mesh Network. This API call will create a
	 * new mesh network and adds the local node as provisioner.
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 */
	virtual BluetoothError createNetwork(const std::string &bearer)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief This API is called when network is already created, and node
	 * need to be attached with the existing mesh token.

	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param token Mesh Token
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE
	 */
	virtual BluetoothError attach(const std::string &bearer, const std::string &token)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief This method is used to get mesh network related information when
	 * local node is provisioner.
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param callback Callback called to provide meshInfo
	 */
	virtual void getMeshInfo(const std::string &bearer, BleMeshInfoCallback callback)
	{
		BleMeshInfo meshInfo;
		if (callback)
			callback(BLUETOOTH_ERROR_UNSUPPORTED, meshInfo);
	}
	/**
	 * @brief This method is used to look for mesh devices to provision when the
	 * local node is provisioner.
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param scanTimeout Scan timeout in seconds. Default value: 20 sec
	 *
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 *                          BLUETOOTH_ERROR_BUSY
	 */
	virtual BluetoothError scanUnprovisionedDevices(const std::string &bearer, const uint16_t scanTimeout)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief This API is used to cancel scanning for unprovisioned BLE mesh devices
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 *                          BLUETOOTH_ERROR_BUSY
	 */
	virtual BluetoothError unprovisionedScanCancel(const std::string &bearer)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief This method initiates provisioning of a device. Once a mesh device
	 * is discovered a device using mesh/scanUnprovisionedDevices API,
	 * the discovered device can be provisioned using this method. After the
	 * provisioning, the device becomes a node in the mesh network.
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param uuid   Device UUID for provisioning device. UUID is received as part of
	 *               subscription response of mesh/scanUnprovisionedDevices API
	 * @param timeout Provision request timeout in seconds
	 *                Default value: 60 sec.
	 *                From specification: The provisioning protocol shall have a
	 *                minimum timeout of 60 seconds that is reset each time a
	 *                provisioning protocol PDU is sent or received
	 *                Note: Authentication data must be supplied in this interval if required
	 *                eg. call mesh/supplyProvisioningOob API if request string is "promptStatic"
	 *
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 */
	virtual BluetoothError provision(const std::string &bearer, const std::string &uuid,
										const uint16_t timeout)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief This method is used to supply the numeric data required during provision
	 * When the "request" field of provisionResult() is "promptNumeric", this API
	 * can be called to supply requested numeric data.
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param number Numeric data
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 *                          BLUETOOTH_ERROR_NOT_ALLOWED
	 */
	virtual BluetoothError supplyProvisioningNumeric(const std::string &bearer, uint32_t number)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief This method is used to supply the OOB data required during provision
	 * When the "request" field of provisionResult() is "promptStatic", this API
	 * can be called to supply requested static data.
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param oobData OOB data
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 *                          BLUETOOTH_ERROR_NOT_ALLOWED
	 */
	virtual BluetoothError supplyProvisioningOob(const std::string &bearer, const std::string &oobData)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief This method is used to get composition data of a mesh node.
	 * The Composition Data contains information about a node, the elements it
	 * includes, and the supported models.
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param destAddress Destination node address
	 *
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 */
	virtual BluetoothError getCompositionData(const std::string &bearer, uint16_t destAddress)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief This method can be used by the application to generate and add a
	 * new application key. This call affects the local key database only.
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param netKeyIndex Net key to bind the application key to
	 * @param appKeyIndex App key index to add
	 *
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_MESH_APP_INDEX_EXISTS,
	 *                          BLUETOOTH_ERROR_MESH_NET_KEY_INDEX_DOES_NOT_EXIST,
	 *                          BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 *                          BLUETOOTH_ERROR_NOT_ALLOWED
	 */
	virtual BluetoothError createAppKey(const std::string &bearer, uint16_t netKeyIndex,
										uint16_t appKeyIndex)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief This method is used to send a message originated by a local model.
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param srcAddress Unicast address of the source element
	 * @param destAddress Destination unicast address or group address
	 * @param appKeyIndex Application key to use for encrypting the message.
	 *                 The appKeyIndex must be valid for that element
	 * @param command Command that corresponds to the message being sent in the payload.
	 *                Note: Application sends the payload that is in the packet
	 *                format as mentioned in the specification.
	 *                webos-bluetooth-service and underlying stack does not do
	 *                any processing of the payload. It just encrypts and sends
	 *                when command is passThrough. Possible values: "passThrough", "onOff"
	 * @param payload Outgoing message
	 *
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 *                          BLUETOOTH_ERROR_NOT_ALLOWED,
	 *                          BLUETOOTH_ERROR_MESH_APP_KEY_INDEX_DOES_NOT_EXIST
	 */
	virtual BluetoothError modelSend(const std::string &bearer, uint16_t srcAddress,
									 uint16_t destAddress, uint16_t appKeyIndex,
									 const std::string &command,
									 BleMeshPayload &payload)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/** @brief This method is used to on/off using generic onoff model
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param destAddress Destination unicast address or group address
	 * @param appKeyIndex Application key to use for encrypting the message.
	 *                 The appKeyIndex must be valid for that element
	 * @param onoff On/Off state.
	 *              Value can be,
	 *              true: Switch on
	 *              false: Switch off
	 * @param ack Indicates on/off message type
	 *              Value can be:
	 *              true: Send acknowledged on/off message
	 *              false: Send unacknowledged on/off message
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 *                          BLUETOOTH_ERROR_NOT_ALLOWED,
	 *                          BLUETOOTH_ERROR_MESH_APP_KEY_INDEX_DOES_NOT_EXIST
	 */
	virtual BluetoothError setOnOff(const std::string &bearer,
									 uint16_t destAddress, uint16_t appKeyIndex, bool onoff, bool ack = false)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief This is a method to get various configuration parameters provided
	 * by configuration model.
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param destAddress Unicast address of the destination (remote node)
	 * @param config Configuration to get
	 *               Values Can be,
	 *               APPKEYINDEX, DEFAULT_TTL, GATT_PROXY, RELAY
	 * @param netKeyIndex Index of the net key. Mandatory field if config field is,
	 *                 APPKEYINDEX
	 *                 Ignored for others
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 *                          BLUETOOTH_ERROR_NOT_ALLOWED,
	 *                          BLUETOOTH_ERROR_MESH_APP_KEY_INDEX_DOES_NOT_EXIST,
	 *                          BLUETOOTH_ERROR_MESH_NET_KEY_INDEX_DOES_NOT_EXIST
	 */
	virtual BluetoothError configGet(const std::string &bearer,
										uint16_t destAddress,
										const std::string &config,
										uint16_t netKeyIndex = 0)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief This is the method to set various configuration parameters provided
	 * by configuration model.
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param destAddress Unicast address of the destination
	 * @param config Config to set. Values can be,
	 *                 APPKEY_ADD
	 *                 APPKEY_UPDATE
	 *                 APPKEY_BIND
	 *                 DEFAULT_TTL
	 *                 GATT_PROXY
	 *                 RELAY
	 * @param netKeyIndex Net key index. Mandatory parameter if config is
	 *                 APPKEY_ADD, APPKEY_UPDATE
	 * @param appKeyIndex App key index. Mandatory parameter if config is APPKEY_ADD,
	 *                    APPKEY_UPDATE, APPKEY_BIND
	 * @param modelId SIG Model ID or Vendor Model ID. Mandatory parameter if
	 *                config is APPKEY_BIND
	 * @param ttl Default TTL to set for the ougoing message from the remote node
	 *            Values can be in the range: 0x00, 0x02–0x7F
	 *            Mandatory parameter if config is DEFAULT_TTL
	 * @param gattProxyState GATT proxy state to set
	 *                       Value can be, 0x00 - The Proxy feature is supported and disabled
	 *                       0x01 - The Proxy feature is supported and enabled
	 *                       0x02 - The Proxy feature is not supported
	 *                       Mandatory parameter if config is GATT_PROXY
	 * @param relayStatus Current Relay and Relay Retransmit states of a node
	 *                    Mandatory parameter if config is RELAY
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE,
	 *                          BLUETOOTH_ERROR_NOT_ALLOWED,
	 *                          BLUETOOTH_ERROR_MESH_APP_KEY_INDEX_DOES_NOT_EXIST,
	 *                          BLUETOOTH_ERROR_MESH_NET_KEY_INDEX_DOES_NOT_EXIST
	 */
	virtual BluetoothError configSet(
		const std::string &bearer, uint16_t destAddress, const std::string &config,
		uint8_t gattProxyState = 0, uint16_t netKeyIndex = 0, uint16_t appKeyIndex = 0,
		uint32_t modelId = 0, uint8_t ttl = 0, BleMeshRelayStatus *relayStatus = NULL)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

	/**
	 * @brief This is the method to update the unicast addresses asigned so far
	 *
	 * @param bearer Underlying bearer to use.
	 *               Pass PB-GATT for PB-GATT bearer
	 *               Pass PB-ADV for PB-ADV bearer
	 * @param unicastAddresses Unicast Addresses
	 * @return Returns error code.
	 *         Possible errors: BLUETOOTH_ERROR_FAIL,
	 *                          BLUETOOTH_ERROR_NONE
	 */
	virtual BluetoothError updateNodeInfo(const std::string &bearer,
								std::vector<uint16_t> &unicastAddresses)
	{
		return BLUETOOTH_ERROR_UNSUPPORTED;
	}

protected:
	/**
	 * @brief Retrieve the Mesh observer
	 *
	 *        If no observer was registered with registerObserver before a NULL object
	 *        instance will be returned which is usable but will doing nothing when
	 *        called.
	 *
	 * @return The Mesh observer
	 */
	BluetoothMeshObserver *getMeshObserver()
	{
		static BluetoothMeshObserver nullObserver;

		if (!observer)
			return &nullObserver;

		return observer;
	}

private:
	BluetoothMeshObserver *observer;
};

#endif //BLUETOOTH_SIL_MESH_H_
