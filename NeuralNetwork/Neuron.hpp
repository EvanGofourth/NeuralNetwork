/******************************************************************************
* @File: Neuron.hpp
* @Author: Evan Gofourth
* @Date: 02/17/2021
* @About: The most basic node in a NeuralNetwork, typically used in a Layer.
*******************************************************************************/
#include<iostream>
#include<string>
#include<vector>
#include<functional>
#include<sstream>
#include<exception>
#include<random>
#include<chrono>
#include<cmath>
#include<chrono>
#include<ctime> 
#include<fstream>
#include<exception>
#include<math.h>
#include<algorithm>
#include"Milk.hpp"
class Neuron{
    public:

        ~Neuron();

        /*******************************************************************************
        * @Method: CONSTRUCTOR Neuron(std::vector<double>* inputVector)
        * @Access: PUBLIC
        * @Description: Constructs a neuron from a vector of input.
        * @param inputVector: The input to the neuron.
        *******************************************************************************/
        Neuron(std::vector<double>* inputVector);


        /*******************************************************************************
        * @Method: Output
        * @Access: PUBLIC
        * @Description: Returns the scalar output of the Neuron.
        *******************************************************************************/
        double Output();


        /*******************************************************************************
        * @Method: Stimulate
        * @Access: PUBLIC
        * @Description: Randomly modifies the weights and bias of the neuron.
        *******************************************************************************/
        void Stimulate();


        /*******************************************************************************
        * @Method: Unstimulate
        * @Access: PUBLIC
        * @Description: Restores weights and bias from _previousWeights and _previousBias.
        *******************************************************************************/
        void Unstimulate();


        /*******************************************************************************
        * @Method: GetBias
        * @Access: PUBLIC
        * @Description: Returns _bias.
        *******************************************************************************/
        double GetBias();


        /*******************************************************************************
        * @Method: GetWeights
        * @Access: PUBLIC
        * @Description: Returns _weights.
        *******************************************************************************/
        std::vector<double>* GetWeights();


        /*******************************************************************************
        * @Method: SetInputs
        * @Access: PUBLIC
        * @Description: Sets _inputs to newInputs
        *******************************************************************************/
        void SetInputs(std::vector<double>* newInputs);


        /*******************************************************************************
        * @Method: IsInFinalLayer
        * @Access: PUBLIC
        * @Description: Sets _isInFinalLayer to newValue.
        *******************************************************************************/
        void IsInFinalLayer(bool newValue);
    private:
        std::vector<double>* _inputs;
        std::vector<double>* _weights;
        std::vector<double>* _previousWeights;
        double _bias;
        double _previousBias;
        bool _isInFinalLayer;
};