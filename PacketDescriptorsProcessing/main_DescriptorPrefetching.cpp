#include <numeric>
#include <cstdio>
#include <cstdlib>

#include "PacketDescriptor.h"
#include "PacketDescriptorPacked.h"
#include "PacketDescriptorReordered.h"
#include "PacketDescriptorWithAssumptions.h"
#include "PacketDescriptorTagged.h"
#include "PacketDescriptorStamped.h"
#include "ProcessingBenchmark.h"
#include "Utils.h"

static const uint64_t PACKET_DESCRIPTORS_BUFFER_SIZE = (uint64_t)1*1024*1024*1024;
static const uint64_t PACKET_PAYLOAD_BUFFER_SIZE = (uint64_t)1*1024*1024*1024;
static const int REPEAT = 50;
static const int REPEAT_PAYLOAD = 20;

uint64_t anti_opt = 1;

template<typename TDescriptor, int PREFETCHING_AMOUNT, int LOCALITY=3>
static void doFor(uint8_t* packetBuffer)
{
	ProcessingBenchmark<TDescriptor, PREFETCHING_AMOUNT, LOCALITY> descriptorBenchmark(packetBuffer, PACKET_DESCRIPTORS_BUFFER_SIZE, REPEAT);
	descriptorBenchmark.FillBuffer();

	printf("---------------------------------------------------------\n");
	printf("Prefetching %d bytes ahead\n", PREFETCHING_AMOUNT);
	printf("Prefetching locality: %d\n", LOCALITY);
	printf("Trivial read processing performance: %d mpps\n", descriptorBenchmark.RunTrivialRead());
}

template<typename TDescriptor, int PREFETCHING_AMOUNT, int LOCALITY=3>
static void doPayloadFor(uint8_t* packetBuffer, uint64_t* payloadBuffer)
{
	ProcessingBenchmark<TDescriptor, PREFETCHING_AMOUNT, LOCALITY> descriptorBenchmark(packetBuffer, PACKET_DESCRIPTORS_BUFFER_SIZE, REPEAT_PAYLOAD);
	descriptorBenchmark.FillBuffer();
	descriptorBenchmark.SetPayloadPtrs(payloadBuffer, PACKET_PAYLOAD_BUFFER_SIZE);

	printf("---------------------------------------------------------\n");
	printf("Prefetching %d pointers ahead\n", PREFETCHING_AMOUNT);
	printf("Prefetching locality: %d\n", LOCALITY);
	printf("Payload read processing performance: %d mpps\n", descriptorBenchmark.RunPayloadRead());
}

/*
template<int LOCALITY>
static void doForLocality(uint8_t* packetBuffer, uint64_t* payloadBuffer)
{
	//doFor<PacketDescriptorPacked, 0, LOCALITY>(packetBuffer, payloadBuffer);
	//doFor<PacketDescriptorPacked, 2, LOCALITY>(packetBuffer, payloadBuffer);
	doFor<PacketDescriptorPacked, 4, LOCALITY>(packetBuffer, payloadBuffer);
	doFor<PacketDescriptorPacked, 8, LOCALITY>(packetBuffer, payloadBuffer);
	doFor<PacketDescriptorPacked, 16, LOCALITY>(packetBuffer, payloadBuffer);
	doFor<PacketDescriptorPacked, 32, LOCALITY>(packetBuffer, payloadBuffer);
	doFor<PacketDescriptorPacked, 64, LOCALITY>(packetBuffer, payloadBuffer);
	//doFor<PacketDescriptorPacked, 128, LOCALITY>(packetBuffer, payloadBuffer);
	doFor<PacketDescriptorPacked, 256, LOCALITY>(packetBuffer);
	doFor<PacketDescriptorPacked, 512, LOCALITY>(packetBuffer);
	doFor<PacketDescriptorPacked, 1024, LOCALITY>(packetBuffer);
	doFor<PacketDescriptorPacked, 2048, LOCALITY>(packetBuffer);
	doFor<PacketDescriptorPacked, 4096, LOCALITY>(packetBuffer);
	doFor<PacketDescriptorPacked, 8192, LOCALITY>(packetBuffer, payloadBuffer);
}*/

uint64_t f()
{ 
    static uint64_t i = 1;
    return i++;
}

int main()
{
	printf("Synthetic cache efficiency benchmark based on imaginary packet processig performance\n");
	printf("For details check: https://extensa.tech\n");

	printf("Allocating memory for packets...\n");
	uint8_t* packetBuffer = Allocate(PACKET_DESCRIPTORS_BUFFER_SIZE);
	uint64_t* payloadBuffer = (uint64_t*)Allocate(PACKET_PAYLOAD_BUFFER_SIZE);
    std::iota(payloadBuffer, payloadBuffer+(PACKET_PAYLOAD_BUFFER_SIZE / 8), 0);
	printf("Done!\n");

	/*doFor<PacketDescriptorPacked, 0>(packetBuffer);
	doFor<PacketDescriptorPacked, 64>(packetBuffer);
	doFor<PacketDescriptorPacked, 128>(packetBuffer);
	doFor<PacketDescriptorPacked, 256>(packetBuffer);
	doFor<PacketDescriptorPacked, 512>(packetBuffer);
	doFor<PacketDescriptorPacked, 1024>(packetBuffer);
	doFor<PacketDescriptorPacked, 2048>(packetBuffer);
	doFor<PacketDescriptorPacked, 4096>(packetBuffer);
	doFor<PacketDescriptorPacked, 8192>(packetBuffer);*/

	doPayloadFor<PacketDescriptorPacked, 0>(packetBuffer, payloadBuffer);
	doPayloadFor<PacketDescriptorPacked, 1>(packetBuffer, payloadBuffer);
	doPayloadFor<PacketDescriptorPacked, 2>(packetBuffer, payloadBuffer);
	doPayloadFor<PacketDescriptorPacked, 3>(packetBuffer, payloadBuffer);
	doPayloadFor<PacketDescriptorPacked, 4>(packetBuffer, payloadBuffer);
	doPayloadFor<PacketDescriptorPacked, 8>(packetBuffer, payloadBuffer);
	doPayloadFor<PacketDescriptorPacked, 16>(packetBuffer, payloadBuffer);
	doPayloadFor<PacketDescriptorPacked, 32>(packetBuffer, payloadBuffer);
	doPayloadFor<PacketDescriptorPacked, 64>(packetBuffer, payloadBuffer);
	doPayloadFor<PacketDescriptorPacked, 128>(packetBuffer, payloadBuffer);
	doPayloadFor<PacketDescriptorPacked, 256>(packetBuffer, payloadBuffer);

	//doPayloadFor<PacketDescriptorPacked, 0>(packetBuffer, payloadBuffer);    
	//doPayloadFor<PacketDescriptorPacked, 8>(packetBuffer, payloadBuffer);
    //doFor<PacketDescriptorPacked, 0>(packetBuffer);
    
	//doPayloadFor<PacketDescriptorPacked, 8>(packetBuffer, payloadBuffer);
	//doPayloadFor<PacketDescriptorPacked, 16>(packetBuffer, payloadBuffer);
	//doPayloadFor<PacketDescriptorPacked, 32>(packetBuffer, payloadBuffer);
}

