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
        std::vector<double>* previousLayerOutput = layers->at(i)->Output();
        for(int j = 0; j < numberOfNeuronsInLayer; j++)
        {
            Neuron* n = new Neuron(previousLayerOutput);
            if(i == subsequentLayerSizes->size() - 1)
                n->IsInFinalLayer(true);
            neurons->push_back(n);
        }
        delete previousLayerOutput;
        Layer* layer = new Layer(neurons);
        layers->push_back(layer);
    }
    _layers = layers;
}

std::vector<double>* NeuralNetwork::Output()
{
    // Final layer neurons return raw logits; softmax is a layer-wide function so it's
    // computed here, with max-subtraction for numerical stability.
    std::vector<double>* logits = _layers->at(_layers->size() - 1)->Output();
    double maxLogit = VectorUtilities<double>::Max(logits);
    std::vector<double>* softmaxOutput = new std::vector<double>();
    double sum = 0.00;
    for (int i = 0; i < logits->size(); i++)
    {
        double e = exp(logits->at(i) - maxLogit);
        softmaxOutput->push_back(e);
        sum += e;
    }
    for (int i = 0; i < softmaxOutput->size(); i++)
        softmaxOutput->at(i) /= sum;
    delete logits;
    return softmaxOutput;
}

void NeuralNetwork::Train(int epochs, double learningRate)
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
                std::get<0>(_shuffledInputData->at(i)), 64, 64),
                std::get<1>(_shuffledInputData->at(i)),
                accuracyFile,
                learningRate
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

void NeuralNetwork::Train(std::vector<double>* flattenedPixels, int correctDeduction, std::ofstream& accuracyFile, double learningRate)
{
    // Scale raw 0-255 pixel bytes down to [0,1]. Without this, the input layer's
    // weighted sums (weights in [-1,1] times thousands of ~0-255 inputs) would be huge,
    // making gradients unusable.
    for (int i = 0; i < flattenedPixels->size(); i++)
        flattenedPixels->at(i) /= 255.00;

    // Feed the whole image to the input layer as one flattened vector, same as every
    // other layer does with the previous layer's output. Each input neuron's weights
    // persist across images (auto-extended in Neuron::Output() if a larger image is
    // seen), instead of the layer being rebuilt from scratch per image.
    _layers->at(0)->SetInputs(flattenedPixels);
    delete flattenedPixels;
    for(int i = 1; i < _layers->size(); i++)
    {
        std::vector<double>* layerOutput = _layers->at(i-1)->Output();
        _layers->at(i)->SetInputs(layerOutput);
        delete layerOutput;
    }

    std::string deduction = "";
    std::vector<double>* output = Output();
    double loss = Loss(output, correctDeduction);
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
    std::cout << "Deduction: " << deduction << std::endl;
    std::cout << "Loss: " << loss << std::endl;

    // Combined softmax + cross-entropy gradient w.r.t. the final layer's logits:
    // dLoss/dz_i = softmax_i - y_i, where y is the one-hot correct-class vector.
    std::vector<double>* dLoss_dActivations = new std::vector<double>();
    for (int i = 0; i < output->size(); i++)
        dLoss_dActivations->push_back(output->at(i) - (i == correctDeduction ? 1.00 : 0.00));
    delete output;

    std::cout << "Backpropagating..\n";
    for (int i = (int)_layers->size() - 1; i >= 0; i--)
    {
        std::vector<double>* previousLayerGradient = _layers->at(i)->Backward(dLoss_dActivations, learningRate);
        delete dLoss_dActivations;
        dLoss_dActivations = previousLayerGradient;
    }
    delete dLoss_dActivations; // gradient w.r.t. raw pixel input; nothing upstream to propagate to.

    double accuracy = (double(_positives) / (double(_positives) + double(_negatives))) * 100.00;
    std::cout << "Accuracy: " << accuracy << "%" << std::endl;
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

double NeuralNetwork::Loss(std::vector<double>* output, int correctDeduction)
{
    return -log(output->at(correctDeduction));
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
