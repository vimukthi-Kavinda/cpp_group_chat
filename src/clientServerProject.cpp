//============================================================================
// Name        : clientServerProject.cpp
// Author      : vk
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include<pthread.h>
#include <thread>
#include<mutex>

std::mutex coutMutex; // Mutex for synchronizing access to std::cout

typedef struct
{
int x;	int y;

}argus;






void threadFunction(int threadId) {
	while(true){
		std::lock_guard<std::mutex> lock(coutMutex); // Lock the mutex

    std::cout << "Thread " << threadId << " is running." << std::endl;
}

}


int main() {

	return 0;
}
