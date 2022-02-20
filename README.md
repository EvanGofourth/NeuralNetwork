# NeuralNetwork
A NeuralNetwork implemented from scratch in C++.
## Getting Started
To use: Copy paste all the files from [SourceCode](NeuralNetwork/SourceCode/) (excluding main.cpp) into your source folder and #include"NeuralNetwork.hpp" where it will be used.<br>
Example usage:
```c++
#include"NeuralNetwork.hpp"
int main()
{
    // Example usage:
    // 1) Create a new NeuralNetwork
    NeuralNetwork* nn = new NeuralNetwork(
        // A std::vector<int>* of your initial layer sizes. (First layer will be dynamically changed in training).
        VectorUtilities<int>::Split("5,5,2", ',', [](std::string s) {return std::stoi(s);}), 
        // A std::vector<std::string>* of the directories to your training data.
        VectorUtilities<std::string>::Split("C:/FAKE_PATH/training_data/Cat,C:/FAKE_PATH/training_data/Dog", ','),
        // A string of the path to where you want your saved_data output.
        "C:/FAKE_PATH/saved_data"
    );
    // 2) Train it.
    nn->Train();
}
```
## Notes
Make sure your last layer has a number of nodes equal to your number of training data categories.<br>
To use NeuralNetwork out of the box you should have one directory per data classification category.<br>
Example:
* C:/FAKE_PATH/Cats
* C:/FAKE_PATH/Dogs
* etc.<br>
