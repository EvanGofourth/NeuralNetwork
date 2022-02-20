/******************************************************************************
* @File: Layer.hpp
* @Author: Evan Gofourth
* @Date: 02/17/2021
* @About: A layer of Neurons for use in a NeuralNetwork.
*******************************************************************************/
#include"Neuron.hpp"
class Layer{
    public:

        ~Layer();

        /*******************************************************************************
        * @Method: CONSTRUCTOR Layer(std::vector<Neuron*>* neurons)
        * @Access: PUBLIC
        * @Description: Constructs a Layer from a vector of Neurons.
        * @param neurons: The vector of Neurons.
        *******************************************************************************/
        Layer(std::vector<Neuron*>* neurons);


        /*******************************************************************************
        * @Method: CONSTRUCTOR Layer(std::vector<Neuron*>* neurons)
        * @Access: PUBLIC
        * @Description: Constructs a Layer from a batch of data.
        * @param batch: The batch used to build Neurons for the Layer.
        *******************************************************************************/
        Layer(std::vector<std::vector<double>*>* batch, bool isFinalLayer = false);


        /*******************************************************************************
        * @Method: Output
        * @Access: PUBLIC
        * @Description: Returns the vector output of the Layer.
        *******************************************************************************/
        std::vector<double>* Output();


        /*******************************************************************************
        * @Method: SetInputs
        * @Access: PUBLIC
        * @Description: Sets the inputs for each of the neurons in the layer.
        *******************************************************************************/
        void SetInputs(std::vector<double>* newInputs);


        /*******************************************************************************
        * @Method: GetBiases
        * @Access: PUBLIC
        * @Description: Returns biases from each neuron in the layer.
        *******************************************************************************/
        std::vector<double>* GetBiases();


        /*******************************************************************************
        * @Method: GetWeights
        * @Access: PUBLIC
        * @Description: Returns weights from each neuron in the layer.
        *******************************************************************************/
        std::vector<std::vector<double>*>* GetWeights();

        /*******************************************************************************
        * @Method: NeuronCount
        * @Access: PUBLIC
        * @Description: Returns count of neurons in the layer.
        *******************************************************************************/
        int NeuronCount();

        /*******************************************************************************
        * @Method: Stimulate
        * @Access: PUBLIC
        * @Description: Randomly adjusts weights and biases for each neuron in the layer.
        *******************************************************************************/
        void Stimulate();

        /*******************************************************************************
        * @Method: Unstimulate
        * @Access: PUBLIC
        * @Description: Restores previous weights and biases for each neuron in the layer.
        *******************************************************************************/
        void Unstimulate();

    private:
        std::vector<Neuron*>* _neurons;
};