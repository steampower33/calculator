#include <iostream>
#include <cstdio>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <cstdlib>

using namespace std;

#pragma comment(lib,"ws2_32")

#define BUF_SIZE 1024

void ErrorHandling(string message);

int main(int argc, char** argv) {
    
    if (argc != 3) {
        cout << "Usage : " << argv[0] << "<IP> <port>" << endl;
        exit(1);
    }

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    SOCKET hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error");

    SOCKADDR_IN servAdr;
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    if (inet_pton(AF_INET, argv[1], &servAdr.sin_addr) != 1)
        ErrorHandling("inet_pton() error");
    servAdr.sin_port = htons(atoi(argv[2]));

    if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("connet() error");
    else
        cout << "Connected.........." << endl;

    char message[BUF_SIZE];
    while (1) {
        string line;
        cout << "Input message(Q to quit): ";
        getline(cin, line);

        strcpy_s(message, line.c_str());

        if (!strcmp(message, "q") || !strcmp(message, "Q"))
            break;

        int strLen = send(hSocket, message, strlen(message), 0);
        int recvLen = 0;
        while (recvLen < strLen) {
            int recvCnt = recv(hSocket, message, BUF_SIZE - 1, 0);
            if (recvCnt == -1)
                ErrorHandling("read() error!");
            recvLen += recvCnt;
        }
        message[strLen] = NULL;
        cout << message << endl;
    }
    closesocket(hSocket);
    WSACleanup();
    return 0;
}

void ErrorHandling(string message) {
    cerr << message << endl;
    exit(1);
}
