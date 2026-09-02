#include"Neuron.hpp"
Neuron::~Neuron()
{
    delete _inputs;
    delete _weights;
}

Neuron::Neuron(std::vector<double>* inputVector)
{
    _weights = new std::vector<double>();
    _isInFinalLayer = false;
    _inputs = new std::vector<double>(*inputVector); // copy; caller retains ownership of inputVector.
    _bias = 0.00;
    _lastZ = 0.00;
    // initialize random weights.
    for (int i=0; i<_inputs->size(); i++)
    {
        _weights->push_back(RandomUtility::GenerateRandomDouble(-1.0,1.0));
    }
}

double Neuron::ActivationDerivative()
{
    if (_isInFinalLayer)
        return 1.00; // identity; softmax is applied layer-wide by NeuralNetwork::Output().
    return _lastZ > 0.00 ? 1.00 : 0.00; // ReLU'
}

std::vector<double>* Neuron::Backward(double dLoss_dActivation, double learningRate)
{
    double delta = dLoss_dActivation * ActivationDerivative();
    std::vector<double>* propagated = new std::vector<double>();
    propagated->reserve(_weights->size());
    for (int i = 0; i < _weights->size(); i++)
    {
        propagated->push_back(delta * _weights->at(i)); // uses pre-update weight.
        _weights->at(i) -= learningRate * delta * _inputs->at(i);
    }
    _bias -= learningRate * delta;
    return propagated;
}

void Neuron::IsInFinalLayer(bool newValue)
{
    _isInFinalLayer = newValue;
}

void Neuron::SetInputs(std::vector<double>* newInputs)
{
    delete _inputs;
    _inputs = new std::vector<double>(*newInputs); // copy; caller retains ownership of newInputs.
}

double Neuron::GetBias()
{
    return _bias;
}

std::vector<double>* Neuron::GetWeights()
{
    return _weights;
}

double Neuron::Output()
{
    int count = 0;
    double output = 0.00;
    if(_weights->size() < _inputs->size())
    {
        while(_weights->size() != _inputs->size())
        {
            _weights->push_back(RandomUtility::GenerateRandomDouble(-1.0, 1.0));
        }
    }
    for(int i = 0; i < _inputs->size(); i++)
    {
        output += (_weights->at(i) * _inputs->at(i));
    }
    output += _bias;
    _lastZ = output;
    if(_isInFinalLayer)
        return output; // raw logit; NeuralNetwork::Output() applies softmax layer-wide.
    return output > 0.00 ? output : 0.00; // rectified linear activation function
}