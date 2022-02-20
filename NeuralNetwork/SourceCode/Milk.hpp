/******************************************************************************
* @File: Milk.hpp
* @Author: Evan Gofourth
* @Date: 02/17/2021
* @About: Milk is a C++17 library that serves as a general utility toolkit.
*   It mostly pertains to vectors, rng, and windows.
*******************************************************************************/
#include<iostream>
#include<string>
#include<vector>
#include<functional>
#include<sstream>
#include<stdexcept>
#include<filesystem>
#include<random>
#include<ctime>

template<class T>
std::function<T(std::string)> default_Split_lambda = [](std::string s) -> T { return s; };
template<class T>
std::function<T(int)> default_RGBAVectorFromImage_lambda = [](int i) -> T { return i; };

template<class T>
class VectorUtilities{
public:

  // Default Constructor
  VectorUtilities();
  // Default Destructor
  ~VectorUtilities();

  /*******************************************************************************
  * @Method: Join
  * @Access: PUBLIC
  * @Description: Returns a std::string of delim delimited elems.
  * @Requirements: T must have operator << overloaded
  * @param elems: the vector of elements to split.
  * @param delim: the delimiter to split the elements on.
  *******************************************************************************/
  static std::string Join(std::vector<T>* elems, char delim);


  /*******************************************************************************
  * @Method: Split
  * @Access: PUBLIC
  * @Description: Splits a string on delim. Returns a std::vector of elements <T>
  *   casted using a user provided std::function<T(std::string)> func.
  * @param string:   the string to Split.
  * @param delim:    the delimiter to Split the string on.
  * @param cast:     a std::function to cast the split elements to type T.
  *                 cast defaults to simply returning the spit string.
  *******************************************************************************/
  static std::vector<T>* Split(std::string string, char delim, std::function<T(std::string)> cast = default_Split_lambda<T>);


  /*******************************************************************************
  * @Method: RGBAVectorFromImage
  * @Access: PUBLIC
  * @Description: Converts an image file (PNG, etc.) to a std::vector<std::vector<T>>. The child vectors are length 4: [R,G,B,A].
  * @param fileName: the name of the image file to read.
  * @param cast:     a std::function to cast the RGBA values to type T.
  *                 cast defaults to simply returning an integer in range (0 to 255).
  *******************************************************************************/
  static std::vector<std::vector<double>*>* RGBAVectorFromImage(std::string fileName);

  /*******************************************************************************
  * @Method: GetAllFileNamesWithinFolder
  * @Access: PUBLIC
  * @Description: Returns a list of file names within a directory.
  * @param folder: the directory to scan.
  *******************************************************************************/
  static std::vector<std::string>* GetAllFileNamesWithinFolder(std::string folder);


  /*******************************************************************************
  * @Method: Max
  * @Access: PUBLIC
  * @Description: Returns max of a vector.
  *******************************************************************************/
  static T Max(std::vector<T>* vector);

  /*******************************************************************************
   * @Method: Max
   * @Access: PUBLIC
   * @Description: Normalizes a vector.
   *******************************************************************************/
  static std::vector<T>* Normalize(std::vector<T>* vector);

  /*******************************************************************************
    * @Method: GenerateRandomVectorOfDoubles
    * @Access: PUBLIC
    * @Description: Generates a random vector of doubles.
    * @param size: the number of random doubles to generate.
    * @param min: the lower bound for random number generation.
    * @param max: the upper bound for random number generation.
    *******************************************************************************/
  static std::vector<double>* GenerateRandomVectorOfDoubles(int size, double min, double max);

  /*******************************************************************************
    * @Method: GenerateRandomDouble
    * @Access: PUBLIC
    * @Description: Generates a random double.
    * @param min: the lower bound for random number generation.
    * @param max: the upper bound for random number generation.
    *******************************************************************************/
  static double GenerateRandomDouble(double min, double max);

private:
  static bool LoadImage(std::vector<unsigned char>& image, const std::string& filename, int& x, int&y);
};

template<class T>
std::vector<double>* VectorUtilities<T>::GenerateRandomVectorOfDoubles(int size, double min, double max)
{
    std::vector<double>* result = new std::vector<double>();
    for (int i = 0; i < size; i++)
    {
        result->push_back(GenerateRandomDouble(min, max));
    }
    return result;
}

template<class T>
double VectorUtilities<T>::GenerateRandomDouble(double min, double max)
{
    using namespace std;

    static default_random_engine generator(unsigned(time(nullptr)));
    uniform_real_distribution<double> distribution(min, max);
    double result = distribution(generator);
    return result;
}

template<class T>
VectorUtilities<T>::VectorUtilities()
{
}

template<class T>
VectorUtilities<T>::~VectorUtilities()
{
}

template<class T>
std::string VectorUtilities<T>::Join(std::vector<T>* elems, char delim)
{
    std::stringstream ss;
    for (int i = 0; i < elems->size(); i++)
    {
        ss << elems->at(i);
        ss << delim;
    }
    std::string result = ss.str();
    result.pop_back();
    delete elems;
    return result;
}

template<class T>
std::vector<T>* VectorUtilities<T>::Split(std::string string, char delim, std::function<T(std::string)> cast)
{
    std::string* temp = new std::string();
    std::vector<T>* result = new std::vector<T>();

    for (int i = 0; i <= string.length(); i++)
    {
        if (string.length() == 0)
            continue;
        if (i < string.length() && string[i] != delim)
        {
            temp->push_back(string[i]);
        }
        else
        {
            result->push_back(cast(*temp));
            delete temp;
            temp = new std::string();
        }
    }
    delete temp;
    return result;
}

template<class T>
std::vector<std::string>* VectorUtilities<T>::GetAllFileNamesWithinFolder(std::string folder)
{
    std::vector<std::string>* names = new std::vector<std::string>();
    for (const auto& file : std::filesystem::directory_iterator(folder))
        names->push_back(file.path().generic_string());
    return names;
}

template<class T>
std::vector<T>* VectorUtilities<T>::Normalize(std::vector<T>* vector)
{
    if (vector->size() == 0)
        throw new std::invalid_argument("Can not normalize an empty vector.");
    std::vector<T>* normalVector = new std::vector<T>();
    T sum = vector->at(0);
    for (int i = 1; i < vector->size(); i++)
    {
        sum += vector->at(i);
    }
    for (int i = 0; i < vector->size(); i++)
    {
        normalVector->push_back(vector->at(i) / sum);
    }
    return normalVector;
}

template<class T>
T VectorUtilities<T>::Max(std::vector<T>* vector)
{
    if (vector->size() == 0)
        throw new std::invalid_argument("Can not compute max of empty vector.");
    T max = vector->at(0);
    for (int i = 0; i < vector->size(); i++)
    {
        if (vector->at(i) > max)
            max = vector->at(i);
    }
    return max;
}
