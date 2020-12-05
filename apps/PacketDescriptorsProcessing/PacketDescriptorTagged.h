#ifndef PACKET_DESCRIPTOR_TAGGED_H_
#define PACKET_DESCRIPTOR_TAGGED_H_

#include <cstdint>
#include <climits>

struct PacketDescriptorTagged
{
	uint64_t timestamp : 48;
	uint64_t size : 13;
	uint64_t isValid:1;
	uint64_t flag2:1;
	uint64_t flag3:1;
	struct
	{
	    uintptr_t payload : (sizeof(intptr_t)*CHAR_BIT-3);
	    uintptr_t physicalPort : 3;
	} tagged;
	uint64_t payloadHash;


	void SetTimestamp(uint64_t _timestamp) {
		timestamp= _timestamp;
	}

	uint64_t GetTimestamp() {
		return timestamp;
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
} __attribute__((packed));


#endif /* PACKET_DESCRIPTOR_TAGGED_H_ */
