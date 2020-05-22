#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <shared_mutex>
#include <string>
#include <vector>
#include <chrono>

#define readersAm 2
#define writersAm 2
#define progRepeat 5
#define rwDelay 100

//Да, здесь нет системных вызовов. Да, я пользовался ими до этого (https://github.com/rururu387/os_lab_2019/tree/master/lab4). Здесь их нет потому, что они не потребовались.

class SharedDataMT
{
	int data;
	std::shared_mutex dataMutex;

public:
	SharedDataMT(int value);

	static void myRead(SharedDataMT* sharedData);
	static void myWrite(SharedDataMT* sharedData, const int value);
};

SharedDataMT::SharedDataMT(int value)
{
	data = value;
}

void SharedDataMT::myRead(SharedDataMT* sharedData)
{
	std::shared_lock<std::shared_mutex> r(sharedData->dataMutex);
	std::cout << "Read: " << sharedData->data << '\n';
	std::cout.flush();
	r.unlock();
}

void SharedDataMT::myWrite(SharedDataMT* sharedData, const int value)
{
	std::unique_lock<std::shared_mutex> w(sharedData->dataMutex);
	std::cout << "Write start: " << sharedData->data << " to \t" << value << '\n';
	std::cout.flush();
	std::this_thread::sleep_for(std::chrono::milliseconds(rwDelay));
	sharedData->data = value;
	std::cout << "Write end: " << sharedData->data << '\n';
	std::cout.flush();
	w.unlock();
}

int main()
{
	srand(time(NULL));
	std::vector<std::thread> rThreadsVector;
	std::vector<std::thread> wThreadsVector;

	SharedDataMT data(rand());
	SharedDataMT::myRead(&data);
	
	for (int curRepeat = 0; curRepeat < progRepeat; curRepeat++)
	{
		for (int i = 0; i < readersAm; i++)
		{
			rThreadsVector.push_back(std::thread(&SharedDataMT::myRead, &data));
		}

		for (int i = 0; i < writersAm; i++)
		{
			wThreadsVector.push_back(std::thread(&SharedDataMT::myWrite, &data, rand()));
		}
	}

	for (int i = 0; i < (readersAm + writersAm) * progRepeat; i++)
	{
		if (!rThreadsVector.empty())
		{
			rThreadsVector.back().join();
			rThreadsVector.pop_back();
		}
		else
		{
			wThreadsVector.back().join();
			wThreadsVector.pop_back();
		}
	}

	return 0;
}