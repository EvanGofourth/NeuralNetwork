#include"RandomUtility.h"
#include<random>
#include<ctime>

RandomUtility::RandomUtility()
{
}

RandomUtility::~RandomUtility()
{
}

double RandomUtility::GenerateRandomDouble(double min, double max)
{
    using namespace std;

    static default_random_engine generator(unsigned(time(nullptr)));
    uniform_real_distribution<double> distribution(min, max);
    double result = distribution(generator);
    return result;
}

std::vector<double>* RandomUtility::GenerateRandomVectorOfDoubles(int size, double min, double max)
{
    std::vector<double>* result = new std::vector<double>();
    for (int i = 0; i < size; i++)
    {
        result->push_back(GenerateRandomDouble(min, max));
    }
    return result;
}
