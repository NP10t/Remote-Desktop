#pragma once
#include "TCPConnection.h"
#include <thread>
// #include "IncludeMe.h"
#include <opencv2/opencv.hpp>
#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

using namespace std;
using namespace cv;

using namespace PNet;

namespace PNet
{
	class Server
	{
	public:
<<<<<<< HEAD
		bool Initialize();
=======
		bool Initialize(IPEndpoint ip);
>>>>>>> remote-origin/nguyenquangthinh
		void Frame();
		void Obey();
		void Livestream();
		void Video();
		cv::Mat captureScreen(HWND hwnd, int targetWidth, int targetHeight);
		std::thread Obey_thread;
		std::thread Livestream_thread;
		// std::vector<std::thread> threads;
		// std::vector<std::thread> buttons;

		void CloseConnection(std::string reason);
<<<<<<< HEAD
	
		std::string IPv4Address;
		std::string GetIPv4Address();
		
=======
>>>>>>> remote-origin/nguyenquangthinh

	protected:
		virtual void OnDisconnect(std::string reason);
		virtual void OnConnect();

<<<<<<< HEAD
		virtual bool ProcessPacket(std::shared_ptr<Packet> packet);
		TCPConnection connection;
		Socket listeningSocket;
=======
		
		virtual bool ProcessPacket(std::shared_ptr<Packet> packet);

		Socket listeningSocket;
		TCPConnection connection;
>>>>>>> remote-origin/nguyenquangthinh
		WSAPOLLFD newConnectionFD;
		WSAPOLLFD listeningSocketFD;
		bool isConnected = false;
	};
}