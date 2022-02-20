#include"Neuron.hpp"
Neuron::~Neuron()
{
    delete _inputs;
    delete _weights;
    delete _previousWeights;
}

Neuron::Neuron(std::vector<double>* inputVector)
{
    _previousBias = 0.00;
    _previousWeights = new std::vector<double>();
    _weights = new std::vector<double>();
    _isInFinalLayer = false;
    _inputs = VectorUtilities<double>::Normalize(inputVector);
    _bias = 0.00;
    // initialize random weights.
    for (int i=0; i<_inputs->size(); i++)
    {
        _weights->push_back(VectorUtilities<double>::GenerateRandomDouble(-1.0,1.0));
        _previousWeights->push_back(_weights->at(i));
    }
}

void Neuron::Stimulate()
{
    _previousBias = _bias; // stash bias to rollback if needed.
    delete _previousWeights;
    _previousWeights = new std::vector<double>();
    for(int i = 0; i < _weights->size(); i++)
    {
        _previousWeights->push_back(_weights->at(i)); // stash previous weights to rollback if needed.
        _weights->at(i) += 0.05 * VectorUtilities<double>::GenerateRandomDouble(-1.0, 1.0);
    }
    _bias += 0.05 * VectorUtilities<double>::GenerateRandomDouble(-1.0, 1.0);
}

void Neuron::Unstimulate()
{
    delete _weights;
    _weights = new std::vector<double>();
    for (int i = 0; i < _previousWeights->size(); i++)
    {
        _weights->push_back(_previousWeights->at(i));
    }
    _bias = _previousBias;
}

void Neuron::IsInFinalLayer(bool newValue)
{
    _isInFinalLayer = newValue;
}

void Neuron::SetInputs(std::vector<double>* newInputs)
{
    delete _inputs;
    _inputs = VectorUtilities<double>::Normalize(newInputs);
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
            _weights->push_back(VectorUtilities<double>::GenerateRandomDouble(-1.0, 1.0));
        }
    }
    for(int i = 0; i < _inputs->size(); i++)
    {
        output += (_weights->at(i) * _inputs->at(i));
    }
    output += _bias;
    if(_isInFinalLayer)
        return pow(2.71828182846, output); // softmax activation function
    return output > 0.00 ? output : 0.00; // rectified linear activation function
}