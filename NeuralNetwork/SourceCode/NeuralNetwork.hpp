/******************************************************************************
* @File: NeuralNetwork.hpp
* @Author: Evan Gofourth
* @Date: 02/17/2021
* @About: A NeuralNetwork - for machine learning.
*******************************************************************************/
#include"Layer.hpp"
class NeuralNetwork{
    public:
        /*******************************************************************************
        * @Method: CONSTRUCTOR NeuralNetwork(std::vector<std::vector<double>*>* batch, std::vector<int>* subsequentLayerSizes)
        * @Access: PUBLIC
        * @Description: Constructs a neural network.
        * @param layerSizes: 
        *   A list of sizes for the layers in the NeuralNetwork. 
        *   Note that when training, the first layer's size is dynamically fitted to the input data.
        * @param categorizedTrainingDataDirectories: 
        *   A list to the directories containing your training data.
        *   Each directory must contain one classification of data for optimal training.
        *   The input data is automatically shuffled from the files in these directories.
        * @param outputDirectory:
        *   This is the directory where the result of your training will be saved.
        *   Output will be
        *       1) Accuracy.txt - the computed accuracy at each iteration.
        *       2) NeuralNetwork.txt - the configuration of your NeuralNetwork at its lowest loss.
        *******************************************************************************/
        NeuralNetwork(std::vector<int>* layerSizes, std::vector<std::string>* categorizedTrainingDataDirectories, std::string outputDirectory);

        
        /*******************************************************************************
        * @Method: Output
        * @Access: PUBLIC
        * @Description: Output from the final layer of the neural network.
        *******************************************************************************/
        std::vector<double>* Output();


        /*******************************************************************************
        * @Method: Train
        * @Access: PUBLIC
        * @Description: Trains your neural network for 1 epoch.
        *******************************************************************************/
        void Train();


    private:
        double Loss(int correctDeduction);
        void SetTrainingDataPaths(std::vector<std::string>* categorizedTrainingDataDirectoryPaths);
        void SetInputLayer(Layer* newLayer);
        void Train(std::vector<std::vector<double>*>* batch, int correctDeduction);
        std::vector<std::vector<std::vector<double>*>*>* GetWeights();
        std::vector<std::vector<double>*>* GetBiases();
        void SaveNeuralNetworkToFile(std::string fileName);
        void InitNeuralNetwork(Layer* firstLayer, std::vector<int>* subsequentLayerSizes);
        std::vector<Layer*>* _layers;
        std::vector<std::string>* _categorizedTrainingDataDirectoryPaths;
        std::vector<std::vector<std::string>*>* _categorizedTrainingDataFilePaths;
        std::vector<std::tuple<std::string, int>>* _shuffledInputData;
        double _lowestLoss;
        int _positives;
        int _negatives;
        std::string _savedDataDirectory;
};