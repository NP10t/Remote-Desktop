#ifndef _MYCLIENT_H_
#define _MYCLIENT_H_

#include "..\PNet\IncludeMe.h"
#include <iostream>
#include <string>


class MyClient : public Client
{
	private:
		bool ProcessPacket(std::shared_ptr<Packet> packet) override;
		void OnConnect(TCPConnection & newConnection) override;
		void OnDisconnect(TCPConnection & lostConnection, std::string reason) override;
};

#endif