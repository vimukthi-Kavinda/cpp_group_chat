#include <iostream>
#include <ws2tcpip.h>
#include <winsock.h>
#include <vector>
#include <atomic>
#include<thread>
using namespace std;
vector<SOCKET> allSkts;

atomic_int msgCount = 0;

void sendMessages(SOCKET *clientSokt,string incoming){
	for (int i = 0; i < allSkts.size(); i++)
	        {
	            SOCKET temp = allSkts[i];
	            if (temp == *clientSokt)
	            {
	                continue;
	            }
	            string d = "message is: " + incoming;
	            int sntRes = send(temp, d.c_str(), d.size() + 1, 0);
	            if (sntRes == INVALID_SOCKET)
	            {
	                closesocket(temp);
	                continue;
	            }
	        }
}




void *msgHndlr(void *s)
{

    SOCKET *clientSokt = (SOCKET *)s;
    char buff[4096];

    while (true)
    {
        if (*clientSokt == INVALID_SOCKET)
        {
            cout << "socket error" << endl;
            closesocket(*clientSokt);
            exit(0);
        }
        ZeroMemory(buff, 4096);
        int resByt = recv(*clientSokt, buff, 4096, 0);
        if (resByt <= 0)
        {
            closesocket(*clientSokt);
            // remove socket from vector
            break;
        }
        string incoming = string(buff, 0, resByt);
        msgCount++;
        cout << "incoming mesage : " << incoming << endl;
        cout << " message count : " << msgCount << endl;

        sendMessages(clientSokt,incoming);

    }
    return nullptr;
}

int main()
{
    SOCKET sevrSocket;
    // init winsock
    try
    {
        WSADATA wsadata;
        int wnsokRes = WSAStartup(MAKEWORD(2, 2), &wsadata);
        if (wnsokRes != 0)
        {
            cout << "cannot initialize winsock" << endl;
            exit(0);
        }

        sevrSocket = socket(AF_INET, SOCK_STREAM, 0);
        cout << "server socket : " << sevrSocket << endl;
        if (sevrSocket == INVALID_SOCKET)
        {
            cout << "error creating socket";
            exit(0);
        }

        sockaddr_in hnt;
        hnt.sin_family = AF_INET;
        hnt.sin_port = htons(5000);
        hnt.sin_addr.S_un.S_addr = INADDR_ANY;

        bind(sevrSocket, (sockaddr *)&hnt, sizeof(hnt));

        // socket listening to connect a client

        // waiting to accept
        sockaddr_in client;
        int cliSize = sizeof(client);
        int lisetnOut = listen(sevrSocket, SOMAXCONN);
        if (lisetnOut == 0)
        {
            cout << "Server is listening to incoming client request" << endl;
        }
        else
        {
            cout << "error occurred" << endl;
            exit(0);
        }

        while (true)
        {

            SOCKET clientSokt = accept(sevrSocket, (sockaddr *)&client, &cliSize);
            if (clientSokt == -1)
            {
                break;
            }
            cout << "client connected" << endl;
            // msgHndlr(clientSokt);
           // pthread_t t1;

            SOCKET *k = (SOCKET *)malloc(sizeof(SOCKET));
            if(k==nullptr){
                throw "malloc error";
            }
            *k = clientSokt;

            allSkts.push_back(clientSokt);
            cout << "client joined : " << allSkts.size() << endl;
           // pthread_create(&t1, nullptr, msgHndlr, k);
            thread t1(msgHndlr,k);
        }
    }

    catch (...)
    { closesocket(sevrSocket);
    WSACleanup();
        cout<<"error captured"<<endl;
    }

    return 0;
}
