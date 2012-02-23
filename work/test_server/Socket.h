#ifndef _SOCKET
#define _SOCKET

#pragma comment (lib, "ws2_32.lib")

#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <winsock2.h>

class Socket
{
private:
	static WSADATA wsa_data;
	static BOOL winsock_initialized;

	SOCKET hSocket;
	int adress_family;
	int soc_type;
	int soc_protocol;

	// Инициализация библиотеки Winsock DLL.
	// Если произошла ошибка - возвращает не нулевое значение.
	int InitWinsock();

	void Cleanup();

public:
	Socket(void);
	~Socket(void);
	
	// Создаёт сокет.
	// Если всё ок - возвращает 0x00, иначе - код ошибки.
	int Create(int type = SOCK_STREAM, int protocol = IPPROTO_TCP);

	// Привязывает локальный адрес к сокету.
	// Если всё ок - возвращает 0x00, иначе - код ошибки.
	int Bind(const sockaddr* saddr, int saddr_len);
	int Bind(short port, TCHAR *in_addr);




	static void WinsockErrorHandler(const TCHAR *err_str, int err_code);
};

#endif // _SOCKET
