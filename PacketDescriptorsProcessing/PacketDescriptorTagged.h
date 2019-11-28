#ifndef PACKET_DESCRIPTOR_TAGGED_H_
#define PACKET_DESCRIPTOR_TAGGED_H_

#include <cstdint>
#include <climits>

struct PacketDescriptorTagged
{
	uint16_t timestampNs_high;
	uint32_t timestampNs_low;
	struct
	{
	    uintptr_t payload : (sizeof(intptr_t)*CHAR_BIT-3);
	    uintptr_t physicalPort : 3;
	} tagged;
	uint64_t payloadHash;
	uint16_t size : 13;
	uint16_t isValid:1;
	uint16_t flag2:1;
	uint16_t flag3:1;

	void SetTimestamp(uint64_t timestamp) {
		timestampNs_high = timestamp >> 32;
		timestampNs_low = timestamp;
	}

	uint64_t GetTimestamp() {
		return (((uint64_t)timestampNs_high << 32) | timestampNs_low);
	}

	bool IsValid() const {
		return isValid;
	}

	void SetValid(bool valid) {
		isValid = valid;
	}

	uint16_t GetSize() const {
		return size;
	}

	void SetSize(uint16_t _size)  {
		size = _size;
	}

	uint8_t GetPort() const {
		return tagged.physicalPort;
	}

	void SetPort(uint8_t port) {
		tagged.physicalPort = port;
	}

	uint64_t* GetPayloadPtr() const {
		return reinterpret_cast<uint64_t*>(tagged.payload << 3);
	}

	void SetPayloadPtr(uint64_t* payload) {
		tagged.payload = reinterpret_cast<uintptr_t>(payload) >> 3;
	}
};


#endif /* PACKET_DESCRIPTOR_TAGGED_H_ */
