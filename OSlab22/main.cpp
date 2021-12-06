#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <thread>


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


void mergeSorMultiprocess(std::vector<int> &array, unsigned int avaliableThreds) {

    if (array.size() == 1 || array.size() == 0)
        return;

    int middle = array.size() / 2;
    std::vector<int> left, right;
    for (int i = 0; i < middle; i++)
        left.push_back(array[i]);
    for(int i = middle; i < array.size(); ++i)
        right.push_back(array[i]);

    if(avaliableThreds >= 2)
    {
        avaliableThreds -= 2;
        std::thread thr1(mergeSorMultiprocess, std::ref(left), avaliableThreds / 2);
        std::thread thr2(mergeSorMultiprocess, std::ref(right), avaliableThreds - avaliableThreds / 2);
        thr1.join();
        thr2.join();
    } else if (avaliableThreds == 1)
    {
        --avaliableThreds;
        std::thread thr1(mergeSorMultiprocess, std::ref(left), avaliableThreds);
        mergeSorMultiprocess(right, avaliableThreds);
        thr1.join();
    }
    else
    {
        mergeSorMultiprocess(left, avaliableThreds);
        mergeSorMultiprocess(right, avaliableThreds);
    }

    auto itL = left.begin();
    auto itR = right.begin();
    array.clear();
    while ((itL != left.end()) || (itR != right.end()))
    {
        if(itR == right.end())
        {
            array.push_back(*itL);
            ++itL;
        }
        else if(itL == left.end())
        {
            array.push_back(*itR);
            ++itR;
        }
        else
        {
            if (*itR > *itL)
            {
                array.push_back(*itL);
                ++itL;
            }
            else
            {
                array.push_back(*itR);
                ++itR;
            }
        }

    }


}


int main() {
    std::cout << "Hello, World!" << std::endl;
    std::vector<int> vec;
    for (unsigned int i = 0; i <  1023 * 1024; i++) {
        vec.push_back(rand());
    }
    auto t1 = high_resolution_clock::now();
    mergeSorMultiprocess(vec, 16);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() << "ms" << std::endl;
    //for (auto i : vec)
    //    std::cout << i << ' ';
    return 0;
}
