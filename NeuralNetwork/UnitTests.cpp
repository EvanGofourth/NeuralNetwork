#include<iostream>
#include<string>
#include<vector>
#include"Milk.hpp"

bool Join_CorrectlyJoinsListOfIntegers()
{
    std::vector<int>* testVector = new std::vector<int>();
    testVector->push_back(10);
    testVector->push_back(20);
    testVector->push_back(30);
    std::string str = VectorUtilities<int>::Join(testVector, ',');
    return (str == "10,20,30");
}

bool Split_CorrectlySplitsStringOfIntegers_WithDefaultLambda()
{
  std::string str = "10,20,30";
  std::vector<std::string>* result = VectorUtilities<std::string>::Split(str, ',');
  return (result->at(0) == "10" && result->at(1) == "20" && result->at(2) == "30");
}

bool Split_CorrectlySplitsStringOfIntegers_WithCustomLambda()
{
  std::string str = "10,20,30";
  std::function<int(std::string)> custom_lambda = [](std::string s)->int{return std::stoi(s);};
  std::vector<int>* result = VectorUtilities<int>::Split(str, ',', custom_lambda);
  return (result->at(0) == 10 && result->at(1) == 20 && result->at(2) == 30);
}

// Test utility functions.
int RunUnitTests()
{
    int errors = 0;
    if(!Join_CorrectlyJoinsListOfIntegers())
    {
        errors++;
        std::cout << "<FAILED> Join_CorrectlyJoinsListOfIntegers\n";
    }
    if(!Split_CorrectlySplitsStringOfIntegers_WithDefaultLambda())
    {
        errors++;
        std::cout << "<FAILED> Split_CorrectlySplitsStringOfIntegers_WithDefaultLambda\n";
    }
    if(!Split_CorrectlySplitsStringOfIntegers_WithCustomLambda())
    {
        errors++;
        std::cout << "<FAILED> Split_CorrectlySplitsStringOfIntegers_WithCustomLambda\n";
    }
    return errors;
}

int Go()
{
  int errors = RunUnitTests();
  std::cout << "Unit tests completed with " + std::to_string(errors) + " error(s).\n";
  return errors;
}
