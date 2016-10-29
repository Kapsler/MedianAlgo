#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include "TimerClass.h"

using namespace std;
int MyMedianOfMedians(vector<int>& numbers, int start, int end, int k);

void GenerateNumbersMersenne(vector<int>& numbers,unsigned int count){

	//Init Mersenne Twister with system time
	std::mt19937 twister(std::chrono::high_resolution_clock::now().time_since_epoch().count());

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

void GenerateNumbersOngoing(vector<int>& numbers, unsigned int count){

	//Since random_shuffle uses the twister, we have to seed it first
	std::mt19937 twister(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	//warmup
	for (auto i = 0; i< 800000; ++i)
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

int GetActualMedian(vector<int>& numbers, int count)
{
	sort(numbers.begin(), numbers.end());

	return numbers.at(count / 2);

}

int GetMedianByNthElement(vector<int>& numbers)
{
	nth_element(numbers.begin(), numbers.begin() + numbers.size() / 2, numbers.end());

	return numbers[numbers.size() / 2];
}

int MyRandomPartition(vector<int>& numbers, int start, int end)
{
	int pivotindex = start + rand() % (end - start + 1);
	int pivot = numbers[pivotindex];

	iter_swap(numbers.begin() + pivotindex, numbers.begin() + end);
	pivotindex = end;

	int i = start - 1;

	for(int j = start; j <= end; ++j)
	{
		if(numbers[j] <= pivot)
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
	if(start == end)
	{
		return numbers[start];
	}

	if(k == 0)
	{
		return -1;
	}

	if(start < end)
	{
		int mid = MyRandomPartition(numbers, start, end);
		int i = mid - start + 1;
		if(i==k)
		{
			return numbers[mid];
		} else if(k < i)
		{
			return MyRandomSelection(numbers, start, mid - 1, k);
		} else
		{
			return MyRandomSelection(numbers, mid + 1, end, k - i);
		}
	}

	//Should not happen
	return INT_MAX;

}

int GetMedianByRandomSelection(vector<int>& numbers)
{
	srand(chrono::high_resolution_clock::now().time_since_epoch().count());
	int median = MyRandomSelection(numbers, 1, numbers.size()-1, numbers.size()/2);

	return median;
}

int MyPartition(vector<int>& numbers, int low, int high)
{
	int pivot = numbers[low];
	int i = low - 1;
	int j = high + 1;
	while(true)
	{
		do
		{
			i++;
		} while (numbers[i] < pivot);

		do
		{
			j--;
		} while (numbers[j] > pivot);

		if(i >= j)
		{
			return j;
		}

		iter_swap(numbers.begin() + i, numbers.begin() + j);
	}

}

void MyQuicksort(vector<int>& numbers, int low, int high)
{
	if(low >= high)
	{
		return;
	}

	int p = MyPartition(numbers, low, high);
	MyQuicksort(numbers, low, p);
	MyQuicksort(numbers, p + 1, high);
}


int GetMedianByQuicksort(vector<int>& numbers)
{
	MyQuicksort(numbers, 0, numbers.size() - 1);

	return numbers[numbers.size() / 2];
}

int MyPartitionOf5(vector<int>& numbers, int start, int end)
{
	
	for(int i = start; i <= end; ++i)
	{
		int next = numbers[i];

		unsigned int j;
		for(j = i; j > start && numbers[j - 1] > next; --j)
		{
			numbers[j] = numbers[j - 1];
		}
		numbers[j] = next;
	}

	return start + ((end - start) / 2);
}

int MyMedianPivot(vector<int>& numbers, int start, int end)
{
	if(end - start < 5)
	{
		return MyPartitionOf5(numbers, start, end);
	}

	for(int i = start; i <= end; i+=5)
	{
		int subEnd = i + 4;
		if(subEnd > end)
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
	for(int i = start; i < end; i++)
	{
		if(numbers[i] < pivot)
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
	while(true)
	{
		if(start == end)
		{
			return start;
		}

		int pivotIndex = MyMedianPivot(numbers, start, end);
		pivotIndex = MyMedianPartition(numbers, start, end, pivotIndex);

		if(k == pivotIndex)
		{
			return k;
		} else if(k < pivotIndex)
		{
			end = pivotIndex - 1;
		} else
		{
			start = pivotIndex + 1;
		}
	}
}

int GetMedianOfMedians(vector<int>& numbers)
{
	int median = MyMedianOfMedians(numbers, 1, numbers.size()-1, numbers.size() / 2);

	return numbers[numbers.size() / 2];
}

void main()
{
	unsigned int numbercount = 100000000;
	vector<int> numbers, temp;
	numbers.reserve(numbercount);
	TimerClass timer;
	double seconds = 0;
	int median = 0;

	GenerateNumbersMersenne(numbers, numbercount);
	//GenerateNumbersOngoing(numbers, numbercount);

	temp = numbers;
	cout << "Starting Quicksort" << endl;
	timer.StartTimer();
	median = GetMedianByQuicksort(temp);
	seconds = timer.GetTime();
	cout << "Median " << median << " in " << seconds << " seconds." << endl;

	temp = numbers;
	cout << "Starting NthElement" << endl;
	timer.StartTimer();
	median = GetMedianByNthElement(temp);
	seconds = timer.GetTime();
	cout << "Median " << median << " in " << seconds << " seconds." << endl;

	temp = numbers;
	cout << "Starting Random Selection" << endl;
	timer.StartTimer();
	median = GetMedianByRandomSelection(temp);
	seconds = timer.GetTime();
	cout << "Median " << median << " in " << seconds << " seconds." << endl;

	temp = numbers;
	cout << "Starting MedianOfMedians" << endl;
	timer.StartTimer();
	median = GetMedianOfMedians(temp);
	seconds = timer.GetTime();
	cout << "Median " << median << " in " << seconds << " seconds." << endl;

	//DebugArray(numbers);

	//cout << "Actual Median: " << GetActualMedian(numbers, numbercount) << endl;

}
