#ifndef PACKET_DESCRIPTOR_WITH_ASSUMPTIONS_H
#define PACKET_DESCRIPTOR_WITH_ASSUMPTIONS_H

#include <cstdint>
#include <cstring>

struct PacketDescriptorWithAssumptions {
	uint64_t timestamp : 48;
	uint64_t size : 13;
	uint64_t physicalPort : 3;
	uint64_t* payload;
	uint64_t payloadHash;
	uint8_t isValid:1;
	uint8_t flag2:1;
	uint8_t flag3:1;

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
		return physicalPort;
	}

	void SetPort(uint8_t port) {
		physicalPort = port;
	}
} __attribute__((packed));

#endif /* PACKET_DESCRIPTOR_WITH_ASSUMPTIONS_H */
