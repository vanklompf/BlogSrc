#ifndef PACKET_DESCRIPTOR_REORDERED_H
#define PACKET_DESCRIPTOR_REORDERED_H

#include <cstdint>


struct PacketDescriptorReordered {
	uint8_t physicalPort;
	uint64_t timestampNs;
	uint16_t size;
	uint64_t* payload;
	uint8_t valid:1;
	uint8_t flag2:1;
	uint8_t flag3:1;
	uint64_t payloadHash;

	bool isValid() const {
		return valid;
	}

	uint16_t getSize() const {
		return size;
	}

	uint8_t getPort() const {
		return physicalPort;
	}
};

#endif /* PACKET_DESCRIPTOR_REORDERED_H */
