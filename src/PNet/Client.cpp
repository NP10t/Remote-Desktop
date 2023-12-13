#include "Client.h"
#include <chrono>
#include <conio.h>
#include <iostream>
#include <Windows.h>


namespace PNet
{
	int keyboard = -1, ctrlD = 0, shiftD = 0, ctrlU = 0, shiftU = 0, Caplock = 0, delta = 0;
	bool Client::Connect(IPEndpoint ip)
	{
		Socket socket = Socket(ip.GetIPVersion());
		if (socket.Create() == PResult::P_Success)
		{
			if (socket.SetBlocking(true) != PResult::P_Success)
				return false;

			std::cout << "Socket successfully created." << std::endl;
			// if (socket.Connect(ip) != PResult::P_Success)
			while (1)
			{
				if (socket.Connect(ip) != PResult::P_Success)
				{
					continue; // neu client mo truoc server thi client wait until server start to listen
				}

				if (socket.SetBlocking(false) == PResult::P_Success)
				{
					TCPConnection newConnection = TCPConnection(socket, ip);
					connections.emplace_back(newConnection); // khi co ket noi toi thi bo vao buffer
					WSAPOLLFD newConnectionFD = {};
					newConnectionFD.fd = newConnection.socket.GetHandle();
					newConnectionFD.events = POLLRDNORM | POLLWRNORM;
					newConnectionFD.revents = 0;

					master_fd.push_back(newConnectionFD); // khi co ket noi toi thi bo vao buffer
					// selected_device = 0;

					OnConnect(newConnection);
					return true;
				}
			}
			// else
			// {
			// }
			socket.Close();
		}
		else
		{
			std::cerr << "Socket failed to create." << std::endl;
		}
		OnConnectFail();
		return false;
	}

	bool Client::Frame(int current_device)
	{
		if(!this->control.joinable()){
		this->control = std::thread(&Client::ControlUsingTCP, this, current_device);
		this->control.detach();
		}
		else
			MessageBox(NULL, TEXT("ko tao thread control dc"), TEXT("Lỗi"), MB_ICONERROR | MB_OK);


		if(!this->video.joinable()){
		// MessageBox(NULL, TEXT("tao thread video ne"), TEXT("9"), MB_ICONERROR | MB_OK);
		this->video = std::thread(&Client::PlayVideo, this, current_device);
		this->video.detach();
		}
		else
			MessageBox(NULL, TEXT("ko tao thread video dc"), TEXT("Lỗi"), MB_ICONERROR | MB_OK);

		return true;
	}

