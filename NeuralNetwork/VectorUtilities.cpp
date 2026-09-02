#include"VectorUtilities.h"

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
std::vector<T>* VectorUtilities<T>::Normalize(std::vector<T>* vector)
{
    if (vector->size() == 0)
        throw std::invalid_argument("Can not normalize an empty vector.");
    std::vector<T>* normalVector = new std::vector<T>();
    normalVector->reserve(vector->size());
    T sum = vector->at(0);
    for (int i = 1; i < vector->size(); i++)
    {
        sum += vector->at(i);
    }
    if (sum == 0)
        throw std::invalid_argument("Can not normalize a vector that sums to 0.");
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
        throw std::invalid_argument("Can not compute max of empty vector.");
    T max = vector->at(0);
    for (int i = 0; i < vector->size(); i++)
    {
        if (vector->at(i) > max)
            max = vector->at(i);
    }
    return max;
}

template<class T>
T VectorUtilities<T>::Min(std::vector<T>* vector)
{
    if (vector->size() == 0)
        throw std::invalid_argument("Can not compute min of empty vector.");
    T min = vector->at(0);
    for (int i = 0; i < vector->size(); i++)
    {
        if (vector->at(i) < min)
            min = vector->at(i);
    }
    return min;
}

template<class T>
std::vector<T>* VectorUtilities<T>::Remove(std::vector<T>* vector, int index)
{
    vector->erase(vector->begin() + index);
    return vector;
}

template<class T>
std::vector<T>* VectorUtilities<T>::Insert(std::vector<T>* vector, T value, int index)
{
  vector->insert(vector->begin() + index, value);
  return vector;
}

template<class T>
std::vector<T>* VectorUtilities<T>::Reverse(std::vector<T>* vector)
{
  std::reverse(vector->begin(), vector->end());
  return vector;
}

template<class T>
std::vector<T>* VectorUtilities<T>::Concat(std::vector<T>* vector1, std::vector<T>* vector2)
{
  vector1->insert(vector1->end(), vector2->begin(), vector2->end());
  return vector1;
}
