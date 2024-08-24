#include "Client.h"
#include <chrono>
#include <conio.h>
#include <iostream>
#include <Windows.h>
#include <algorithm>


namespace PNet
{	
	bool Client::Initialize(IPEndpoint ip, std::vector<WSAPOLLFD> &master_fd, std::vector<TCPConnection> &connections)
	{
		Socket socket = Socket(ip.GetIPVersion());
		if (socket.Create() == PResult::P_Success)
		{
			if (socket.SetBlocking(true) != PResult::P_Success)
				return false;

			while (1)
			{
				if (socket.Connect(ip) != PResult::P_Success) 
				{
					continue; // neu client mo truoc server thi client wait until server start to listen
				}
				// MessageBox(NULL, TEXT("Socket connected"), TEXT("Socket"), MB_ICONERROR | MB_OK);

				if (socket.SetBlocking(false) == PResult::P_Success)
				{
					TCPConnection newConnection = TCPConnection(socket, ip);
					connections.emplace_back(newConnection); // khi co ket noi toi thi bo vao buffer
					WSAPOLLFD newConnectionFD = {};
					newConnectionFD.fd = newConnection.socket.GetHandle();
					newConnectionFD.events = POLLRDNORM | POLLWRNORM;
					newConnectionFD.revents = 0;

					master_fd.push_back(newConnectionFD); // khi co ket noi toi thi bo vao buffer
					OnConnect(newConnection);
					return true;
				}
			}
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
			MessageBox(NULL, TEXT("ko tao thread control dc"), TEXT("Loi"), MB_ICONERROR | MB_OK);


		if(!this->video.joinable()){
		this->video = std::thread(&Client::PlayVideo, this, current_device);
		this->video.detach();
		}
		else
			MessageBox(NULL, TEXT("ko tao thread video dc"), TEXT("Loi"), MB_ICONERROR | MB_OK);

		return true;
	}

	void Client::ControlUsingTCP(int current_device) {
		while (current_device == selected_device) // khi chuyen sang thiet bi khac thi current_device != selected_device => huy luon thread nay, tao lai thread khac 
		{
			// mtx_control_thread.lock();
			if(controlMaster_fd.size() < 1) continue;
			int connectionIndex = current_device;
			WSAPOLLFD use_fd = controlMaster_fd[connectionIndex];

			if (WSAPoll(&use_fd, 1, 1) > 0)
			{
				TCPConnection &connection = controlConnections[connectionIndex];

				if (use_fd.revents & POLLERR) // If error occurred on this socket
				{
					// mtx_control_thread.unlock();
					CloseConnection(connectionIndex, "POLLERR_control", controlConnections, controlMaster_fd);
					return;
				}

				if (use_fd.revents & POLLHUP) // If poll hangup occurred on this socket
				{
					// mtx_control_thread.unlock();
					CloseConnection(connectionIndex, "POLLHUP", controlConnections, controlMaster_fd);
					return;
				}

				if (use_fd.revents & POLLNVAL) // If invalid socket
				{
					// mtx_control_thread.unlock();
					CloseConnection(connectionIndex, "POLLNVAL", controlConnections, controlMaster_fd);
					return;
				}
				if (use_fd.revents & POLLWRNORM) // If normal data can be written without blocking
				{
					Commander(controlConnections[connectionIndex]);
					PacketManager &pm = connection.pm_outgoing;
					while (pm.HasPendingPackets())
					{
						if (pm.currentTask == PacketManagerTask::ProcessPacketSize) // Sending packet size
						{
							pm.currentPacketSize = pm.Retrieve()->buffer.size();
							uint16_t bigEndianPacketSize = htons(pm.currentPacketSize);
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
								// mtx_control_thread.unlock();
								return;
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
								// mtx_control_thread.unlock();
								return; // Added after tutorial was made 2019-06-24
							}
						}
					}
				}
			}
			// mtx_control_thread.unlock();
		}
		// MessageBox(NULL, TEXT("dong control thread client tu nhien"), TEXT("Loi"), MB_ICONERROR | MB_OK);
	}

