#include"NeuralNetwork.hpp"
NeuralNetwork::NeuralNetwork(std::vector<int>* layerSizes, std::vector<std::string>* categorizedTrainingDataDirectories, std::string outputDirectory)
{
    SetTrainingDataPaths(categorizedTrainingDataDirectories);
    std::vector<Neuron*>* neurons = new std::vector<Neuron*>();
    for (int i = 0; i < layerSizes->at(0); i++)
    {
        Neuron* n = new Neuron(VectorUtilities<double>::GenerateRandomVectorOfDoubles(4, -1.0, 1.0));
        neurons->push_back(n);
    }
    Layer* layer = new Layer(neurons);
    layerSizes->erase(layerSizes->begin());
    InitNeuralNetwork(layer, layerSizes);
    _savedDataDirectory = outputDirectory;
}

void NeuralNetwork::InitNeuralNetwork(Layer* firstLayer, std::vector<int>* subsequentLayerSizes)
{
    _lowestLoss = 1;
    std::vector<Layer*>* layers = new std::vector<Layer*>();
    layers->push_back(firstLayer);
    for(int i = 0; i < subsequentLayerSizes->size(); i++)
    {
        std::vector<Neuron*>* neurons = new std::vector<Neuron*>();
        int numberOfNeuronsInLayer = subsequentLayerSizes->at(i);
        for(int j = 0; j < numberOfNeuronsInLayer; j++)
        {
            Neuron * n = new Neuron(layers->at(i)->Output());
            if(i == subsequentLayerSizes->size() - 1)
                n->IsInFinalLayer(true);
            neurons->push_back(n);
        }
        Layer* layer = new Layer(neurons);
        layers->push_back(layer);
    }
    _layers = layers;
}

std::vector<double>* NeuralNetwork::Output()
{
    std::vector<double>* normalizedOutput = new std::vector<double>();
    std::vector<double>* finalLayerOutput = _layers->at(_layers->size() - 1)->Output();
    double normBase = 0.00;
    for (int i = 0; i < finalLayerOutput->size(); i++)
        normBase += finalLayerOutput->at(i);
    for (int i = 0; i < finalLayerOutput->size(); i++)
        normalizedOutput->push_back(finalLayerOutput->at(i) / normBase);
    delete finalLayerOutput;
    return normalizedOutput;
}

void NeuralNetwork::SetInputLayer(Layer* newLayer)
{   
    Layer* toDelete = _layers->at(0);
    _layers->erase(_layers->begin());
    _layers->insert(_layers->begin(), newLayer);
    delete toDelete;
}

void NeuralNetwork::Train()
{
    _positives = 0;
    _negatives = 0;
    for(int i = 0; i < _shuffledInputData->size(); i++)
    {
        std::cout << "------------------------------------------------------\n";
        std::cout << "Iteration: " << i + 1 << " of " << _shuffledInputData->size() << std::endl;
        std::cout << "Training from image: " << std::get<0>(_shuffledInputData->at(i)) << std::endl;
        Train(VectorUtilities<double>::RGBAVectorFromImage(
            std::get<0>(_shuffledInputData->at(i))),
            std::get<1>(_shuffledInputData->at(i))
            );
        if (i + 1 == _shuffledInputData->size())
        {
            std::cout << _savedDataDirectory << "/NeuralNetwork.txt..\n";
            SaveNeuralNetworkToFile(_savedDataDirectory + "/NeuralNetwork.txt");
            std::cout << "Saving complete!\n";
        }
        std::cout << "------------------------------------------------------\n";
    }
}

void NeuralNetwork::Train(std::vector<std::vector<double>*>* batch, int correctDeduction)
{
    // replace input layer to match data shape.
    SetInputLayer(new Layer(batch));
    for(int i = 1; i < _layers->size(); i++)
    {
        _layers->at(i)->SetInputs(_layers->at(i-1)->Output());
    }
    std::cout << "Stimulating neurons..\n";
    for (int i = 0; i < _layers->size(); i++)
    {
        _layers->at(i)->Stimulate();
    }
    std::string deduction = "";
    if(Output()->at(correctDeduction) == VectorUtilities<double>::Max(Output()))
    {
        deduction = "CORRECT";
        _positives++;
    } 
    else
    {
        deduction = "INCORRECT";
        _negatives++;
    }
    std::cout << "Output: " << VectorUtilities<double>::Join(Output(), ',') << std::endl;
    std::cout << "Deduction: " << deduction << std::endl;
    double loss = Loss(correctDeduction);
    double accuracy = (double(_positives) / (double(_positives) + double(_negatives))) * 100.00;
    if(loss < _lowestLoss)
    {
        _lowestLoss = Loss(correctDeduction);
        std::cout << "New lowest loss: " << _lowestLoss << std::endl;
        std::cout << "Accuracy: " << accuracy << "%" << std::endl;
    }
    else
    {
        std::cout << "Unstimulating neurons..\n";
        for (int i = 0; i < _layers->size(); i++)
        {
            _layers->at(i)->Unstimulate();
        }
    }
    // write accuracy to a file.
    std::ofstream ofs;
    ofs.open(_savedDataDirectory + "/Accuracy.txt", std::ios_base::app | std::ios_base::in);
    if (ofs.is_open())
    {
        ofs << accuracy << "\n";
        ofs.close();
    }

    // free memory from batch
    for(int i = 0; i < batch->size(); i++)
    {
       delete batch->at(i);
    }
    delete batch;
}

