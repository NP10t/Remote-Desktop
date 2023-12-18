#include "Server.h"
#include <iostream>
#include "Network.h"

namespace PNet
{
	std::string Server::GetIPv4Address()
	{
		std::string line;
		std::ifstream IPFile;
		int offset;
		char *search0 = new char[36];
		search0 = "IPv4 Address. . . . . . . . . . . :";;
		system("ipconfig > ip.txt");
		IPFile.open("ip.txt");
		if(IPFile.is_open())
		{
			while(!IPFile.eof())
			{
				getline(IPFile, line);
				if((offset = line.find(search0)) != string::npos)
				{
					line.erase(0,39);
					IPFile.close();
					return line;
				}
			}
		}
		return "";
	}

	bool Server::Initialize()
	{
		IPEndpoint ip("0.0.0.0", 6112);
		listeningSocket = Socket(ip.GetIPVersion());
		if (listeningSocket.Create() == PResult::P_Success)
		{
			// std::cout << "Socket successfully created." << std::endl;
			if (listeningSocket.Listen(ip) == PResult::P_Success)
			{
				listeningSocketFD.fd = listeningSocket.GetHandle();
				listeningSocketFD.events = POLLRDNORM;
				listeningSocketFD.revents = 0;

				std::cout << "Socket successfully listening." << std::endl;
				return true;
			}
			else
			{
				std::cerr << "Failed to listen." << std::endl;
			}
			listeningSocket.Close();
		}
		else
		{
			std::cerr << "Socket failed to create." << std::endl;
		}
		return false;
	}

	void Server::Frame()
	{
		while(1){
			if (WSAPoll(&listeningSocketFD, 1, 1) > 0)
			{
				if (listeningSocketFD.revents & POLLRDNORM)
				{
					Socket newConnectionSocket;
					IPEndpoint newConnectionEndpoint;
					if (listeningSocket.Accept(newConnectionSocket, &newConnectionEndpoint) == PResult::P_Success)
					{
						connection = TCPConnection(newConnectionSocket, newConnectionEndpoint);
						OnConnect();
						newConnectionFD.fd = newConnectionSocket.GetHandle();
						newConnectionFD.events = POLLRDNORM | POLLWRNORM;
						newConnectionFD.revents = 0;

						Obey_thread = std::thread(&Server::Obey, this);
						Obey_thread.detach();

						Livestream_thread = std::thread(&Server::Livestream, this);
						Livestream_thread.detach();
						return;
					}
					else
					{
						std::cerr << "Failed to accept new connection." << std::endl;
					}
				}
			}
		}
	}

