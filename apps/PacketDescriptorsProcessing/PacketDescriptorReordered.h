#ifndef PACKET_DESCRIPTOR_REORDERED_H
#define PACKET_DESCRIPTOR_REORDERED_H

#include <cstdint>

struct PacketDescriptorReordered {
	uint64_t timestampNs;
	uint64_t* payload;
	uint64_t payloadHash;
	uint16_t size;
	uint8_t physicalPort;
	uint8_t isValid:1;
	uint8_t flag2:1;
	uint8_t flag3:1;

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
};

#endif /* PACKET_DESCRIPTOR_REORDERED_H */
