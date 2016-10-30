#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include "TimerClass.h"
#include "ReaderWriter.h"
#include "CommandLineParser.h"

using namespace std;
int MyMedianOfMedians(vector<int>& numbers, int start, int end, int k);

//########## HELPER FUNCTIONS BEGIN ##########

void GenerateNumbersMersenne(vector<int>& numbers, unsigned int count)
{
	//Init Mersenne Twister with system time
	std::mt19937 twister(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	//warmup
	for (auto i = 0; i < 800000; ++i)
	{
		twister();
	}

	for (int i = 0; i < count; ++i)
	{
		numbers.push_back(twister());
	}
}

void GenerateNumbersOngoing(vector<int>& numbers, unsigned int count)
{
	//Since random_shuffle uses the twister, we have to seed it first
	std::mt19937 twister(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	//warmup
	for (auto i = 0; i < 800000; ++i)
	{
		twister();
	}

	for (int i = 0; i < count; ++i)
	{
		numbers.push_back(i);
	}

	random_shuffle(numbers.begin(), numbers.end());
}


void DebugArray(vector<int>& numbers)
{
	for (int i = 0; i < numbers.size(); ++i)
	{
		cout << numbers[i] << endl;
	}
}

bool handleParameters(int argc, char* argv[], string& inputFilename, unsigned int& numbercount)
{
	CommandLineParser cmdline(argc, argv);

	if (cmdline.cmdOptionExists("--in"))
	{
		inputFilename = cmdline.getCmdOption("--in");
	}
	else
	{
		cerr << "No inputfile, generating numbers!" << endl;

		//If no inputfile --count is needed
		if (cmdline.cmdOptionExists("--count"))
		{
			numbercount = stoi(cmdline.getCmdOption("--count"));
		}
		else
		{
			cerr << "Parameter --count not found!" << endl;
			return false;
		}
	}

	return true;
}

//########## HELPER FUNCTIONS ENDE ##########
//########## Sort BEGIN ##########

int GetActualMedian(vector<int>& numbers)
{
	sort(numbers.begin(), numbers.end());

	return numbers.at(numbers.size() / 2);
}

//########## Sort ENDE ##########
//########## Nth Element BEGIN ##########

int GetMedianByNthElement(vector<int>& numbers)
{
	nth_element(numbers.begin(), numbers.begin() + numbers.size() / 2, numbers.end());

	return numbers[numbers.size() / 2];
}

//########## Nth Element ENDE ##########
//########## Random Select BEGIN ##########

int MyRandomPartition(vector<int>& numbers, int start, int end)
{
	int pivotindex = start + rand() % (end - start + 1);
	int pivot = numbers[pivotindex];

	iter_swap(numbers.begin() + pivotindex, numbers.begin() + end);
	pivotindex = end;

	int i = start - 1;

	for (int j = start; j <= end; ++j)
	{
		if (numbers[j] <= pivot)
		{
			i = i + 1;
			iter_swap(numbers.begin() + i, numbers.begin() + j);
		}
	}

	iter_swap(numbers.begin() + i + 1, numbers.begin() + pivotindex);
	return i + 1;
}

int MyRandomSelection(vector<int>& numbers, int start, int end, int k)
{
	if (start == end)
	{
		return numbers[start];
	}

	if (k == 0)
	{
		return -1;
	}

	if (start < end)
	{
		int mid = MyRandomPartition(numbers, start, end);
		int i = mid - start + 1;
		if (i == k)
		{
			return numbers[mid];
		}
		else if (k < i)
		{
			return MyRandomSelection(numbers, start, mid - 1, k);
		}
		else
		{
			return MyRandomSelection(numbers, mid + 1, end, k - i);
		}
	}

	//Should not happen
	return -1;
}

int GetMedianByRandomSelection(vector<int>& numbers)
{
	srand(chrono::high_resolution_clock::now().time_since_epoch().count());
	int median = MyRandomSelection(numbers, 1, numbers.size() - 1, numbers.size() / 2);

	return median;
}

//########## Random Select ENDE ##########
//########## Quicksort Begin ##########

int MyQuicksortPartition(vector<int>& numbers, int low, int high)
{
	int pivot = numbers[low];
	int i = low - 1;
	int j = high + 1;
	while (true)
	{
		do
		{
			i++;
		}
		while (numbers[i] < pivot);

		do
		{
			j--;
		}
		while (numbers[j] > pivot);

		if (i >= j)
		{
			return j;
		}

		iter_swap(numbers.begin() + i, numbers.begin() + j);
	}
}

void MyQuicksort(vector<int>& numbers, int low, int high)
{
	if (low >= high)
	{
		return;
	}

	int p = MyQuicksortPartition(numbers, low, high);
	MyQuicksort(numbers, low, p);
	MyQuicksort(numbers, p + 1, high);
}


int GetMedianByQuicksort(vector<int>& numbers)
{
	MyQuicksort(numbers, 0, numbers.size() - 1);

	return numbers[numbers.size() / 2];
}

//########## Quicksort ENDE ##########
//########## Median of Medians BEGIN ##########

int MyPartitionOf5(vector<int>& numbers, int start, int end)
{
	for (int i = start; i <= end; ++i)
	{
		int next = numbers[i];

		unsigned int j;
		for (j = i; j > start && numbers[j - 1] > next; --j)
		{
			numbers[j] = numbers[j - 1];
		}
		numbers[j] = next;
	}

	return start + ((end - start) / 2);
}

int MyMedianPivot(vector<int>& numbers, int start, int end)
{
	if (end - start < 5)
	{
		return MyPartitionOf5(numbers, start, end);
	}

	for (int i = start; i <= end; i += 5)
	{
		int subEnd = i + 4;
		if (subEnd > end)
		{
			subEnd = end;
		}

		int median5 = MyPartitionOf5(numbers, i, subEnd);
		iter_swap(numbers.begin() + median5, numbers.begin() + start + floor((i - start) / 5));
	}

	return MyMedianOfMedians(numbers, start, start + ceil((end - start) / 5) - 1, start + (end - start) / 10);
}

int MyMedianPartition(vector<int>& numbers, int start, int end, int pivotindex)
{
	int pivot = numbers[pivotindex];
	iter_swap(numbers.begin() + pivotindex, numbers.begin() + end);
	int tempindex = start;
	for (int i = start; i < end; i++)
	{
		if (numbers[i] < pivot)
		{
			iter_swap(numbers.begin() + tempindex, numbers.begin() + i);
			tempindex++;
		}
	}

	iter_swap(numbers.begin() + end, numbers.begin() + tempindex);
	return tempindex;
}

int MyMedianOfMedians(vector<int>& numbers, int start, int end, int k)
{
	while (true)
	{
		if (start == end)
		{
			return start;
		}

		int pivotIndex = MyMedianPivot(numbers, start, end);
		pivotIndex = MyMedianPartition(numbers, start, end, pivotIndex);

		if (k == pivotIndex)
		{
			return k;
		}
		else if (k < pivotIndex)
		{
			end = pivotIndex - 1;
		}
		else
		{
			start = pivotIndex + 1;
		}
	}
}

int GetMedianOfMedians(vector<int>& numbers)
{
	int median = MyMedianOfMedians(numbers, 1, numbers.size() - 1, numbers.size() / 2);

	return numbers[numbers.size() / 2];
}

//########## Median of Medians ENDE ##########


/* 
 * Generating Random numbers is way faster than reading from input, also saves disk space I guess.
 * Generating of random numbers works by calling the built in mersenne twister. 
 * Seed is time_since_epoch by chrono::high_resolution_clock::now()
 * Warumup of 800000 Iterations
 * 
 * Different Algorithms are called one by one.
 * To ensure a fair measurement, the vector of numbers is copied by value before given to the algorithm.
 * Measuring time begins one function call before the algorithm starts
 * Measuring time ends after the median is returned
 * 
 * Output Format: 
 * ALGORITHM median FOUND_MEDIAN in TIMED_SECONDS seconds.
 *
 */

int main(int argc, char* argv[])
{
	//Init stuff
	unsigned int numbercount;
	vector<int> numbers, temp;
	string inputfile = "";
	TimerClass timer;
	double seconds = 0;
	int median = 0;

	//Handle parameters
	if (!handleParameters(argc, argv, inputfile, numbercount))
	{
		return -1;
	}

	//If inputfile was found
	if (!inputfile.empty())
	{
		ReaderWriter readwrite(inputfile, numbers);
	}
	//If count is bigger than 0
	else if (numbercount > 0)
	{
		numbers.reserve(numbercount);
		cout << "Generating " << numbercount << " numbers!" << endl;
		GenerateNumbersMersenne(numbers, numbercount);
		//GenerateNumbersOngoing(numbers, numbercount);
	} else
	{
		cerr << "Input wrong!" << endl;
		return false;
	}

	//Quicksort
	temp = numbers;
	cout << endl;
	cout << "Starting Quicksort" << endl;
	timer.StartTimer();
	median = GetMedianByQuicksort(temp);
	seconds = timer.GetTime();
	cout << "Quicksort median " << median << " in " << seconds << " seconds." << endl;

	temp = numbers;
	cout << endl;
	cout << "Starting NthElement" << endl;
	timer.StartTimer();
	median = GetMedianByNthElement(temp);
	seconds = timer.GetTime();
	cout << "Nth Element median " << median << " in " << seconds << " seconds." << endl;

	temp = numbers;
	cout << endl;
	cout << "Starting Random Selection" << endl;
	timer.StartTimer();
	median = GetMedianByRandomSelection(temp);
	seconds = timer.GetTime();
	cout << "Random Select median " << median << " in " << seconds << " seconds." << endl;

	temp = numbers;
	cout << endl;
	cout << "Starting MedianOfMedians" << endl;
	timer.StartTimer();
	median = GetMedianOfMedians(temp);
	seconds = timer.GetTime();
	cout << "Median of medians median " << median << " in " << seconds << " seconds." << endl;

	//DebugArray(numbers);

	temp = numbers;
	cout << endl;
	cout << "Starting sort()" << endl;
	timer.StartTimer();
	median = GetActualMedian(temp);
	seconds = timer.GetTime();
	cout << "Actual Median: " << median << " in " << seconds << " seconds." << endl;
}
