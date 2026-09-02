#include"NeuralNetwork.hpp"
int main()
{
    // Example usage:
    // 1) Create a new NeuralNetwork
    NeuralNetwork* nn = new NeuralNetwork(
        // A std::vector<int>* of your initial layer sizes. (First layer will be dynamically changed in training).
        VectorUtilities<int>::Split("5,5,2", ',', [](std::string s) {return std::stoi(s);}), 
        // A std::vector<std::string>* of the directories to your training data.
        VectorUtilities<std::string>::Split("D:/DESKTOP/PROGRAMMING/Archive_Working/NeuralNetwork/TestData/rider,D:/DESKTOP/PROGRAMMING/Archive_Working/NeuralNetwork/TestData/soimoi", ','),
        // A string of the path to where you want your saved_data output.
        "D:/DESKTOP/PROGRAMMING/Archive_Working/NeuralNetwork/TestData/SavedData"
    );
    // 2) Train it. Second argument is the number of epochs (passes over the training data).
    nn->Train(10);
}
