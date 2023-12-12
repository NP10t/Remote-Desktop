#pragma once
#include "..\PNet\IncludeMe.h"

class MyClient : public Client
{
	bool ProcessPacket(std::shared_ptr<Packet> packet) override;
	void OnConnect(TCPConnection & newConnection) override;
	void OnDisconnect(TCPConnection & lostConnection, std::string reason) override;
};