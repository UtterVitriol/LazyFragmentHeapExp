#define WIN_32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <iostream>
#include <MSWSock.h>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	char ip[] = "127.0.0.1";
	char port[] = "6666";
	char path[] = "cmd /c \"C:\\Users\\uttervitriol\\source\\repos\\LazyFragmentHeapExp\\LazyFragmentationHeap.exe\"";
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		puts("wsastartup");
		return 1;
	}

	struct addrinfo hints = { 0 };
	struct addrinfo* result = NULL;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(ip, port, &hints, &result))
	{
		puts("getaddrinfo");
		return 1;
	}


	SOCKET sock = WSASocketA(result->ai_family, result->ai_socktype, result->ai_protocol, NULL, 0, 0);
	if (INVALID_SOCKET == sock)
	{
		puts("socket");
		return 1;
	}

	//SOCKET sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	//if (INVALID_SOCKET == sock)
	//{
	//	puts("socket");
	//	return 1;
	//}
	
	//SOCKET sock = INVALID_SOCKET;
	//if (SOCKET_ERROR == connect(sock, result->ai_addr, (int)result->ai_addrlen))
	//{
	//	puts("connect");
	//	return 1;
	//}

	if (SOCKET_ERROR == bind(sock, result->ai_addr, (int)result->ai_addrlen))
	{
		puts("bind");
		return GetLastError();
	}
	
	if (SOCKET_ERROR == listen(sock, 5))
	{
		puts("listen");
		return 1;
	}
	int len = 0;
	SOCKET client = accept(sock, NULL, NULL);
	if (INVALID_SOCKET == client)
	{
		puts("accept");
		return 1;
	}

	FILE;
	STARTUPINFOA sinfo = { 0 };
	sinfo.cb = sizeof(sinfo);
	sinfo.dwFlags = (STARTF_USESTDHANDLES);
	sinfo.hStdInput = (HANDLE)client;
	sinfo.hStdOutput = (HANDLE)client;
	sinfo.hStdError = (HANDLE)client;
	PROCESS_INFORMATION pinfo = { 0 };
	if (!CreateProcessA(NULL, path, NULL, NULL, TRUE, NULL, NULL, "C:\\Users\\uttervitriol\\source\\repos\\LazyFragmentHeapExp\\", &sinfo, &pinfo))
	{
		puts("createprocessa");
	}

	printf("created: %d\n", GetLastError());

	//getchar();
	DWORD exit = 0;
	WaitForSingleObject(pinfo.hProcess, INFINITE);
	GetExitCodeProcess(pinfo.hProcess, &exit);
	int err = TerminateProcess(pinfo.hProcess, 0);
	closesocket(client);
	closesocket(sock);

	WSACleanup();

	return err;
}