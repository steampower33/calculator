#include <iostream>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <cstdlib>

using namespace std;

#pragma comment(lib,"ws2_32")

#define BUF_SIZE 1024

void ErrorHandling(string message);

int main(int argc, char** argv) {

    if (argc != 2) {
        cout << "Usage : " << argv[0] << "<port>" << endl;
        exit(1);
    }

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    SOCKET hServSock;
    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if (hServSock == INVALID_SOCKET)
        ErrorHandling("socket() error!");

    SOCKADDR_IN servAdr;
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(atoi(argv[1]));

    if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");

    if (listen(hServSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error");

    SOCKET hClntSock;
    SOCKADDR_IN clntAdr;
    int clntAdrSize = sizeof(clntAdr);
    char message[BUF_SIZE];
    int strLen;
    for (int i = 0; i < 5; ++i) {
        hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
        if (hClntSock == INVALID_SOCKET)
            ErrorHandling("accept() error");
        else
            cout << "Connected client " << i + 1 << endl;

        while ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)
            send(hClntSock, message, strLen, 0);

        closesocket(hClntSock);
    }
    closesocket(hServSock);
    WSACleanup();
    return 0;
}

void ErrorHandling(string message) {
    cerr << message << endl;
    exit(1);
}
