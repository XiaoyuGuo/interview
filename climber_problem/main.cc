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

#define H_ARRAYSIZE(a) \
    ((sizeof(a) / sizeof(*(a))) / \
    static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

using std::vector;
using std::string;
using std::stringstream;

namespace climber_problem
{
    struct Mountain
    {
        size_t start;
        size_t end;
        size_t height;
    };

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
            if (input_str[i]==',') input_str[i]=' ';
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

    * \brief Split mountains' vector to several heaps

    * \param  const vector<Mountain>& mountains_vector   Mountains' vector

    * \return vector<size_t> split                       Each pair of value represent a heap in [a, b]

    * Space complexity : O(n)

    * Time  complexity : O(n)

    * No memory leak or unclosed handle

    */
    vector<size_t> SplitHeaps(const vector<Mountain>& mountains_vector)
    {
        vector<size_t> split;

        split.push_back(0);

        size_t max_end = mountains_vector[0].end;
        for (size_t i = 1; i < mountains_vector.size(); ++i)
        {
            if (mountains_vector[i].start > max_end)
            {
                split.push_back(i-1);
                split.push_back(i);
            }
            max_end = mountains_vector[i].end > max_end ? mountains_vector[i].end : max_end;
        }
        
        split.push_back(mountains_vector.size() - 1);

        return split;
    }


    /**

    * \brief Calc the num of step climbing up

    * \param  vector<Mountain>& mountains_vector    mountians_vector

    * \param  const size_t start                    Scope of mountains_vector [start, end]

    * \param  const size_t end                      Scope of mountains_vecotr [start, end]

    * \return step_of_climb_up                      Step number of climb up in this heap

    * Space complexity : O(n)

    * Time  complexity : O(n^2)                     Worst situation. Much better in practice than normal algorithm

    * No memory leak or unclosed handle

    */
    size_t StepOfClimbUp(const vector<Mountain>& mountains_vector,
                         const size_t start,
                         const size_t end)
    {
        vector<Mountain> having_effect;
        size_t step_of_climb_up = 0;

        for (size_t i = start; i <= end; i++)
        {
            size_t effect_height = 0;
            for (vector<Mountain>::iterator iter = having_effect.begin();
                 iter != having_effect.end();)
            {
                if ((*iter).end >= mountains_vector[i].start)
                {
                    effect_height = effect_height > (*iter).height ? effect_height : (*iter).height;
                    iter++;
                }
                else
                {
                    iter = having_effect.erase(iter);
                }
            }

            if (mountains_vector[i].height > effect_height)
            {
                step_of_climb_up += mountains_vector[i].height - effect_height;
            }
            having_effect.push_back(mountains_vector[i]);
        }

        return step_of_climb_up;
    }
}

int resolve(const char* input)
{
    using namespace climber_problem;

    size_t max = 0;
    vector<Mountain> mountains_vector = AssembleToVector(input, &max);

    if (mountains_vector.size() == 0) return 0;
    std::sort(mountains_vector.begin(), mountains_vector.end(), less_start);

    vector<size_t> split = SplitHeaps(mountains_vector);
    size_t step_of_climb_up = 0;

    for (size_t i = 0; i < split.size(); i += 2)
    {
        step_of_climb_up += StepOfClimbUp(mountains_vector, split[i], split[i+1]);
       
    }

    //Step number of climb up is equal with climb down
    //Then we need add horizon distance
    return max + 2 * step_of_climb_up;
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
    int expectedSteps[] = {25, 4, 7, 10, 14, 15, 3, 12, 13, 14, 20, 25, 0, 241};
    for (size_t i = 0; i < H_ARRAYSIZE(input); ++i)
    {
        assert(resolve(input[i]) == expectedSteps[i]);
    }
    return 0;
}
