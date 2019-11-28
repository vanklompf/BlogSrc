#ifndef PACKET_DESCRIPTOR_STAMPED_H_
#define PACKET_DESCRIPTOR_STAMPED_H_

#include <cstdint>
#include <climits>

struct PacketDescriptorStamped
{
	struct
	{
		uintptr_t size : 13;
		uintptr_t isValid:1;
		uintptr_t flag2:1;
		uintptr_t flag3:1;
	    uintptr_t payload : ((sizeof(intptr_t)-2)*CHAR_BIT-3);
	    uintptr_t physicalPort : 3;
	} tagged;
	uint64_t payloadHash;
	uint64_t timestamp : 48;


	void SetTimestamp(uint64_t _timestamp) {
		timestamp= _timestamp;
	}

	uint64_t GetTimestamp() {
		return timestamp;
	}

	bool IsValid() const {
		return tagged.isValid;
	}

	void SetValid(bool valid) {
		tagged.isValid = valid;
	}

	uint16_t GetSize() const {
		return tagged.size;
	}

	void SetSize(uint16_t _size)  {
		tagged.size = _size;
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


#endif /* PACKET_DESCRIPTOR_STAMPED_H_ */
