#include <iostream>
#include <ws2tcpip.h>
#include <string>
#include <winsock.h>
#include<thread>

using namespace std;
SOCKET sockt;

void *readProcess(void *x)
{

    string inp;

    cout << "input sth : " << endl;
    getline(cin, inp);
    while (true)
    {
        if (inp == "exit"||sockt==INVALID_SOCKET )
        {
            cout << "exit conversation" << endl;
            cout<<"socket error input thread" <<endl;
            closesocket(sockt);
            WSACleanup();
            exit(0);
        }
        int sntRes = send(sockt, inp.c_str(), inp.size() + 1, 0);
        if(sntRes==-1){
            cout<<"socket error input thread" <<endl;
            closesocket(sockt);
            WSACleanup();
            exit(0);
        }
        cout << "input sth : " << endl;
        getline(cin, inp);
    }
    // int sntRes = send(sockt, inp.c_str(), inp.size() + 1, 0);

    return nullptr;
}

void *writeProcess(void *y)
{
    char getBuf[4096];
    while (true)
    {if(sockt==-1){
        cout<<"socket error in write thread"<<endl;
        closesocket(sockt);
        exit(0);
    }
        ZeroMemory(getBuf, 4096);
        int resByt = recv(sockt, getBuf, 4096, 0);
        if(resByt<=0){
cout<<"socket error in recieving thread" <<endl;
            closesocket(sockt);
            WSACleanup();
            exit(0);
        }else
        cout << "recieved : " << string(getBuf, 0, resByt) << endl;
    }

    return nullptr;
}

int main()
{
    try{
    string ipaddr = "127.0.0.1";

    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);

    sockt = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in hnt; // winsock which port to connect
    hnt.sin_family = AF_INET;
    hnt.sin_port = htons(5000);
    inet_pton(AF_INET, ipaddr.c_str(), &hnt.sin_addr); // hnt.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");

    // connect to server
    int res = connect(sockt, (sockaddr *)&hnt, sizeof(hnt));
    if (res == -1)
    {
        cout << "error occurred while connecting" << endl;
        closesocket(sockt);
        WSACleanup();
       exit(0);
    }

   /* pthread_t readThread;
    pthread_t writeThread;

    pthread_create(&readThread, nullptr, readProcess, nullptr);
    pthread_create(&writeThread, nullptr, writeProcess, nullptr);
    pthread_join(readThread, nullptr);
    pthread_join(writeThread, nullptr);*/

    thread readThread(readProcess,nullptr);
    thread writeThread(writeProcess,nullptr);

    readThread.join();
    writeThread.join();




    }

    catch(...){
cerr<<"error captured"<<endl;
    }
    closesocket(sockt);
        WSACleanup();
    return 0;
}
