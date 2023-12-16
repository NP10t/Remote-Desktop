#pragma once
#include "..\PNet\IncludeMe.h"

using namespace std;

class MyServer : public Server
{
	public:
<<<<<<< HEAD
		void Run();
=======
		void Run(std::string IPString);
>>>>>>> remote-origin/nguyenquangthinh
		
	private:
		void OnConnect() override;
		// void OnConnect(TCPConnection & newConnection) override;
		// void OnDisconnect(TCPConnection & lostConnection, std::string reason) override;
		bool ProcessPacket(std::shared_ptr<Packet> packet) override;
<<<<<<< HEAD
};
=======
		void Chat(TCPConnection & connection);
};
>>>>>>> remote-origin/nguyenquangthinh