	void Server::Obey()
	{
		while (this->isConnected)
		{
			mtx_obey_thread.lock();
			WSAPOLLFD use_fd = newConnectionFD;
			if (WSAPoll(&use_fd, 1, 1) > 0)
			{
				if (use_fd.revents & POLLERR) // If error occurred on this socket
				{
					// MessageBox(NULL, TEXT("pollerr tai obey"), TEXT("Loi"), MB_ICONERROR | MB_OK);
					mtx_obey_thread.unlock();
					CloseConnection("POLLERR");
					return;
				}

				if (use_fd.revents & POLLHUP) // If poll hangup occurred on this socket
				{
					// MessageBox(NULL, TEXT("pollup tai obey"), TEXT("Loi"), MB_ICONERROR | MB_OK);
					mtx_obey_thread.unlock();
					CloseConnection("POLLHUP");
					return;
				}

				if (use_fd.revents & POLLNVAL) // If invalid socket
				{
					// MessageBox(NULL, TEXT("pollnval tai obey"), TEXT("Loi"), MB_ICONERROR | MB_OK);
					mtx_obey_thread.unlock();
					CloseConnection("POLLNVAL");
					return;
				}

				if (use_fd.revents & POLLRDNORM) // If normal data can be read without blocking
				{
					int bytesReceived = 0;

					if (connection.pm_incoming.currentTask == PacketManagerTask::ProcessPacketSize)
					{
						bytesReceived = recv(use_fd.fd, (char *)&connection.pm_incoming.currentPacketSize + connection.pm_incoming.currentPacketExtractionOffset, sizeof(uint32_t) - connection.pm_incoming.currentPacketExtractionOffset, 0);
					}
					else // Process Packet Contents
					{
						bytesReceived = recv(use_fd.fd, (char *)&connection.buffer + connection.pm_incoming.currentPacketExtractionOffset, connection.pm_incoming.currentPacketSize - connection.pm_incoming.currentPacketExtractionOffset, 0);
					}

					if (bytesReceived == 0) // If connection was lost
					{
						// MessageBox(NULL, TEXT("lost connection tai obey"), TEXT("Loi"), MB_ICONERROR | MB_OK);
						mtx_obey_thread.unlock();
						CloseConnection("Recv==0");
						return;
					}

					if (bytesReceived == SOCKET_ERROR) // If error occurred on socket
					{
						int error = WSAGetLastError();
						if (error != WSAEWOULDBLOCK)
						{
							// MessageBox(NULL, TEXT("SOCKET_ERROR tai obey"), TEXT("Loi"), MB_ICONERROR | MB_OK);
							mtx_obey_thread.unlock();
							CloseConnection("Recv<0");
							return;
						}
					}

					if (bytesReceived > 0)
					{
						connection.pm_incoming.currentPacketExtractionOffset += bytesReceived;
						if (connection.pm_incoming.currentTask == PacketManagerTask::ProcessPacketSize)
						{
							if (connection.pm_incoming.currentPacketExtractionOffset == sizeof(uint32_t))
							{
								connection.pm_incoming.currentPacketSize = ntohs(connection.pm_incoming.currentPacketSize);
								if (connection.pm_incoming.currentPacketSize > PNet::g_MaxPacketSize)
								{
									mtx_obey_thread.unlock();
									CloseConnection("Packet size too large.");
									return;
								}
								connection.pm_incoming.currentPacketExtractionOffset = 0;
								connection.pm_incoming.currentTask = PacketManagerTask::ProcessPacketContents;
							}
						}
						else // Processing packet contents
						{
							if (connection.pm_incoming.currentPacketExtractionOffset == connection.pm_incoming.currentPacketSize)
							{
								std::shared_ptr<Packet> packet = std::make_shared<Packet>();
								packet->buffer.resize(connection.pm_incoming.currentPacketSize);
								memcpy(&packet->buffer[0], connection.buffer, connection.pm_incoming.currentPacketSize);

								connection.pm_incoming.Append(packet);

								connection.pm_incoming.currentPacketSize = 0;
								connection.pm_incoming.currentPacketExtractionOffset = 0;
								connection.pm_incoming.currentTask = PacketManagerTask::ProcessPacketSize;

								// while (connection.pm_incoming.HasPendingPackets())
								// {
								std::shared_ptr<Packet> frontPacket = connection.pm_incoming.Retrieve();
								if (!ProcessPacket(frontPacket))
								{
									mtx_obey_thread.unlock();
									CloseConnection("Failed to process incoming packet.");
									return;
								}
								connection.pm_incoming.Pop();
								// }
							}
						}
					}
				}
			}
			mtx_obey_thread.unlock();
		}
		// MessageBox(NULL, TEXT("dong Obey tu nhien"), TEXT("Loi"), MB_ICONERROR | MB_OK);
	}

	void Server::Livestream()
	{
		while (this->isConnected)
		{
			mtx_livestream_thread.lock();
			WSAPOLLFD use_fd = newConnectionFD;
			if (WSAPoll(&use_fd, 1, 1) > 0)
			{

				if (use_fd.revents & POLLERR) // If error occurred on this socket
				{
					// MessageBox(NULL, TEXT("pollerr tai livestream"), TEXT("Loi"), MB_ICONERROR | MB_OK);
					mtx_livestream_thread.unlock();
					CloseConnection("POLLERR");
					return;
				}

				if (use_fd.revents & POLLHUP) // If poll hangup occurred on this socket
				{
					// MessageBox(NULL, TEXT("pollup tai livestream"), TEXT("Loi"), MB_ICONERROR | MB_OK);
					mtx_livestream_thread.unlock();
					CloseConnection("POLLHUP");
					return;
				}

				if (use_fd.revents & POLLNVAL) // If invalid socket
				{
					// MessageBox(NULL, TEXT("pollnval tai livestream"), TEXT("Loi"), MB_ICONERROR | MB_OK);
					mtx_livestream_thread.unlock();
					CloseConnection("POLLNVAL");
					return;
				}

				if (use_fd.revents & POLLWRNORM) // If normal data can be written without blocking
				{
					PacketManager &pm = connection.pm_outgoing;
					while (pm.HasPendingPackets())
					{
						if (pm.currentTask == PacketManagerTask::ProcessPacketSize) // Sending packet size
						{
							pm.currentPacketSize = pm.Retrieve()->buffer.size();
							if (pm.currentPacketSize > PNet::g_MaxPacketSize)
							{
								// std::cout << "vuot\n";
								pm.Pop();
								continue;
							}

							uint32_t bigEndianPacketSize = htonl(pm.currentPacketSize);
							int bytesSent = send(use_fd.fd, (char *)(&bigEndianPacketSize) + pm.currentPacketExtractionOffset, sizeof(uint32_t) - pm.currentPacketExtractionOffset, 0);
							if (bytesSent > 0)
							{
								pm.currentPacketExtractionOffset += bytesSent;
							}

							if (pm.currentPacketExtractionOffset == sizeof(uint32_t)) // If full packet size was sent
							{
								pm.currentPacketExtractionOffset = 0;
								pm.currentTask = PacketManagerTask::ProcessPacketContents;
							}
							else // If full packet size was not sent, break out of the loop for sending outgoing packets for this connection - we'll have to try again next time we are able to write normal data without blocking
							{
								break;
							}
						}
						else // Sending packet contents
						{
							char *bufferPtr = &pm.Retrieve()->buffer[0];
							int bytesSent = send(use_fd.fd, (char *)(bufferPtr) + pm.currentPacketExtractionOffset, pm.currentPacketSize - pm.currentPacketExtractionOffset, 0);
							if (bytesSent > 0)
							{
								pm.currentPacketExtractionOffset += bytesSent;
							}

							if (pm.currentPacketExtractionOffset == pm.currentPacketSize) // If full packet contents have been sent
							{
								pm.currentPacketExtractionOffset = 0;
								pm.currentTask = PacketManagerTask::ProcessPacketSize;
								pm.Pop(); // Remove packet from queue after finished processing
							}
							else
							{
								break; // Added after tutorial was made 2019-06-24
							}
						}
					}
				}
			}

			Video();
			// int key = waitKey(1);
			// if (key == 'x')
			// 	return;

			mtx_livestream_thread.unlock();
		}
		// MessageBox(NULL, TEXT("dong livestream mot cach tu nhien"), TEXT("Loi"), MB_ICONERROR | MB_OK);
	}

