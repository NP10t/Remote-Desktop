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
		bool Connect(IPEndpoint ip);
		bool Frame(int select_device);
		void ControlUsingTCP(int select_device);
		void PlayVideo(int select_device);
		void Mouse(TCPConnection &connection);
		void Keyboard(TCPConnection &connection);
		std::thread control;
		std::thread video;
		int selected_device;
		void CloseConnection(int connectionIndex, std::string reason);		
	protected:
		virtual bool ProcessPacket(std::shared_ptr<Packet> packet);
		virtual void OnConnectFail();
		virtual void OnConnect(TCPConnection &newConnection);
		virtual void OnDisconnect(TCPConnection &lostConnection, std::string reason);

		std::vector<TCPConnection> connections;
		std::vector<WSAPOLLFD> master_fd;
		std::vector<WSAPOLLFD> use_fd;
	};
}