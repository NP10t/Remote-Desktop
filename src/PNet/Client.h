#pragma once
#include "TCPConnection.h"
#include <thread>
#include <opencv2/opencv.hpp>
#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <mutex>

using namespace std;
using namespace cv;

namespace PNet
{
	class Client
	{
	public:
		Mat img;
		float H;
		float W;
		float T;
		float L;
		bool Initialize(IPEndpoint ip);
		bool Frame(int select_device);
		void ControlUsingTCP(int select_device);
		void PlayVideo(int select_device);
		void Commander(TCPConnection &connection);
		std::thread control;
		std::thread video;
		int selected_device;
		int selected_device_connected;
		void CloseConnection(int connectionIndex, std::string reason);	
		std::vector<TCPConnection> connections;
		std::mutex mtx_control_thread;
		std::mutex mtx_playvideo_thread;
	protected:
		virtual bool ProcessPacket(std::shared_ptr<Packet> packet);
		virtual void OnConnectFail();
		virtual void OnConnect(TCPConnection &newConnection);
		virtual void OnDisconnect(TCPConnection &lostConnection, std::string reason);

		std::vector<WSAPOLLFD> master_fd;
		std::vector<WSAPOLLFD> use_fd;
	};
}