void NeuralNetwork::SetTrainingDataPaths(std::vector<std::string>* categorizedTrainingDataDirectoryPaths)
{
    //if(_layers->at(_layers->size()-1)->NeuronCount() != categorizedTrainingDataDirectoryPaths->size())
    //    throw new std::invalid_argument("categorizedTrainingDataDirectoryPaths->size() MUST equal neuron count of last layer in neural network.");
    _categorizedTrainingDataDirectoryPaths = categorizedTrainingDataDirectoryPaths;
    std::vector<std::vector<std::string>*>* categorizedTrainingDataFilePaths = new std::vector<std::vector<std::string>*>();
    for(int i = 0; i < _categorizedTrainingDataDirectoryPaths->size(); i++)
    {
        categorizedTrainingDataFilePaths->push_back(VectorUtilities<std::string>::GetAllFileNamesWithinFolder(_categorizedTrainingDataDirectoryPaths->at(i)));
    }
    _categorizedTrainingDataFilePaths = categorizedTrainingDataFilePaths;

    std::vector<std::tuple<std::string, int>>* fileNames = new std::vector<std::tuple<std::string, int>>();
    for(int i = 0; i < _categorizedTrainingDataDirectoryPaths->size(); i++)
    {
        for(int j = 0; j < _categorizedTrainingDataFilePaths->at(i)->size(); j++)
        {
            
            std::string fileName = _categorizedTrainingDataFilePaths->at(i)->at(j);
            fileNames->push_back(std::make_tuple(fileName, i));
        }
    }
    // randomly sort input data
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(*fileNames), std::end(*fileNames), rng);
    _shuffledInputData = fileNames;

    // free memory
    delete _categorizedTrainingDataDirectoryPaths;
    for (int i = 0; i < _categorizedTrainingDataFilePaths->size(); i++)
    {
        delete _categorizedTrainingDataFilePaths->at(i);
    }
    delete _categorizedTrainingDataFilePaths;
}

void NeuralNetwork::SaveNeuralNetworkToFile(std::string fileName)
{
    std::ofstream ofs;
    ofs.open(fileName, std::ofstream::out | std::ofstream::trunc);
    if(ofs.is_open())
    {
        // first section is neural network shape.
        std::vector<int>* layerSizes = new std::vector<int>();
        for(int i = 0; i < _layers->size(); i++)
        {
            layerSizes->push_back(_layers->at(i)->NeuronCount());
        }
        ofs << VectorUtilities<int>::Join(layerSizes, ',');
        ofs << "~"; // delimiter per section
        // next section is weights
        std::vector<std::vector<std::vector<double>*>*>* weights = GetWeights();
        // for each layer
        for(int i = 0; i < weights->size(); i++)
        {
            // for each node
            for(int j = 0; j < weights->at(i)->size(); j++)
            {
                ofs << VectorUtilities<double>::Join(weights->at(i)->at(j), ',');
                ofs << "*"; // delimiter per node.
            }
            // no delimiter at end
            if(i != weights->size() - 1)
                ofs << "|"; // delimiter per layer
        }
        ofs << "~"; // delimiter per section
        // next section is biases
        std::vector<std::vector<double>*>* biases = GetBiases();
        for(int i = 0; i < biases->size(); i++)
        {
            ofs << VectorUtilities<double>::Join(biases->at(i), ','); // , is delimiter per node.
            // no delimiter at end.
            if(i != biases->size() - 1)
                ofs << "|"; // delimiter per vector.
        }
        ofs.close();

        // free memory
        for(int i = 0; i < weights->size(); i++)
        {
            for(int j = 0; j < weights->at(i)->size(); j++)
            {
                delete weights->at(i)->at(j);
            }
            delete weights->at(i);
        }
        delete weights;

        for (int i = 0; i < biases->size(); i++)
        {
            delete biases->at(i);
        }
        delete biases;
    }
}

double NeuralNetwork::Loss(int correctDeduction)
{
    return -log(this->Output()->at(correctDeduction));
} 

std::vector<std::vector<double>*>* NeuralNetwork::GetBiases()
{
    std::vector<std::vector<double>*>* result = new std::vector<std::vector<double>*>();
    for(int i = 0; i < _layers->size(); i++)
    {
        result->push_back(_layers->at(i)->GetBiases());
    }
    return result;
}

std::vector<std::vector<std::vector<double>*>*>* NeuralNetwork::GetWeights()
{
    std::vector<std::vector<std::vector<double>*>*>* result = new std::vector<std::vector<std::vector<double>*>*>();
    for(int i = 0; i < _layers->size(); i++)
    {
        result->push_back(_layers->at(i)->GetWeights());
    }
    return result;
}
