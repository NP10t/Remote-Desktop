#include "MyServer.h"
#include <opencv2/opencv.hpp>
#include <iostream>

void MyServer::OnConnect()
{
	isConnected = 1;
	std::cout << "Successfully connected to the Client!" << std::endl;

	std::shared_ptr<Packet> helloPacket = std::make_shared<Packet>(PacketType::PT_ChatMessage);
	*helloPacket << std::string("Hello from the Server!");
	connection.pm_outgoing.Append(helloPacket);
}

bool MyServer::ProcessPacket(std::shared_ptr<Packet> packet)
{
	switch (packet->GetPacketType())
	{
	case PacketType::PT_Mouse: {
		// moi ngiu Kieu Tho lam tiep
	}
	case PacketType::PT_Keyboard: {
		// moi ngiu Kieu Tho lam tiep
	}
	case PacketType::PT_ChatMessage:
	{
		std::string chatmessage;
		*packet >> chatmessage;
		// std::cout << "Client says: " << chatmessage << std::endl;
		break;
	}
	case PacketType::PT_IntegerArray:
	{
		uint32_t arraySize = 0;
		*packet >> arraySize;
		std::cout << "Array Size: " << arraySize << std::endl;
		for (uint32_t i = 0; i < arraySize; i++)
		{
			uint32_t element = 0;
			*packet >> element;
			std::cout << "Element[" << i << "] - " << element << std::endl;
		}
		break;
	}
	default:
		std::cout << "Unrecognized packet type: " << packet->GetPacketType() << std::endl;
		return false;
	}

	return true;
}