#include "MyServer.h"
#include <opencv2/opencv.hpp>
#include <chrono>
#include <conio.h>
#include <iostream>
#include <Windows.h>
int shitfD = 0, ctrlD = 0;

void MyServer::Run()
{
	if (this->Initialize())
	{
		this->Frame();
	}
}

void MyServer::OnConnect()
{
	isConnected = 1;
	// std::cout << "Successfully connected to the Client!" << std::endl;

	// std::shared_ptr<Packet> helloPacket = std::make_shared<Packet>(PacketType::PT_ChatMessage);
	// *helloPacket << std::string("Hello from the Server!");
	// connection.pm_outgoing.Append(helloPacket);
}

bool MyServer::ProcessPacket(std::shared_ptr<Packet> packet)
{
	switch (packet->GetPacketType())
	{
	case PacketType::PT_Mouse:
	{
		uint32_t *a = new uint32_t[7];
		a[0] = 0, a[1] = 0, a[2] = 0, a[3] = 0, a[4] = 0, a[5] = 0, a[6] = 0;
		*packet >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5] >> a[6];
		// std::cout << "X: " << a[0] <<" " << a[1] <<'\n';
		SetCursorPos(a[0], a[1]);
		if (a[2] != 0)
			//std::cout << "LEFT IS CLICK\n"; 
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);

		if (a[3] != 0)
			//std::cout << "RIGHT IS CLICK\n"; 
			mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);

		if (a[4] != 0)
			//std::cout << "LEFT CLICK UP\n"; 
			mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

		if (a[5] != 0)
			// std::cout << "RIGHT CLICK UP\n"; 
			mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);

		if (a[6] != 0)
		{
			// std::cout << "SRCOLLINGGGG ";
			// if (a[6] > 0)
			// 	std::cout << "UPPPPP " << a[6] << '\n';
			// else
			// 	std::cout << "DOWNNNNNN\n";
			mouse_event(MOUSEEVENTF_WHEEL,0,0, a[6]*120,0);
		}
		break;
	}
	case PacketType::PT_Keyboard:
	{
		uint32_t *a = new uint32_t[6];
		a[0] = 0, a[1] = 0, a[2] = 0, a[3] = 0, a[4] = 0, a[5] = 0;
		*packet >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5];
		if (a[1])
			shitfD = 1;
		if (a[3])
			ctrlD = 1;
		if (a[1] == 1 && a[3] == 1 && a[0] != -1)
		{
			// std::cout << "shift and ctrl is down\n";
			keybd_event(VK_CONTROL, 0, 0, 0);
			keybd_event(VK_SHIFT, 0, 0, 0);
			keybd_event(a[0],0,0,0);
			keybd_event(a[0],0,KEYEVENTF_KEYUP,0);
		}
		else if (shitfD != 0 && a[0] != -1)
		{
			keybd_event(VK_SHIFT, 0, 0, 0);
			// std::cout << "SHIFT IS DOWN\n";
			// char output = a[0];
			// if (output >= 'a' && output <= 'z' && a[5] == 0)
			// {
			// 	a[0] -= 32;
			// 	output -= 32;
			// }
			keybd_event(a[0],0,0,0);
			keybd_event(a[0],0,KEYEVENTF_KEYUP,0);
			//std::cout << "KEY PRESS: " << output << '\n';
		}
		else if (shitfD && !(GetKeyState(VK_SHIFT) & 0x8000))
		{
			shitfD = 0;
			// std::cout << "SHIFT IS UP\n";
			keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);
		}
		else if (ctrlD != 0 && a[0] != -1)
		{
			keybd_event(VK_CONTROL, 0, 0, 0);
			// std::cout << "CRTL IS PRESS\n";
			// char output = a[0];
			// std::cout << "KEY PRESS: " << output << '\n';
			keybd_event(a[0],0,0,0);
			keybd_event(a[0],0,KEYEVENTF_KEYUP,0);
		}
		if (ctrlD != 0 && !(GetKeyState(VK_CONTROL) & 0x8000))
		{
			ctrlD = 0;
			//std::cout << "CTRL IS UP\n";
			keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
		}
		if (a[0] != -1 && a[1] == 0 && a[3] == 0)
		{
			keybd_event(a[0],0,0,0);
			keybd_event(a[0],0,KEYEVENTF_KEYUP,0);
			// char output = a[0];
			// std::cout << "KEY PRESS: " << output << '\n';
		}
		break;
	}
	case PacketType::PT_ChatMessage:
	{
		std::string chatmessage;
		*packet >> chatmessage;
		// std::cout << "Client says: " << chatmessage << std::endl;
		break;
	}
	case PacketType::PT_IntegerArray:
	{
		uint32_t arraySize = 0;
		*packet >> arraySize;
		std::cout << "Array Size: " << arraySize << std::endl;
		for (uint32_t i = 0; i < arraySize; i++)
		{
			uint32_t element = 0;
			*packet >> element;
			std::cout << "Element[" << i << "] - " << element << std::endl;
		}
		break;
	}
	default:
		std::cout << "Unrecognized packet type: " << packet->GetPacketType() << std::endl;
		return false;
	}

	return true;
}