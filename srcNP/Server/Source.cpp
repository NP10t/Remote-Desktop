//Server Code [Tutorial 9] [Nonblocking] [Winsock]
//Author: Jacob Preston 2019-06-25

#include "MyServer.h"
#include <iostream>


//g++ .\PNet\IPEndpoint.cpp .\PNet\Network.cpp .\PNet\Socket.cpp .\PNet\Packet.cpp .\PNet\TCPConnection.cpp .\PNet\PacketManager.cpp .\PNet\Server.cpp .\Server\Source.cpp .\Server\MyServer.cpp -o nguoihau.exe -lws2_32

int main()
{
	if (Network::Initialize())
	{
		MyServer server;
		if (server.Initialize(IPEndpoint("::", 6112)))
		{
			while (true)
			{
				server.Frame();
			}
			for (auto& thread : server.threads) {
				thread.join();
			}
		}
	}
	Network::Shutdown();
	system("pause");
	return 0;
}