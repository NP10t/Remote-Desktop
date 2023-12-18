#pragma once
#include <stdint.h>

namespace PNet
{
	enum PacketType : uint16_t
	{
		PT_Invalid,
		PT_ChatMessage,
		PT_IntegerArray,
		PT_Test,
		PT_Image,
		PT_Keyboard,
		PT_Mouse,
		PT_Command
	};
}