#include"NeuralNetwork.hpp"
#include"FileUtility.h"
NeuralNetwork::NeuralNetwork(std::vector<int>* layerSizes, std::vector<std::string>* categorizedTrainingDataDirectories, std::string outputDirectory)
{
    SetTrainingDataPaths(categorizedTrainingDataDirectories);
    std::vector<Neuron*>* neurons = new std::vector<Neuron*>();
    for (int i = 0; i < layerSizes->at(0); i++)
    {
        Neuron* n = new Neuron(RandomUtility::GenerateRandomVectorOfDoubles(4, -1.0, 1.0));
        neurons->push_back(n);
    }
    Layer* layer = new Layer(neurons);
    layerSizes->erase(layerSizes->begin());
    InitNeuralNetwork(layer, layerSizes);
    _savedDataDirectory = outputDirectory;
}

void NeuralNetwork::InitNeuralNetwork(Layer* firstLayer, std::vector<int>* subsequentLayerSizes)
{
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

void NeuralNetwork::Train(int epochs)
{
    std::ofstream accuracyFile;
    accuracyFile.open(_savedDataDirectory + "/Accuracy.txt", std::ios_base::app);
    for (int epoch = 0; epoch < epochs; epoch++)
    {
        std::cout << "======================= Epoch: " << epoch + 1 << " of " << epochs << " =======================\n";
        // the initial order is already shuffled at construction time (SetTrainingDataPaths);
        // reshuffle before every epoch after the first so each pass sees a different order.
        if (epoch > 0)
            ShuffleInputData();
        _positives = 0;
        _negatives = 0;
        for(int i = 0; i < _shuffledInputData->size(); i++)
        {
            std::cout << "------------------------------------------------------\n";
            std::cout << "Iteration: " << i + 1 << " of " << _shuffledInputData->size() << std::endl;
            std::cout << "Training from image: " << std::get<0>(_shuffledInputData->at(i)) << std::endl;
            Train(FileUtility::FlattenedRGBAVectorFromImage(
                std::get<0>(_shuffledInputData->at(i))),
                std::get<1>(_shuffledInputData->at(i)),
                accuracyFile
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
    accuracyFile.close();
}

void NeuralNetwork::Train(std::vector<double>* flattenedPixels, int correctDeduction, std::ofstream& accuracyFile)
{
    // Feed the whole image to the input layer as one flattened vector, same as every
    // other layer does with the previous layer's output. Each input neuron's weights
    // now persist across images (auto-extended in Neuron::Output() if a larger image
    // is seen), instead of the layer being rebuilt from scratch per image.
    bool imageSkipped = false;
    std::string skipReason;
    std::vector<double>* layerOutput = nullptr;
    try
    {
        _layers->at(0)->SetInputs(flattenedPixels);
        for(int i = 1; i < _layers->size(); i++)
        {
            layerOutput = _layers->at(i-1)->Output();
            _layers->at(i)->SetInputs(layerOutput);
            delete layerOutput;
            layerOutput = nullptr;
        }
    }
    catch (const std::invalid_argument& e)
    {
        // A layer went dead (every neuron's pre-activation was <= 0, so its ReLU
        // output summed to 0 and VectorUtilities::Normalize refused to divide by it).
        imageSkipped = true;
        skipReason = e.what();
        delete layerOutput; // no-op if null; frees the in-flight layer output if SetInputs threw on it
    }
    delete flattenedPixels;
    if (imageSkipped)
    {
        std::cout << "Skipping image: " << skipReason << std::endl;
        return;
    }
    // loss for this image under the weights carried over from the previous image,
    // i.e. before this step's random perturbation.
    double lossBeforeStimulation = Loss(correctDeduction);
    std::cout << "Stimulating neurons..\n";
    for (int i = 0; i < _layers->size(); i++)
    {
        _layers->at(i)->Stimulate();
    }
    std::string deduction = "";
    std::vector<double>* output = Output();
    if(output->at(correctDeduction) == VectorUtilities<double>::Max(output))
    {
        deduction = "CORRECT";
        _positives++;
    }
    else
    {
        deduction = "INCORRECT";
        _negatives++;
    }
    std::cout << "Output: " << VectorUtilities<double>::Join(output, ',') << std::endl;
    delete output;
    std::cout << "Deduction: " << deduction << std::endl;
    double lossAfterStimulation = Loss(correctDeduction);
    double accuracy = (double(_positives) / (double(_positives) + double(_negatives))) * 100.00;
    if(lossAfterStimulation < lossBeforeStimulation)
    {
        std::cout << "Loss improved: " << lossBeforeStimulation << " -> " << lossAfterStimulation << std::endl;
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
    // write accuracy to the shared, already-open file.
    if (accuracyFile.is_open())
    {
        accuracyFile << accuracy << "\n";
    }
}

void NeuralNetwork::SetTrainingDataPaths(std::vector<std::string>* categorizedTrainingDataDirectoryPaths)
{
    //if(_layers->at(_layers->size()-1)->NeuronCount() != categorizedTrainingDataDirectoryPaths->size())
    //    throw new std::invalid_argument("categorizedTrainingDataDirectoryPaths->size() MUST equal neuron count of last layer in neural network.");
    _categorizedTrainingDataDirectoryPaths = categorizedTrainingDataDirectoryPaths;
    std::vector<std::vector<std::string>*>* categorizedTrainingDataFilePaths = new std::vector<std::vector<std::string>*>();
    for(int i = 0; i < _categorizedTrainingDataDirectoryPaths->size(); i++)
    {
        categorizedTrainingDataFilePaths->push_back(FileUtility::GetAllFileNamesWithinFolder(_categorizedTrainingDataDirectoryPaths->at(i)));
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
    _shuffledInputData = fileNames;
    ShuffleInputData();

    // free memory
    delete _categorizedTrainingDataDirectoryPaths;
    for (int i = 0; i < _categorizedTrainingDataFilePaths->size(); i++)
    {
        delete _categorizedTrainingDataFilePaths->at(i);
    }
    delete _categorizedTrainingDataFilePaths;
}

void NeuralNetwork::ShuffleInputData()
{
    static std::default_random_engine rng(unsigned(time(nullptr)));
    std::shuffle(std::begin(*_shuffledInputData), std::end(*_shuffledInputData), rng);
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
        delete layerSizes;
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

        // free memory. Note: weights->at(i)->at(j) is Neuron::_weights itself
        // (Neuron::GetWeights returns the live pointer, not a copy), so only the
        // containers GetWeights/Layer::GetWeights allocated are deleted here.
        for(int i = 0; i < weights->size(); i++)
        {
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
