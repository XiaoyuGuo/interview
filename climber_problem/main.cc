/**
*
*  Author : GXY
*  Date   : 2017/4/4
*  This is an algorithm problem for interview
*
*/

#include <stdio.h>
#include <assert.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <queue>
#include <functional>
#include <iostream>

#define H_ARRAYSIZE(a) \
    ((sizeof(a) / sizeof(*(a))) / \
    static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

using std::vector;
using std::string;
using std::stringstream;
using std::priority_queue;
using std::less;

namespace climber_problem
{
	struct Mountain
	{
		size_t start;
		size_t end;
		size_t height;
		
	};

	bool operator<(const Mountain& m1, const Mountain& m2)
	{
		return m1.height < m2.height;
	}

	bool less_start(const Mountain& m1, const Mountain& m2)
	{
		return m1.start < m2.start;
	}

	/**

	* \brief Assemble one valid input into mountains_vector and get max

	* \param  const char* input    One valid input

	* \param  size_t* max_ptr      Warning, it can change outside value, represent max posX

	* Space complexity : O(n)

	* Time  complexity : O(n)

	* No memory leak or unclosed handle

	*/
	vector<Mountain> AssembleToVector(const char* input, size_t* max_ptr)
	{
		vector<Mountain> mountains_vector;
		string input_str(input);
		stringstream ss;

		for (size_t i = 0; i < input_str.size(); ++i)
		{
			if (input_str[i] == ',') input_str[i] = ' ';
		}

		ss << input_str;
		size_t num_of_mountains;
		ss >> num_of_mountains;

		for (size_t i = 0; i < num_of_mountains; ++i)
		{
			size_t start, end, height;
			ss >> start >> end >> height;
			*max_ptr = end > *max_ptr ? end : *max_ptr;
			Mountain m;
			m.start = start;
			m.end = end;
			m.height = height;
			mountains_vector.push_back(m);
		}

		return mountains_vector;
	}

	/**

	* \brief Calc the num of step climbing up

	* \param  const vector<Mountain> & mountains_vector

	* \return size_t up_step      Number of step climbing up

	* Space complexity : O(n)

	* Time  complexity : O(nlogn)

	* No memory leak or unclosed handle

	*/
	size_t UpStep(const vector<Mountain> & mountains_vector) {

		size_t up_step = 0;
		size_t pre_h = 0;
		size_t curr_x;
		size_t curr_h = 0;
		size_t i = 0;
		priority_queue<Mountain, vector<Mountain>, less<Mountain> > pq;
		
		while (!pq.empty() || i < mountains_vector.size())
		{
			if (pq.empty()) curr_x = mountains_vector[i].start; 
			else curr_x = pq.top().end; // Walk through prev mountain (high Priority)

			while (i < mountains_vector.size() && curr_x >= mountains_vector[i].start)
			{
				// Push all mountians (may have effect to current) into priority_queue
				// and update curr_x which means the highest priority mountains start
				if (mountains_vector[i].height > curr_h) {
					curr_x = mountains_vector[i].start;
				}
				pq.push(mountains_vector[i]);
				i++;
			}

			while (!pq.empty() && curr_x >= pq.top().end)
			{
				// Pop all moutains which will not effect current
				pq.pop();
			}

			pre_h = curr_h;
			if (pq.empty())
			{
				curr_h = 0;
			}
			else
			{
				curr_h = pq.top().height;
			}
			
			if (curr_h > pre_h) {
				up_step += curr_h - pre_h;
			}
		}
		
		return up_step;
	}
}

int resolve(const char* input)
{
	using namespace climber_problem;

	size_t max = 0;
	vector<Mountain> mountains_vector = AssembleToVector(input, &max);

	if (mountains_vector.size() == 0) return 0;

	//Sort by start
	std::sort(mountains_vector.begin(), mountains_vector.end(), less_start);

	//Step number of climb up is equal with climb down
	//Then we need add horizon distance
	return UpStep(mountains_vector) * 2 + max;
}

int main(int argc, char* argv[])
{
	const char* input[] = {
		"3\n1,3,2\n2,4,4\n6,7,5\n", //The giving example
		"1\n1,2,1\n",
		"2\n1,2,1\n2,3,2",
		"3\n1,2,1\n2,3,2\n3,6,1",
		"4\n1,2,1\n2,3,2\n3,6,1\n5,8,2",
		"5\n1,2,1\n2,3,2\n3,6,1\n5,8,2\n7,9,1",
		"1\n0,1,1",
		"2\n0,1,1\n2,4,3",
		"3\n0,1,1\n2,4,3\n3,5,1",
		"4\n0,1,1\n2,4,3\n3,5,1\n5,6,1",
		"5\n0,1,1\n2,4,3\n3,5,1\n5,6,1\n6,8,3",
		"1\n1,5,10\n",
		"0\n",
		"2\n0,1,10\n200,201,10"
	};
	int expectedSteps[] = { 25, 4, 7, 10, 14, 15, 3, 12, 13, 14, 20, 25, 0, 241 };
	for (size_t i = 0; i < H_ARRAYSIZE(input); ++i)
	{
		assert(resolve(input[i]) == expectedSteps[i]);
	}
	return 0;
}
