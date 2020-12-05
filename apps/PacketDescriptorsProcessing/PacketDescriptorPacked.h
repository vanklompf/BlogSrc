#ifndef PACKET_DESCRIPTOR_PACKED_H
#define PACKET_DESCRIPTOR_PACKED_H

#include <cstdint>


struct PacketDescriptorPacked {
	uint64_t timestampNs;
	uint64_t* payload;
	uint64_t payloadHash;
	uint16_t size;
	uint8_t physicalPort;
	uint8_t isValid:1;
	uint8_t flag2:1;
	uint8_t flag3:1;

	uint64_t GetTimestamp() {
		return timestampNs;
	}

	void SetTimestamp(uint64_t timestamp) {
		timestampNs = timestamp;
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
		return physicalPort;
	}

	void SetPort(uint8_t port) {
		physicalPort = port;
	}

	void SetPayloadPtr(uint64_t* _payload) {
		payload = _payload;
	}

	uint64_t*  GetPayloadPtr() {
		return payload;
	}
} __attribute__((packed));

#endif /* PACKET_DESCRIPTOR_PACKED_H */
