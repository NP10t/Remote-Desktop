#include "MyServer.h"
#include <opencv2/opencv.hpp>
#include <chrono>
#include <conio.h>
#include <iostream>
#include <Windows.h>

float shitfD = 0, ctrlD = 0;

void MyServer::Run()
{
	// if (Network::Initialize())
	{
		if (this->Initialize())
		{
			// while (true)
			// {
				this->Frame();
			// }
		}
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
	case PacketType::PT_Command:
	{
		// return true;
		float W_ratio = GetSystemMetrics(SM_CXSCREEN);
		float H_ratio = GetSystemMetrics(SM_CYSCREEN);
		float a[13];
		a[0] = 0, a[1] = 0, a[2] = 0, a[3] = 0, a[4] = 0, a[5] = 0, a[6] = 0, a[7] = -1, a[8] = 0, a[9] = 0, a[10] = 0, a[11] = 0;
		*packet >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5] >> a[6] >> a[7] >> a[8] >> a[9] >> a[10] >> a[11] >> a[12];

		// std::wstring strSelectedDevice2 = std::to_wstring(a[0]);
		// strSelectedDevice2 += std::to_wstring(H_ratio);
		// LPCTSTR lpSelectedDevice2 = strSelectedDevice2.c_str();
		// MessageBox(NULL, lpSelectedDevice2, TEXT("size of use_fd (client, 1)"), MB_ICONERROR | MB_OK);
		if(a[0] >= 0 && a[0] <= 1 && a[1] >= 0 && a[1] <= 1)
		{
			SetCursorPos(round(a[0]*W_ratio),round(a[1]*H_ratio-20));

			if(a[2] != 0) mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);

			if(a[3] != 0) mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);

			if(a[4] != 0) mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

			if(a[5] != 0) mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);

			if(a[6] == 1 || a[6] == -1) 
			{
				mouse_event(MOUSEEVENTF_WHEEL,0,0, a[6]*120,0);
			}
			if(a[8]) shitfD = 1;
			if(a[10]) ctrlD = 1;
			if(a[8] == 1 && a[10] == 1 && a[7] != -1)
			{
				keybd_event(VK_CONTROL, 0, 0, 0);
				keybd_event(VK_SHIFT, 0, 0, 0);
				keybd_event(a[7],0,0,0);
				keybd_event(a[7],0,KEYEVENTF_KEYUP,0);
				keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
			}
			else if(a[8] != 0 && a[7] != -1)
			{
				keybd_event(VK_SHIFT, 0, 0, 0);
				// std::cout << "SHIFT IS DOWN\n";
				// char output = a[7];
				// if(output >= 'a' && output <='z' && a[12] == 0)
				// {
				// 	a[7] -= 32;
				// 	output -= 32;
				// }
				keybd_event(a[7],0,0,0);
				keybd_event(a[7],0,KEYEVENTF_KEYUP,0);
				// std::cout <<"KEY PRESS: " << output <<'\n';
				keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);

			}
			// else if(shitfD && !(GetKeyState(VK_SHIFT) & 0x8000))
			// {
			// 	shitfD = 0;
			// 	// std::cout << "SHIFT IS UP\n";
			// 	keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);
			// }
			else if(a[10] != 0 && a[7] != -1)
			{
				keybd_event(VK_CONTROL, 0, 0, 0);
				// std::cout <<"CRTL IS PRESS\n";
				// char output = a[7];
				// std::cout <<"KEY PRESS: " << output <<'\n';
				keybd_event(a[7],0,0,0);
				keybd_event(a[7],0,KEYEVENTF_KEYUP,0);
				keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
			}
			// if(ctrlD != 0 && !(GetKeyState(VK_CONTROL) & 0x8000))
			// {
			// 	ctrlD = 0;
			// 	//std::cout <<"CTRL IS UP\n";
			// 	keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
			// }
			if(a[7] != -1 && a[8] == 0 && a[10] == 0)
			{
				INPUT input;
				memset(&input, 0, sizeof(INPUT));
				input.type = INPUT_KEYBOARD;
				input.ki.wVk = a[7];
				input.ki.dwFlags = 0; // 0 for key press
				SendInput(1, &input, sizeof(INPUT));
				input.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &input, sizeof(INPUT));
			}
		}
		break;
	}

	// case PacketType::PT_IntegerArray:
	// {
	// 	uint32_t arraySize = 0;
	// 	*packet >> arraySize;
	// 	std::cout << "Array Size: " << arraySize << std::endl;
	// 	for (uint32_t i = 0; i < arraySize; i++)
	// 	{
	// 		uint32_t element = 0;
	// 		*packet >> element;
	// 		std::cout << "Element[" << i << "] - " << element << std::endl;
	// 	}
	// 	break;
	// }
	case PacketType::PT_ChatMessage:
	{
		std::string chatmessage;
		*packet >> chatmessage;
		// std::cout << "Client says: " << chatmessage << std::endl;
		break;
	}

	default:
		// std::cout << "Unrecognized packet type: " << packet->GetPacketType() << std::endl;
		return true;
	}

	return true;
}