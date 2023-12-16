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
		bool Initialize();
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
	
		std::string IPv4Address;
		std::string GetIPv4Address();
		

	protected:
		virtual void OnDisconnect(std::string reason);
		virtual void OnConnect();

		virtual bool ProcessPacket(std::shared_ptr<Packet> packet);
		TCPConnection connection;
		Socket listeningSocket;
		WSAPOLLFD newConnectionFD;
		WSAPOLLFD listeningSocketFD;
		bool isConnected = false;
	};
}