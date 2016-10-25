#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;


void GenerateNumbers(vector<unsigned int>& numbers, int count){

	//Init Mersenne Twister with system time
	std::mt19937 twister(std::chrono::system_clock::now().time_since_epoch().count());

	//warmup
	for (auto i = 0; i< 800000; ++i)
	{
		twister();
	}

	for (int i = 0; i < count; ++i)
	{
		numbers.push_back(twister());
	}

}

void DebugArray(vector<unsigned int>& numbers)
{
	for (int i = 0; i < numbers.size(); ++i)
	{
		cout << numbers[i] << endl;
	}
}

int UseNthElement(vector<unsigned int>* numbers, int count)
{
	
	return 0;
}

void main()
{
	int numbercount = 10;
	vector<unsigned int> numbers;
	numbers.reserve(numbercount);

	GenerateNumbers(numbers, numbercount);

	DebugArray(numbers);


}
