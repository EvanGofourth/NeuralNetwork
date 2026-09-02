/******************************************************************************
* @File: RandomUtility.h
* @Author: Evan Gofourth
* @About: RandomUtility is a C++17 library that offers static helpers for
*         generating random doubles.
*******************************************************************************/
#include<vector>

// Utilities for random number generation.
class RandomUtility{
public:

  // Default Constructor
  RandomUtility();
  // Default Destructor
  ~RandomUtility();

  /*******************************************************************************
  * @Method: GenerateRandomDouble
  * @Access: PUBLIC
  * @Description: Generates a random double.
  * @param min: the lower bound for random number generation.
  * @param max: the upper bound for random number generation.
  *******************************************************************************/
  static double GenerateRandomDouble(double min, double max);

  /*******************************************************************************
  * @Method: GenerateRandomVectorOfDoubles
  * @Access: PUBLIC
  * @Description: Generates a random vector of doubles.
  * @param size: the number of random doubles to generate.
  * @param min: the lower bound for random number generation.
  * @param max: the upper bound for random number generation.
  *******************************************************************************/
  static std::vector<double>* GenerateRandomVectorOfDoubles(int size, double min, double max);

private:
  // None.
};
