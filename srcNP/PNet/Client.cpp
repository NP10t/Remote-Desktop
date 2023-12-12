#include "Client.h"
#include <iostream>

namespace PNet
{
	bool Client::Connect(IPEndpoint ip)
	{
		Socket socket = Socket(ip.GetIPVersion());
		if (socket.Create() == PResult::P_Success)
		{
			if (socket.SetBlocking(true) != PResult::P_Success)
				return false;

			std::cout << "Socket successfully created." << std::endl;
			// if (socket.Connect(ip) != PResult::P_Success)
			while(1)
			{
				if(socket.Connect(ip) != PResult::P_Success){
					continue; //neu client mo truoc server thi client wait until server start to listen
				}

				if (socket.SetBlocking(false) == PResult::P_Success)
				{
					TCPConnection &newConnection = TCPConnection(socket, ip);
					connections.emplace_back(newConnection); // khi co ket noi toi thi bo vao buffer
					WSAPOLLFD newConnectionFD = {};
					newConnectionFD.fd = newConnection.socket.GetHandle();
					newConnectionFD.events = POLLRDNORM | POLLWRNORM;
					newConnectionFD.revents = 0;

					master_fd.push_back(newConnectionFD); //khi co ket noi toi thi bo vao buffer
					selected_device = 0;

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

	void Client::ControlUsingTCP()
	{
		while (true)
		{
			int i = selected_device;
			if (i == -1)
				continue;
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

	void Client::PlayVideo()
	{
		while (true)
		{
			int i = selected_device;
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
								std::shared_ptr<Packet> frontPacket = connections[i].pm_incoming.Retrieve();
								if (!ProcessPacket(frontPacket))
								{
									CloseConnection(i, "Failed to process incoming packet.");
									break;
								}
								int key = waitKey(1);
								if (key == 'x')
									return;
								connections[i].pm_incoming.Pop();
								// }
							}
						}
					}
					// }
				}
			}
		}
	}

	bool Client::Frame()
	{
		std::thread control(&Client::ControlUsingTCP, this);
		this->control = std::move(control);

		std::thread video(&Client::PlayVideo, this);
		this->video = std::move(video);

		return true;
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
		TCPConnection &connection = connections[connectionIndex];
		OnDisconnect(connection, reason);
		master_fd.erase(master_fd.begin() + (connectionIndex));
		use_fd.erase(use_fd.begin() + (connectionIndex));
		connection.Close();
		connections.erase(connections.begin() + connectionIndex);
	}

	void Client::Mouse(TCPConnection &connection)
	{
		std::cout << "dang truyen chuot\n";
		std::string a = "chuot ne em\n";
		std::shared_ptr<Packet> packet = std::make_shared<Packet>(PacketType::PT_ChatMessage);
		*packet << a;
		connection.pm_outgoing.Append(packet);
	}

	void Client::Keyboard(TCPConnection &connection)
	{
		std::cout << "dang truyen ban phim\n";
		std::string a = "ban phim ne em\n";
		std::shared_ptr<Packet> packet = std::make_shared<Packet>(PacketType::PT_ChatMessage);
		*packet << a;
		connection.pm_outgoing.Append(packet);
	}
}