#include "stdafx.h"

//#pragma comment (lib, "ws2_32.lib")

//#include <windows.h>
//#include <ws2tcpip.h>
//#include <iphlpapi.h>
//#include <winsock2.h>
#include <conio.h>
//#include "Socket.h"
//#include <W32Lib.h>

#define DEFAULT_PORT_STR		"666"
#define DEFAULT_PORT			666
#define DEFAULT_BUFLEN			512

using namespace std;

void NewServerFunc(void);

int _tmain(int argc, _TCHAR* argv[])
{
	NewServerFunc();

	/*
	
	WSADATA wsaData;
	int iResult;

	// Инициализация библиотеки Winsock (WS2_32.dll) верисии 2.2
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}
	else {
		printf("Winsock intit OK\n");
	}

	addrinfo hints;
	addrinfo *result;

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if ( iResult != 0 ) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}
	else {
		printf("getaddrinfo OK\n");
	}

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	ListenSocket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}
	else {
		printf("Socket created\n");
	}

	// Setup the TCP listening socket
	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}
	else {
		printf("bind OK\n");
	}

	freeaddrinfo(result);

	if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
		printf( "Error at bind(): %ld\n", WSAGetLastError() );
		closesocket(ListenSocket);
		WSACleanup();
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}

	printf("wait for connection\n");

	sockaddr client_addr;
	int addr_len = sizeof(sockaddr);
	ZeroMemory(&client_addr, sizeof(sockaddr));

	// Accept a client socket
	ClientSocket = accept(ListenSocket, &client_addr, &addr_len);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}
	
	printf("connected\n");

	char recvbuf[DEFAULT_BUFLEN];
	int iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;
	ZeroMemory(recvbuf, sizeof(recvbuf));

	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				_tprintf(_T("\nDone. Press any key ..."));
				_getch();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else  {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			_tprintf(_T("\nDone. Press any key ..."));
			_getch();
			return 1;
		}

	} while (iResult > 0);


	// shutdown the send half of the connection since no more data will be sent
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}

	// Очистка
	closesocket(ListenSocket);
	WSACleanup();

	_tprintf(_T("\nDone. Press any key ..."));
	_getch();
	
	*/

	return 0;
}

void NewServerFunc(void)
{
	//int iResult;
	//Socket server_soc;

	//iResult = server_soc.Create(SOCK_STREAM, IPPROTO_TCP);
	//if (iResult != 0x00) 
	//	Socket::WinsockErrorHandler(_T("Socket::Create()"), iResult);
	//else
	//	_tprintf(_T("Socket::Create() - OK"));

	//iResult = server_soc.Bind(DEFAULT_PORT, _T("127.0.0.1"));
	//if (iResult != 0x00) 
	//	Socket::WinsockErrorHandler(_T("Socket::Bind()"), iResult);
	//else
	//	_tprintf(_T("Socket::Bind() - OK"));


	//
	//int x = 0;
}

