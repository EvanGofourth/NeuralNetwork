#include"Layer.hpp"

Layer::~Layer()
{
    for(int i = 0; i < _neurons->size(); i++)
    {
        delete _neurons->at(i);
    }
}

Layer::Layer(std::vector<Neuron*>* neurons)
{
    _neurons = neurons;
}

Layer::Layer(std::vector<std::vector<double>*>* batch, bool isFinalLayer)
{
    std::vector<Neuron*>* neurons = new std::vector<Neuron*>();
    for(int i = 0; i < batch->size(); i++)
    {
        Neuron* n = new Neuron(batch->at(i));
        n->IsInFinalLayer(isFinalLayer);
        neurons->push_back(n);
    }
    _neurons = neurons;
}

std::vector<double>* Layer::Output()
{
    std::vector<double> * output = new std::vector<double>();
    for(int i = 0; i < _neurons->size(); i++)
    {
        Neuron* n = _neurons->at(i);
        output->push_back(n->Output());
    } 
    return output;
}

std::vector<double>* Layer::GetBiases()
{
    std::vector<double>* result = new std::vector<double>();
    for(int i = 0; i < _neurons->size(); i++)
    {
        result->push_back(_neurons->at(i)->GetBias());
    }
    return result;
}


std::vector<std::vector<double>*>* Layer::GetWeights()
{
    std::vector<std::vector<double>*>* result = new std::vector<std::vector<double>*>();
    for(int i = 0; i < _neurons->size(); i++)
    {
        result->push_back(_neurons->at(i)->GetWeights());
    }
    return result;
}

void Layer::Stimulate()
{
    for(int i = 0; i < _neurons->size(); i++)
    {
        _neurons->at(i)->Stimulate();
    }
}

void Layer::Unstimulate()
{
    for (int i = 0; i < _neurons->size(); i++)
    {
        _neurons->at(i)->Unstimulate();
    }
}

void Layer::SetInputs(std::vector<double>* newInputs)
{
    for(int i = 0; i < _neurons->size(); i++)
    {
        _neurons->at(i)->SetInputs(newInputs);
    }
}

int Layer::NeuronCount()
{
    return _neurons->size();
}