	void Client::PlayVideo(int current_device)
	{
		namedWindow("Screen of controlled deviced", WINDOW_NORMAL);
		while (current_device == selected_device) // khi chuyen sang thiet bi khac thi current_device != selected_device => huy luon thread nay, tao lai thread khac 
		{	
			// mtx_playvideo_thread.lock();
			// if(master_fd.size() < 1) continue;
			if(controlMaster_fd.size() < 1) continue;
			int connectionIndex = current_device;
			WSAPOLLFD use_fd = playvideoMaster_fd[connectionIndex];

			if (WSAPoll(&use_fd, 1, 1) > 0)
			{
				TCPConnection &connection = playvideoConnections[connectionIndex];

				if (use_fd.revents & POLLRDNORM) // If normal data can be read without blocking
				{
					uint32_t bytesReceived = 0;

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
						MessageBox(NULL, TEXT("lost connection tai video"), TEXT("Loi"), MB_ICONERROR | MB_OK);
						// mtx_playvideo_thread.unlock();
						CloseConnection(connectionIndex, "Recv==0", playvideoConnections, playvideoMaster_fd);
						return;
					}

					if (bytesReceived == SOCKET_ERROR) // If error occurred on socket
					{
						int error = WSAGetLastError();
						if (error != WSAEWOULDBLOCK)
						{
							MessageBox(NULL, TEXT("error occur tai video"), TEXT("Loi"), MB_ICONERROR | MB_OK);
							// mtx_playvideo_thread.unlock();
							CloseConnection(connectionIndex, "Recv<0", playvideoConnections, playvideoMaster_fd);
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
								connection.pm_incoming.currentPacketSize = ntohl(connection.pm_incoming.currentPacketSize);
								if (connection.pm_incoming.currentPacketSize > PNet::g_MaxPacketSize)
								{
									// mtx_playvideo_thread.unlock();
									CloseConnection(connectionIndex, "Packet size too large.", playvideoConnections, playvideoMaster_fd);
									return;
								}
								connection.pm_incoming.currentPacketExtractionOffset = 0;
								connection.pm_incoming.currentTask = PacketManagerTask::ProcessPacketContents;
							}
						}
						else // Processing packet contents
						{
							if (connection.pm_incoming.currentPacketExtractionOffset == connection.pm_incoming.currentPacketSize) {
							// 	std::shared_ptr<Packet> packet = std::make_shared<Packet>();
							// 	packet->buffer.resize(connection.pm_incoming.currentPacketSize);
							// 	memcpy(&packet->buffer[0], connection.buffer, connection.pm_incoming.currentPacketSize);

							// 	connection.pm_incoming.currentPacketSize = 0;
							// 	connection.pm_incoming.currentPacketExtractionOffset = 0;
							// 	connection.pm_incoming.currentTask = PacketManagerTask::ProcessPacketSize;

							// 	ProcessPacket(packet);
							// 	int key = waitKey(1);

							// }
								int x = 6;
								std::vector<uchar> bufferVec(connection.pm_incoming.currentPacketSize - x);
								memcpy(bufferVec.data(), connection.buffer + x, bufferVec.size());

								connection.pm_incoming.currentPacketSize = 0;
								connection.pm_incoming.currentPacketExtractionOffset = 0;
								connection.pm_incoming.currentTask = PacketManagerTask::ProcessPacketSize;

								img = imdecode(bufferVec, IMREAD_COLOR);
								// if (img.empty())
								// {
								// 	std::cout << "invalib image\n";
								// 	return;
								// }

								imshow("Screen of controlled deviced", img);	

								int key = waitKey(1);		
							}
						}
					}
				}
				else if (use_fd.revents & POLLERR) // If error occurred on this socket
				{
					// mtx_playvideo_thread.unlock();
					CloseConnection(connectionIndex, "POLLERR_video", playvideoConnections, playvideoMaster_fd);
					return;
				}

				else if (use_fd.revents & POLLHUP) // If poll hangup occurred on this socket
				{
					// mtx_playvideo_thread.unlock();
					CloseConnection(connectionIndex, "POLLHUP", playvideoConnections, playvideoMaster_fd);
					return;
				}

				else if (use_fd.revents & POLLNVAL) // If invalid socket
				{
					// mtx_playvideo_thread.unlock();
					CloseConnection(connectionIndex, "POLLNVAL", playvideoConnections, playvideoMaster_fd);
					return;
				}
			}
			// mtx_playvideo_thread.unlock();
		}
		destroyAllWindows();
		// MessageBox(NULL, TEXT("dong video thread client tu nhien"), TEXT("Loi"), MB_ICONERROR | MB_OK);
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


	void Client::CloseConnection(int connectionIndex, std::string reason, std::vector<TCPConnection> &connections, std::vector<WSAPOLLFD> &master_fd)
	{	
		if(selected_device_connected == false || connectionIndex == -1) { // disconnect roi thi ko disconnect nua, hoac neu thiet bi thu -1 thi ko disconnect
			return;
		}

		selected_device = -1; // gan selected_device = -1 de current_device != selected_device => huy luon 2 thread control va video,  sau nay neu muon thi tao lai thread khac 
		selected_device_connected = false; 

		// mtx_playvideo_thread.lock();
		// mtx_control_thread.lock();

		TCPConnection &connection = connections[connectionIndex];
		OnDisconnect(connection, reason);
		master_fd.erase(master_fd.begin() + (connectionIndex));
		connection.Close();
		connections.erase(connections.begin() + connectionIndex);

		// mtx_control_thread.unlock();
		// mtx_playvideo_thread.unlock();
	}

	HHOOK mh;
	float LCD = 0, RCD = 0, LCU = 0, RCU = 0;
	float keyboard = -1, ctrlD = 0, shiftD = 0, ctrlU = 0, shiftU = 0, Caplock = 0, delta = 0, altD=0, altU=0;

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

	void sleeptime(int n)
	{
		auto startLoopTime = std::chrono::high_resolution_clock::now();
		while(1)
		{
			auto endLoopTime = std::chrono::high_resolution_clock::now();
			auto loopDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endLoopTime - startLoopTime);
			if (loopDuration.count() >= n) break;
		}
	}

	void Client::Commander(TCPConnection& connection) {
		mh = SetWindowsHookExA(WH_MOUSE_LL, mouse, NULL, 0);
		auto startLoopTime = std::chrono::high_resolution_clock::now();

		while (true) {
			ProcessMessages();
			auto endLoopTime = std::chrono::high_resolution_clock::now();
			auto loopDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endLoopTime - startLoopTime);
			if (loopDuration.count() >= 10) break;
		}

        if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
		{
			LCD = 1;
			LCU = 0;
			sleeptime(100);
		}

        if (GetAsyncKeyState(VK_RBUTTON) & 0x8001)
		{
			RCD = 1;
			RCU = 0;
			sleeptime(100);
		}

        if (LCD && !(GetAsyncKeyState(VK_LBUTTON) & 0x8001))
		{
			LCU = 1;
			LCD = 0;
			sleeptime(100);
		}
        if (RCD && !(GetAsyncKeyState(VK_RBUTTON) & 0x8001))
		{
			RCU = 1;
			RCD = 0;
			sleeptime(100);
		}


        if (GetKeyState(VK_SHIFT) & 0x8000) shiftD = 1;
        if (GetKeyState(VK_CONTROL) & 0x8000) ctrlD = 1;
		if (GetKeyState(VK_MENU) & 0x8000) altD = 1;

        // for (int i = 5; i < 255; ++i) {
        //     if (GetAsyncKeyState(i) & 0x8001) {
        //         if (i == 16 || i == 160 || i == 17 || i == 162 || i == 179 || i == 18 || i == 164) continue;
		// 		keyboard = i;
        //         //if (i >= 65 && i <= 65 + 'Z' - 'A') keyboard += 32;
        //         sleeptime(150);
        //         break;
        //     }
        // }

		keyboard = _getch();

        if (shiftD && !(GetKeyState(VK_SHIFT) & 0x8000)) {
            shiftD = 0;
            shiftU = 1;
        }

        if (ctrlD && !(GetKeyState(VK_CONTROL) & 0x8000)) {
            ctrlD = 0;
            ctrlU = 1;
        }
		
		if (altD && !(GetKeyState(VK_MENU) & 0x8000)) {
			altD = 0;
			altU = 1;
		}

        if (GetKeyState(VK_CAPITAL) & 0x0001) Caplock = 1;
		
		RECT windowRect;
		HWND hwnd = FindWindow(NULL, L"Screen of controlled deviced");

		if (hwnd == NULL) {
		    std::cout << "Window not found\n";
		} else {
		    GetWindowRect(hwnd, &windowRect);

		    T = windowRect.top;
		    L = windowRect.left;

		    W = windowRect.right - windowRect.left;
		    H = windowRect.bottom - windowRect.top;
		}

        std::shared_ptr<Packet> packet = std::make_shared<Packet>(PacketType::PT_Command);
        POINT xy;
        GetCursorPos(&xy);
		float x = xy.x;
        float y = xy.y;

		float ratio_x = (x - L)/W;
		float ratio_y = (y - T)/H;

		if(Caplock && keyboard >= 97 && keyboard <= 97 + 'z' - 'a') keyboard -= 32;
        *packet << ratio_x << ratio_y << LCD << RCD << LCU << RCU << delta << keyboard << shiftD << shiftU << ctrlD << ctrlU << altD << altU << Caplock;
        if (ctrlU == 1) ctrlU = 0;
        if (shiftU == 1) shiftU = 0;
		delta = 0;
        keyboard = -1;
		RCU = 0;
		LCU = 0;
		Caplock = 0;
        connection.pm_outgoing.Append(packet);
    }
}