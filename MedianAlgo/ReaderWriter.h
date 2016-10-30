#pragma once
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

class ReaderWriter
{
public:
	ReaderWriter(string inputFilename, vector<int>& numbers);
	~ReaderWriter();

	static void GenerateInputFile(const string outputFilename, const vector<int>& numbers);

	int numberOfLines, lineLength;
private:
	void extractValuesFromContent(vector<int>& numbers);
	void readInputFile(string inputFilename);

	string content;
	vector<char> line;
};
