/******************************************************************************
* @File: VectorUtilities.h
* @Author: Evan Gofourth
* @Date: 02/17/2021
* @About: VectorUtilities is a C++17 library that serves as a general utility
*         toolkit for static vector operations.
*******************************************************************************/
#include<string>
#include<vector>
#include<functional>
#include<sstream>
#include<stdexcept>
#include<random>
#include<algorithm>
#include<map>

template<class T>
std::function<T(std::string)> default_Split_lambda = [](std::string s) -> T { return s; };

// Utilities for vector operations.
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
  * @Description: Returns a std::string of delim delimited elems. Does not take
  *   ownership of elems; the caller retains it and is responsible for deleting it.
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
  //[](std::string s) {return std::stoi(s);} // example lambda for integer vector
  static std::vector<T>* Split(std::string string, char delim, std::function<T(std::string)> cast = default_Split_lambda<T>);

  /*******************************************************************************
  * @Method: Max
  * @Access: PUBLIC
  * @Description: Returns max of a vector.
  *******************************************************************************/
  static T Max(std::vector<T>* vector);

  /*******************************************************************************
  * @Method: Min
  * @Access: PUBLIC
  * @Description: Returns min of a vector.
  *******************************************************************************/
  static T Min(std::vector<T>* vector);

  /*******************************************************************************
   * @Method: Normalize
   * @Access: PUBLIC
   * @Description: Normalizes a vector so its elements sum to 1.
   * @Throws: std::invalid_argument if vector is empty or its elements sum to 0.
   *******************************************************************************/
  static std::vector<T>* Normalize(std::vector<T>* vector);

  /*******************************************************************************
   * @Method: Remove
   * @Access: PUBLIC
   * @Description: Removes the element at index.
   *******************************************************************************/
  static std::vector<T>* Remove(std::vector<T>* vector, int index);

  /*******************************************************************************
   * @Method: Insert
   * @Access: PUBLIC
   * @Description: Inserts value at index in vector.
   *******************************************************************************/
  static std::vector<T>* Insert(std::vector<T>* vector, T value, int index);

  /*******************************************************************************
   * @Method: Reverse
   * @Access: PUBLIC
   * @Description: Reverses a vector.
   *******************************************************************************/
  static std::vector<T>* Reverse(std::vector<T>* vector);
  /*******************************************************************************
   * @Method: Concat
   * @Access: PUBLIC
   * @Description: Concatenates two vectors.
   *******************************************************************************/
  static std::vector<T>* Concat(std::vector<T>* vector1, std::vector<T>* vector2);
private:
    // None.
};
