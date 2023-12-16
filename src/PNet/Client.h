#pragma once
#include "TCPConnection.h"
#include <thread>
// #include "ProcessVideo.h"
#include <opencv2/opencv.hpp>
#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

using namespace std;
using namespace cv;

namespace PNet
{
	class Client
	{
	public:
<<<<<<< HEAD
		bool Initialize(IPEndpoint ip);
=======
		double H;
		double W;
		double T;
		double L;
		bool Connect(IPEndpoint ip);
>>>>>>> remote-origin/nguyenquangthinh
		bool Frame(int select_device);
		void ControlUsingTCP(int select_device);
		void PlayVideo(int select_device);
		void Mouse(TCPConnection &connection);
		void Keyboard(TCPConnection &connection);
<<<<<<< HEAD
=======
		void Chat(TCPConnection &connection);
>>>>>>> remote-origin/nguyenquangthinh
		std::thread control;
		std::thread video;
		int selected_device;
		int selected_device_connected;
<<<<<<< HEAD
		void CloseConnection(int connectionIndex, std::string reason);	
		std::vector<TCPConnection> connections;	

=======
		void CloseConnection(int connectionIndex, std::string reason);		
>>>>>>> remote-origin/nguyenquangthinh
	protected:
		virtual bool ProcessPacket(std::shared_ptr<Packet> packet);
		virtual void OnConnectFail();
		virtual void OnConnect(TCPConnection &newConnection);
		virtual void OnDisconnect(TCPConnection &lostConnection, std::string reason);

<<<<<<< HEAD
		// std::vector<TCPConnection> connections;
=======
		std::vector<TCPConnection> connections;
>>>>>>> remote-origin/nguyenquangthinh
		std::vector<WSAPOLLFD> master_fd;
		std::vector<WSAPOLLFD> use_fd;
	};
}