	void Server::OnDisconnect(std::string reason)
	{
		std::cout << "Lost connection. Reason: " << reason << "." << std::endl;
	}
	void Server::OnConnect()
	{
		std::cout << "Successfully connected!" << std::endl;
	}

	
	void Server::CloseConnection(std::string reason)
	{
		if(isConnected == false) return;
		isConnected = false;
		// MessageBox(NULL, TEXT("dang dong ket loi"), TEXT("Loi"), MB_ICONERROR | MB_OK);

		mtx_livestream_thread.lock();
		mtx_obey_thread.lock();
		OnDisconnect(reason);
		listeningSocketFD.fd = 0;
		connection.Close();

		mtx_obey_thread.unlock();
		mtx_livestream_thread.unlock();
	}

	bool Server::ProcessPacket(std::shared_ptr<Packet> packet)
	{
		std::cout << "Packet received with size: " << packet->buffer.size() << std::endl;
		return true;
	}

	cv::Mat Server::captureScreen(HWND hwnd, int targetWidth, int targetHeight)
	{
		HDC hwindowDC, hwindowCompatibleDC;
		HBITMAP hbwindow;
		cv::Mat src;
		BITMAPINFOHEADER bi;

		hwindowDC = GetDC(hwnd);
		hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
		SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

		RECT windowsize;
		GetClientRect(hwnd, &windowsize);

		int srcheight = windowsize.bottom;
		int srcwidth = windowsize.right;
		// int height = windowsize.bottom / 1;
		// int width = windowsize.right / 1;
		int height = targetHeight / 1;
		int width = targetWidth / 1;

		src.create(height, width, CV_8UC3);

		hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = width;
		bi.biHeight = -height;
		bi.biPlanes = 1;
		bi.biBitCount = 24;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		SelectObject(hwindowCompatibleDC, hbwindow);
		StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY);
		GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);

		DeleteObject(hbwindow);
		DeleteDC(hwindowCompatibleDC);
		ReleaseDC(hwnd, hwindowDC);

		return src;
	}

	void Server::Video()
	{

		HWND hwndDesktop = GetDesktopWindow();
		// Mat img = captureScreen(hwndDesktop, 1280, 720);
		Mat img = captureScreen(hwndDesktop, 1000, 500);
		// if (img.empty())
		// 	return;

		std::vector<uchar> bufferVec;
		std::vector<int> compression_params;
		compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
		compression_params.push_back(90); // Đặt chất lượng JPEG (giá trị từ 0-100)

		imencode(".jpg", img, bufferVec, compression_params);

		// namedWindow("Server", WINDOW_NORMAL);
		// imshow("Server", img);
		
		std::shared_ptr<Packet> packet = std::make_shared<Packet>(PacketType::PT_Image);
		*packet << bufferVec;
		connection.pm_outgoing.Append(packet);
	}
}