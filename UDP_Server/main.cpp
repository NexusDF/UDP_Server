#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main(int argc, char* argv[])
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		cout << "Can't start Winsock! " << wsOk;
		return;
	}

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	char broadcast = '1';
	int broadOk = setsockopt(out, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	if (broadOk != 0)
	{
		cout << "Error in setting Broadcast option";
		closesocket(out);
		return;
	}

	sockaddr_in receiver, sender;

	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(54000);
	receiver.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(out, (sockaddr*)&receiver, sizeof(receiver)) < 0)
	{
		cout << "Error in BINDING" << WSAGetLastError();
		closesocket(out);
		return;
	}

	char buf[1024];
	int senderLength = sizeof(sender);
	ZeroMemory(buf, 1024);
	ZeroMemory(&sender, senderLength);

	recvfrom(out, buf, 1024, 0, (sockaddr*)&sender, &senderLength);
	sendto(out, buf, 1024, 0, (sockaddr*)&sender, senderLength);

	char serverIp[256];
	inet_ntop(AF_INET, &sender.sin_addr, serverIp, 256);
	cout << buf << " from " << serverIp << endl;

	// Close the socket
	closesocket(out);

	// Close down Winsock
	WSACleanup();
}