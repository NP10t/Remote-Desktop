#pragma once
#include <queue> //for std::queue
#include <memory> //for std::shared_ptr
#include "Packet.h"

namespace PNet
{
	enum PacketManagerTask
	{
		ProcessPacketSize,
		ProcessPacketContents
	};

	class PacketManager
	{
	private:
		std::queue<std::shared_ptr<Packet>> packets;
	public:
		void Clear();
		bool HasPendingPackets();
		void Append(std::shared_ptr<Packet> p);
		std::shared_ptr<Packet> Retrieve();
		void Pop();

		uint32_t currentPacketSize = 0;
		uint32_t currentPacketExtractionOffset = 0;
		PacketManagerTask currentTask = PacketManagerTask::ProcessPacketSize;
	};
}