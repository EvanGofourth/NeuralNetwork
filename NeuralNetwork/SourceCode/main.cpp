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
