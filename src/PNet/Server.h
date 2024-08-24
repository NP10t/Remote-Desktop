#pragma once
#include "TCPConnection.h"
#include <thread>
#include <opencv2/opencv.hpp>
#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <mutex>
#include <string>
#include <fstream>
using namespace std;
using namespace cv;

using namespace PNet;

namespace PNet
{
	class Server
	{
	public:
		float shitfD = 0, ctrlD = 0;

		bool Initialize(Socket &listeningSocket, WSAPOLLFD &listeningSocketFD, int port);
		void Frame(Socket &listeningSocket, WSAPOLLFD &listeningSocketFD, WSAPOLLFD &newConnectionFD, SHORT even);
		void Obey();
		void Livestream();
		void Video();
		cv::Mat captureScreen(HWND hwnd, int targetWidth, int targetHeight);
		std::thread Obey_thread;
		std::thread Livestream_thread;
		bool isConnected = false;

		void CloseConnection(std::string reason, WSAPOLLFD &listeningSocketFD);
	
		std::string IPv4Address;
		std::string GetIPv4Address();
		std::mutex mtx_obey_thread;
		std::mutex mtx_livestream_thread;

	// protected:
		virtual void OnDisconnect(std::string reason);
		virtual void OnConnect();
		virtual bool ProcessPacket(std::shared_ptr<Packet> packet);
		TCPConnection connection;
		Socket obeyListeningSocket;
		Socket livestreamListeningSocket;


		WSAPOLLFD obeyNewConnectionFD;
		WSAPOLLFD obeyListeningSocketFD;

		WSAPOLLFD livestreamNewConnectionFD;
		WSAPOLLFD livestreamListeningSocketFD;
	};
}