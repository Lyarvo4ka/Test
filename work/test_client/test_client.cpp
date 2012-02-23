#include "stdafx.h"

#pragma comment (lib, "ws2_32.lib")

#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <winsock2.h>
#include <conio.h>

#define DEFAULT_PORT "33333"
#define DEFAULT_BUFLEN 512

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	addrinfo *result = NULL;
	addrinfo *ptr = NULL;
	addrinfo hints;
	char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Validate the parameters
	if (argc != 2) {
		printf("usage: %s server-name\n", argv[0]);
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if ( iResult != 0 ) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			_tprintf(_T("\nDone. Press any key ..."));
			_getch();
			return 1;
		}

		// Connect to server.
		iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}

	// Send an initial buffer
	iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
		return 1;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if ( iResult > 0 )
			printf("Bytes received: %d\n", iResult);
		else if ( iResult == 0 )
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());

	} while( iResult > 0 );

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	_tprintf(_T("\nDone. Press any key ..."));
	_getch();

	return 0;
}

