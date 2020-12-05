#include <cstdio>
#include <cstdlib>

#include "PacketDescriptor.h"
#include "PacketDescriptorPacked.h"
#include "PacketDescriptorReordered.h"
#include "PacketDescriptorWithAssumptions.h"
#include "PacketDescriptorTagged.h"
#include "PacketDescriptorStamped.h"
#include "ProcessingBenchmark.h"
#include "utils/Allocate.h"

static const uint64_t PACKET_BUFFER_SIZE = (uint64_t)2*1024*1024*1024;
static const int REPEAT = 5000;

uint64_t anti_opt = 1;

template<typename TDescriptor>
static void doFor(uint8_t* packetBuffer)
{
	ProcessingBenchmark<TDescriptor> descriptorBenchmark(packetBuffer, PACKET_BUFFER_SIZE, REPEAT);
	descriptorBenchmark.FillBuffer();

	printf("---------------------------------------------------------\n");
	printf("%s\nSize: %lu\n", descriptorBenchmark.GetDescriptorName(), descriptorBenchmark.GetDescriptorSize());
	printf("Trivial read processing performance: %d mpps\n", descriptorBenchmark.RunTrivialRead());
	printf("Read processing performance: %d mpps\n", descriptorBenchmark.RunRead());
	printf("Write processing performance: %d mpps\n", descriptorBenchmark.RunWrite());
}

int main()
{
	printf("Synthetic cache efficiency benchmark based on imaginary packet processig performance\n");
	printf("For details check: https://extensa.tech\n");

	printf("Allocating memory for packets...\n");
	uint8_t* packetBuffer = Allocate(PACKET_BUFFER_SIZE);
	printf("Done!\n");

	doFor<PacketDescriptor>(packetBuffer);
	doFor<PacketDescriptorReordered>(packetBuffer);
	doFor<PacketDescriptorPacked>(packetBuffer);
	doFor<PacketDescriptorWithAssumptions>(packetBuffer);
	doFor<PacketDescriptorTagged>(packetBuffer);
	doFor<PacketDescriptorStamped>(packetBuffer);

	return 0;
}
