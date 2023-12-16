#include "Server.h"
#include <iostream>
#include "Network.h"
#include <mutex>

namespace PNet
{
<<<<<<< HEAD
	std::mutex mtxs;

	std::string Server::GetIPv4Address()
	{
		char hostbuffer[256];
		gethostname(hostbuffer, sizeof(hostbuffer));

		struct hostent* host_entry =  gethostbyname(hostbuffer);

		this->IPv4Address = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
		return IPv4Address;
	}

	bool Server::Initialize()
	{
		IPEndpoint ip("0.0.0.0", 6112);
		listeningSocket = Socket(ip.GetIPVersion());
		if (listeningSocket.Create() == PResult::P_Success)
		{
			// std::cout << "Socket successfully created." << std::endl;
=======
	bool Server::Initialize(IPEndpoint ip)
	{
		listeningSocket = Socket(ip.GetIPVersion());
		if (listeningSocket.Create() == PResult::P_Success)
		{
			std::cout << "Socket successfully created." << std::endl;
>>>>>>> remote-origin/nguyenquangthinh
			if (listeningSocket.Listen(ip) == PResult::P_Success)
			{
				listeningSocketFD.fd = listeningSocket.GetHandle();
				listeningSocketFD.events = POLLRDNORM;
				listeningSocketFD.revents = 0;

<<<<<<< HEAD
				// std::cout << "Socket is listening" << std::endl;
=======
				std::cout << "Socket successfully listening." << std::endl;
>>>>>>> remote-origin/nguyenquangthinh
				return true;
			}
			else
			{
<<<<<<< HEAD
				// std::cerr << "Failed to listen." << std::endl;
=======
				std::cerr << "Failed to listen." << std::endl;
>>>>>>> remote-origin/nguyenquangthinh
			}
			listeningSocket.Close();
		}
		else
		{
<<<<<<< HEAD
			// std::cerr << "Socket failed to create." << std::endl;
=======
			std::cerr << "Socket failed to create." << std::endl;
>>>>>>> remote-origin/nguyenquangthinh
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
<<<<<<< HEAD
						// std::cerr << "Failed to accept new connection." << std::endl;
=======
						std::cerr << "Failed to accept new connection." << std::endl;
>>>>>>> remote-origin/nguyenquangthinh
					}
				}
			}
		}
	}

	void Server::Obey()
	{
		while (this->isConnected)
		{
			WSAPOLLFD use_fd = newConnectionFD;
			if (WSAPoll(&use_fd, 1, 1) > 0)
			{
				if (use_fd.revents & POLLERR) // If error occurred on this socket
				{
					CloseConnection("POLLERR");
					return;
				}

				if (use_fd.revents & POLLHUP) // If poll hangup occurred on this socket
				{
					CloseConnection("POLLHUP");
					return;
				}

				if (use_fd.revents & POLLNVAL) // If invalid socket
				{
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
						CloseConnection("Recv==0");
						return;
					}

					if (bytesReceived == SOCKET_ERROR) // If error occurred on socket
					{
						int error = WSAGetLastError();
						if (error != WSAEWOULDBLOCK)
						{
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
<<<<<<< HEAD
									// std::cout << "to vay ne (obey) " << connection.pm_incoming.currentPacketSize << "\n";
=======
									std::cout << "to vay ne (obey) " << connection.pm_incoming.currentPacketSize << "\n";
>>>>>>> remote-origin/nguyenquangthinh
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
									CloseConnection("Failed to process incoming packet.");
									return;
								}
								connection.pm_incoming.Pop();
<<<<<<< HEAD
								}
							// }
=======
								// }
							}
>>>>>>> remote-origin/nguyenquangthinh
						}
					}
				}
			}
		}
	}

	void Server::Livestream()
	{
		while (this->isConnected)
		{
			WSAPOLLFD use_fd = newConnectionFD;
			if (WSAPoll(&use_fd, 1, 1) > 0)
			{

				if (use_fd.revents & POLLERR) // If error occurred on this socket
				{
					CloseConnection("POLLERR");
					return;
				}

				if (use_fd.revents & POLLHUP) // If poll hangup occurred on this socket
				{
					CloseConnection("POLLHUP");
					return;
				}

				if (use_fd.revents & POLLNVAL) // If invalid socket
				{
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
<<<<<<< HEAD
								// std::cout << "vuot\n";
=======
								std::cout << "vuot\n";
>>>>>>> remote-origin/nguyenquangthinh
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
			int key = waitKey(1);
			if (key == 'x')
				return;
		}
	}

	void Server::OnDisconnect(std::string reason)
	{
<<<<<<< HEAD
		// std::cout << "Lost connection. Reason: " << reason << "." << std::endl;
	}
	void Server::OnConnect()
	{
		// std::cout << "Successfully connected!" << std::endl;
	}

	void Server::CloseConnection(std::string reason)
	{
		mtxs.lock();
=======
		std::cout << "Lost connection. Reason: " << reason << "." << std::endl;
	}
	void Server::OnConnect()
	{
		std::cout << "Successfully connected!" << std::endl;
	}

	std::mutex mtx;
	void Server::CloseConnection(std::string reason)
	{
		mtx.lock();
>>>>>>> remote-origin/nguyenquangthinh
		if(isConnected == false) return;

		isConnected = false;
		
		OnDisconnect(reason);
		listeningSocketFD.fd = 0;
<<<<<<< HEAD
		listeningSocket.Close();
		connection.Close();

		mtxs.unlock();
=======
		connection.Close();

		mtx.unlock();
>>>>>>> remote-origin/nguyenquangthinh
	}

	bool Server::ProcessPacket(std::shared_ptr<Packet> packet)
	{
<<<<<<< HEAD
		// std::cout << "Packet received with size: " << packet->buffer.size() << std::endl;
=======
		std::cout << "Packet received with size: " << packet->buffer.size() << std::endl;
>>>>>>> remote-origin/nguyenquangthinh
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
		Mat img = captureScreen(hwndDesktop, 1280, 720);
		if (img.empty())
			return;

		std::vector<uchar> bufferVec;
		std::vector<int> compression_params;
		compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
		compression_params.push_back(60); // Đặt chất lượng JPEG (giá trị từ 0-100)

		imencode(".jpg", img, bufferVec, compression_params);

		// namedWindow("Server", WINDOW_NORMAL);
		// imshow("Server", img);
		std::string bufferStr(bufferVec.begin(), bufferVec.end());
<<<<<<< HEAD
		// std::cout << "kick thuoc ne " << bufferStr.size() << "\n";
=======
		std::cout << "kick thuoc ne " << bufferStr.size() << "\n";
>>>>>>> remote-origin/nguyenquangthinh
		std::shared_ptr<Packet> packet = std::make_shared<Packet>(PacketType::PT_Image);
		*packet << bufferStr;
		connection.pm_outgoing.Append(packet);
	}
}