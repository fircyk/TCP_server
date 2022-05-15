#include <iostream>
#include <ws2tcpip.h>
#include <winsock2.h>



using namespace std;

int main()
{

    WSADATA wsData;
    sockaddr_in serv_address;
    sockaddr_in cli_address;
    serv_address.sin_family = AF_INET;
    int serv_address_size = sizeof(serv_address);
    int cli_address_size = sizeof(cli_address);

    WORD ver = MAKEWORD(2,2);

    int wsStart = WSAStartup(ver, &wsData);

    if(wsStart != 0){
        cout << "winsock initialisation failed" << endl;
    }

    SOCKET servSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(servSocket == INVALID_SOCKET){
        cout << "creating socket failed" << endl;
        return 0;
    }

    serv_address.sin_port = htons(54000);
    serv_address.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(servSocket, (sockaddr*)&serv_address, serv_address_size);

    listen(servSocket, SOMAXCONN);

    SOCKET clientSocket = accept(servSocket, (sockaddr*)&cli_address, &cli_address_size);

    char host[NI_MAXHOST];
    char service[NI_MAXHOST];

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXHOST);

    if(getnameinfo((sockaddr*)&cli_address, cli_address_size, host, NI_MAXHOST, service, NI_MAXSERV, 0)==0){
        cout << host << " connected, port: " << service << endl;
    }else{
       // inet_ntop(AF_INET, &cli_address.sin_addr, host, NI_MAXHOST);
        string dupa = inet_ntoa(cli_address.sin_addr);
       // inet_nt
        cout << host << " connected, port: " << ntohs(cli_address.sin_port) << endl;
    }

    closesocket(servSocket);
    char buffer[2048];

    while(true){
        ZeroMemory(buffer, 2048);
        int bytes_received = recv(clientSocket, buffer, 2048, 0);
        if(bytes_received == SOCKET_ERROR){
            cout << "error recv" << endl;
            break;
        }
        if(bytes_received == 0){
            cout << "client disconnected" << endl;
            break;
        }

        send(clientSocket, buffer, bytes_received+1, 0);

    }

    closesocket(clientSocket);

    WSACleanup();


    return 0;
}
