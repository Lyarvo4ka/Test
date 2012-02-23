#include "StdAfx.h"
#include "Socket.h"

WSADATA Socket::wsa_data;
BOOL Socket::winsock_initialized = FALSE;

Socket::Socket(void) : hSocket(INVALID_SOCKET),
	adress_family(AF_INET)
{

}

Socket::~Socket(void)
{
	Cleanup();
}

void Socket::Cleanup()
{
	if (winsock_initialized)
		WSACleanup();
	if (hSocket != INVALID_SOCKET)
		closesocket(hSocket);
}

int Socket::InitWinsock()
{
	int res = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (res == 0x00)
	{
		winsock_initialized = TRUE;
	}
	return res;
}

int Socket::Create(int type, int protocol)
{
	// Инициализация библиотеки Winsock
	if (!winsock_initialized)
	{
		int res = InitWinsock();
		if (res != 0x00)
			return res;
	}

	hSocket = ::socket(AF_INET, type, protocol);
	if (hSocket == INVALID_SOCKET)
		return ::WSAGetLastError();
	else
	{
		soc_type = type;
		soc_protocol = protocol;
		return 0x00;
	}

}

int Socket::Bind(const sockaddr* saddr, int saddr_len)
{
	int res = ::bind(hSocket, saddr, saddr_len);
	if (res == 0x00)
		return 0x00;
	else
		return ::WSAGetLastError();
}

int Socket::Bind(short port, TCHAR *in_addr)
{
	sockaddr_in addr_in;
	ZeroMemory(&addr_in, sizeof(addr_in));
	addr_in.sin_family = adress_family;
	addr_in.sin_addr.s_addr = inet_addr(in_addr);
	addr_in.sin_port = htons((u_short)port);

	int res = ::bind(hSocket, (sockaddr *)&addr_in, sizeof(addr_in));
	if (res == 0x00)
		return 0x00;
	else
		return ::WSAGetLastError();
}

void Socket::WinsockErrorHandler(const TCHAR *err_str, int err_code)
{
	_tprintf(_T("%s. Error code: %d"), err_str, err_code);
}