	void Client::ControlUsingTCP(int current_device)
	{
		// while(1)
		// {
		// 	if (selected_device == -1) continue;
		// 	int i = selected_device;
			
		while (current_device == selected_device)
		{
			int i = current_device;
			use_fd = master_fd;
			if (use_fd.size() && WSAPoll(&use_fd[i], 1, 1) > 0)
			{
				// for (int i = use_fd.size() - 1; i >= 1; i--){
				int connectionIndex = i;
				TCPConnection &connection = connections[connectionIndex];

				if (use_fd[i].revents & POLLERR) // If error occurred on this socket
				{
					CloseConnection(connectionIndex, "POLLERR");
					continue;
				}

				if (use_fd[i].revents & POLLHUP) // If poll hangup occurred on this socket
				{
					CloseConnection(connectionIndex, "POLLHUP");
					continue;
				}

				if (use_fd[i].revents & POLLNVAL) // If invalid socket
				{
					CloseConnection(connectionIndex, "POLLNVAL");
					continue;
				}

				if (use_fd[i].revents & POLLWRNORM) // If normal data can be written without blocking
				{
					Mouse(connections[i]);
					Keyboard(connections[i]);
					PacketManager &pm = connection.pm_outgoing;
					while (pm.HasPendingPackets())
					{
						if (pm.currentTask == PacketManagerTask::ProcessPacketSize) // Sending packet size
						{
							pm.currentPacketSize = pm.Retrieve()->buffer.size();
							uint16_t bigEndianPacketSize = htons(pm.currentPacketSize);
							int bytesSent = send(use_fd[i].fd, (char *)(&bigEndianPacketSize) + pm.currentPacketExtractionOffset, sizeof(uint32_t) - pm.currentPacketExtractionOffset, 0);
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
							int bytesSent = send(use_fd[i].fd, (char *)(bufferPtr) + pm.currentPacketExtractionOffset, pm.currentPacketSize - pm.currentPacketExtractionOffset, 0);
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
				// }
			}
		}
	}

	void Client::PlayVideo(int current_device)
	{
		// while(1)
		// {
		// 	if (selected_device == -1) continue;
		// 	int i = selected_device;

		while (current_device == selected_device)
		{
			int i = current_device;
			use_fd = master_fd;
			if (use_fd.size() && WSAPoll(&use_fd[i], 1, 1) > 0)
			{
				// for (int i = use_fd.size() - 1; i >= 1; i--)
				// {
				int connectionIndex = i;
				TCPConnection &connection = connections[connectionIndex];

				if (use_fd[i].revents & POLLERR) // If error occurred on this socket
				{
					CloseConnection(connectionIndex, "POLLERR");
					continue;
				}

				if (use_fd[i].revents & POLLHUP) // If poll hangup occurred on this socket
				{
					CloseConnection(connectionIndex, "POLLHUP");
					continue;
				}

				if (use_fd[i].revents & POLLNVAL) // If invalid socket
				{
					CloseConnection(connectionIndex, "POLLNVAL");
					continue;
				}

				if (use_fd[i].revents & POLLRDNORM) // If normal data can be read without blocking
				{
					uint32_t bytesReceived = 0;

					if (connection.pm_incoming.currentTask == PacketManagerTask::ProcessPacketSize)
					{

						bytesReceived = recv(use_fd[i].fd, (char *)&connection.pm_incoming.currentPacketSize + connection.pm_incoming.currentPacketExtractionOffset, sizeof(uint32_t) - connection.pm_incoming.currentPacketExtractionOffset, 0);
					}
					else // Process Packet Contents
					{
						bytesReceived = recv(use_fd[i].fd, (char *)&connection.buffer + connection.pm_incoming.currentPacketExtractionOffset, connection.pm_incoming.currentPacketSize - connection.pm_incoming.currentPacketExtractionOffset, 0);
					}

					if (bytesReceived == 0) // If connection was lost
					{
						CloseConnection(connectionIndex, "Recv==0");
						continue;
					}

					if (bytesReceived == SOCKET_ERROR) // If error occurred on socket
					{
						int error = WSAGetLastError();
						if (error != WSAEWOULDBLOCK)
						{
							CloseConnection(connectionIndex, "Recv<0");
							continue;
						}
					}

					if (bytesReceived > 0)
					{
						connection.pm_incoming.currentPacketExtractionOffset += bytesReceived;
						if (connection.pm_incoming.currentTask == PacketManagerTask::ProcessPacketSize)
						{
							if (connection.pm_incoming.currentPacketExtractionOffset == sizeof(uint32_t))
							{
								connection.pm_incoming.currentPacketSize = ntohl(connection.pm_incoming.currentPacketSize);
								if (connection.pm_incoming.currentPacketSize > PNet::g_MaxPacketSize)
								{
									std::cout << "to vay ne (play video) " << connection.pm_incoming.currentPacketSize << "\n";
									CloseConnection(connectionIndex, "Packet size too large.");
									continue;
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
								// while (connections[i-1].pm_incoming.HasPendingPackets())
								// {
								// MessageBox(NULL, TEXT("5"), TEXT("2"), MB_ICONERROR | MB_OK);
								std::shared_ptr<Packet> frontPacket = connections[i].pm_incoming.Retrieve();
								if (!ProcessPacket(frontPacket))
								{
									CloseConnection(i, "Failed to process incoming packet.");
									break;
								}
								// MessageBox(NULL, TEXT("6"), TEXT("2"), MB_ICONERROR | MB_OK);
								connections[i].pm_incoming.Pop();
								// if(current_device != selected_device) return;
								int key = waitKey(1);
								if (key == 'x')
									return;
								// }
							}
						}
					}
					// }
				}
			}
		}
		destroyAllWindows();
	}

	bool Client::ProcessPacket(std::shared_ptr<Packet> packet)
	{
		std::cout << "Packet received with size: " << packet->buffer.size() << std::endl;
		return true;
	}
	void Client::OnConnect(TCPConnection &newConnection)
	{
		std::cout << newConnection.ToString() << " - New connection accepted." << std::endl;
	}

	void Client::OnDisconnect(TCPConnection &lostConnection, std::string reason)
	{
		std::cout << "[" << reason << "] Connection lost: " << lostConnection.ToString() << "." << std::endl;
	}

	void Client::OnConnectFail()
	{
		std::cout << "Failed to connect." << std::endl;
	}

	void Client::CloseConnection(int connectionIndex, std::string reason)
	{
		std::wstring wideReason(reason.begin(), reason.end());
		LPCTSTR wideReasonPtr = wideReason.c_str();
		MessageBox(NULL, wideReasonPtr, TEXT("client"), MB_ICONERROR | MB_OK);
		std::wstring strSelectedDevice = std::to_wstring(connections.size());
		LPCTSTR lpSelectedDevice = strSelectedDevice.c_str();
		MessageBox(NULL, lpSelectedDevice, TEXT("size of connections (client, 1)"), MB_ICONERROR | MB_OK);
		
		if(connectionIndex == -1) return;
		selected_device = -1;
		TCPConnection &connection = connections[connectionIndex];
		OnDisconnect(connection, reason);
		master_fd.erase(master_fd.begin() + (connectionIndex));

		std::wstring strSelectedDevice2 = std::to_wstring(use_fd.size());
		LPCTSTR lpSelectedDevice2 = strSelectedDevice2.c_str();
		MessageBox(NULL, lpSelectedDevice2, TEXT("size of use_fd (client, 1)"), MB_ICONERROR | MB_OK);
		use_fd.erase(use_fd.begin() + (connectionIndex));
		connection.Close();
		
		std::wstring strSelectedDevice1 = std::to_wstring(connections.size());
		LPCTSTR lpSelectedDevice1 = strSelectedDevice1.c_str();
		MessageBox(NULL, lpSelectedDevice1, TEXT("size of connections (client, 2)"), MB_ICONERROR | MB_OK);
		
		connections.erase(connections.begin() + connectionIndex);
	}

	HHOOK mh;

	LRESULT CALLBACK mouse(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode < 0)
		{
			return CallNextHookEx(mh, nCode, wParam, lParam);
		}

		MSLLHOOKSTRUCT *pMouseStruct = (MSLLHOOKSTRUCT *)lParam;

		if (pMouseStruct != NULL)
		{
			if (wParam == WM_MOUSEWHEEL)
			{
				// Determine the direction of the mouse wheel scroll
				delta = GET_WHEEL_DELTA_WPARAM(pMouseStruct->mouseData) > 0 ? 1 : -1;
			}
			else
			{
				delta = 0;
			}
		}

		return CallNextHookEx(mh, nCode, wParam, lParam);
	}

	void ProcessMessages()
	{
		// Process messages in the message queue
		MSG message;
		PeekMessage(&message, NULL, 0, 0, PM_REMOVE);
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	void Client::Mouse(TCPConnection &connection)
	{
		mh = SetWindowsHookExA(WH_MOUSE_LL, mouse, NULL, 0);
		auto startLoopTime = std::chrono::high_resolution_clock::now();

		while (true)
		{
			ProcessMessages();
			auto endLoopTime = std::chrono::high_resolution_clock::now();
			auto loopDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endLoopTime - startLoopTime);
			if (loopDuration.count() >= 10 || delta != 0)
				break;
		}

		int LCD = 0, RCD = 0, LCU = 0, RCU = 0;
		if (GetAsyncKeyState(VK_SCROLL) & 0x8001)
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
				LCD = 1;
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8001)
			RCD = 1;
		if (LCD && !(GetAsyncKeyState(VK_LBUTTON) & 0x8001))
			LCU = 1;
		if (RCD && !(GetAsyncKeyState(VK_RBUTTON) & 0x8001))
			RCU = 1;
		POINT xy;
		GetCursorPos(&xy);
		int x = xy.x;
		int y = xy.y;
		std::shared_ptr<Packet> packet = std::make_shared<Packet>(PacketType::PT_Mouse);
		*packet << x << y << LCD << RCD << LCU << RCU << delta;
		delta = 0;
		connection.pm_outgoing.Append(packet);
	}

	void Client::Keyboard(TCPConnection &connection)
	{
		if (GetKeyState(VK_SHIFT) & 0x8000)
			shiftD = 1;
		if (GetKeyState(VK_CONTROL) & 0x8000)
			ctrlD = 1;
		// ctrl = 17/162, shift = 16,160
		for (int i = 5; i < 255; ++i)
		{
			if (GetAsyncKeyState(i) & 0x8001)
			{
				if (i == 16 || i == 160 || i == 17 || i == 162 || i == 179)
					continue;
				keyboard = i;
				if (!(GetKeyState(VK_CAPITAL) & 0x0001) && i >= 65 && i <= 65 + 'Z' - 'A')
					keyboard += 32;
				Sleep(80);
				break;
			}
		}
		if (shiftD && !(GetKeyState(VK_SHIFT) & 0x8000))
		{
			shiftD = 0;
			shiftU = 1;
		}
		if (ctrlD && !(GetKeyState(VK_CONTROL) & 0x8000))
		{
			ctrlD = 0;
			ctrlU = 1;
		}
		if (GetKeyState(VK_CAPITAL) & 0x0001)
			Caplock = 1;
		else
			Caplock = 0;
		if (ctrlU == 1)
			ctrlU = 0;
		if (shiftU == 1)
			shiftU = 0;
		std::shared_ptr<Packet> packet = std::make_shared<Packet>(PacketType::PT_Keyboard);
		*packet << keyboard << shiftD << shiftU << ctrlD << ctrlU << Caplock;
		keyboard = -1;
		connection.pm_outgoing.Append(packet);
	}
}