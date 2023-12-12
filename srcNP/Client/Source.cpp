//Client Code [Tutorial 9] [Nonblocking] [Winsock]
//Author: Jacob Preston 2019-06-25

#include "MyClient.h"
#include <iostream>

int main()
{
	if (Network::Initialize())
	{
		MyClient client;
		if (client.Connect(IPEndpoint("::1", 6112))) //ket noi den server co dia chi ip... cong... Lam nut bat su kien thi goi ham nay de ket noi, Ket noi moi se duoc luu lai o buffer bla bla NP lam het r
		{
			client.Frame(); //(gui den nguoi lam nut bat su kien): ham nay chi gan goi 1 lan, khi ket noi den server lan dau tien thi goi ham nay, tu do ve sau ko can nua
			client.control.join(); // ham Frame se tao ra 2 thread control va video, nen can co control.join va video.join
			client.video.join(); // ca 3 dong nay di voi nhau, chi can goi 1 lan, ko phai cu co ket noi moi la goi them 1 lan, NO. goi 1 lan thoi
		}
	}
	Network::Shutdown();
	system("pause");
	return 0;
}