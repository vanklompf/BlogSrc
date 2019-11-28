#ifndef PACKET_DESCRIPTOR_WITH_ASSUMPTIONS_H
#define PACKET_DESCRIPTOR_WITH_ASSUMPTIONS_H

#include <cstdint>
#include <cstring>

struct PacketDescriptorWithAssumptions {
	uint16_t timestampNs_high;
	uint32_t timestampNs_low;
	uint64_t* payload;
	uint64_t payloadHash;
	uint16_t size : 13;
	uint16_t physicalPort : 3;
	uint8_t isValid:1;
	uint8_t flag2:1;
	uint8_t flag3:1;

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
		return physicalPort;
	}

	void SetPort(uint8_t port) {
		physicalPort = port;
	}
} __attribute__((packed));

#endif /* PACKET_DESCRIPTOR_WITH_ASSUMPTIONS_H */
