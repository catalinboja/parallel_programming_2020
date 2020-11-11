#pragma once

#include <omp.h>
#include <stdio.h>
#include <string>
#include <thread>
using namespace std;

#define nimic ;

void hello(int id, std::string mesaj) {
	for (int i = 0; i < 5; i++) {
		printf("\n %d - %s", id, mesaj.c_str());
	}
}

int main() {


	thread t1(hello, 1, "Hello");
	thread t2(hello, 2, "Hello");

	printf("\n Hello din main");

	t1.join();
	t2.join();

	printf("\n ------------------------");
	
#pragma omp parallel 
	{
		hello(0, "Hello");
	}
}