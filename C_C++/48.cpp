
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

class NeuralNetwork {
private:
    int inputNodes, hiddenNodes, outputNodes;
    std::vector<std::vector<double>> weightsInputHidden;
    std::vector<std::vector<double>> weightsHiddenOutput;
    std::vector<double> hiddenLayer;
    std::vector<double> outputLayer;

    double sigmoid(double x) {
        return 1 / (1 + exp(-x));
    }

    double sigmoidDerivative(double x) {
        return x * (1 - x);
    }

    void initializeWeights(std::vector<std::vector<double>>& weights, int rows, int cols) {
        weights.resize(rows, std::vector<double>(cols));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                weights[i][j] = (double)rand() / RAND_MAX - 0.5;  // Random values between -0.5 and 0.5
            }
        }
    }

public:
    NeuralNetwork(int inputNodes, int hiddenNodes, int outputNodes)
        : inputNodes(inputNodes), hiddenNodes(hiddenNodes), outputNodes(outputNodes) {
        srand(time(NULL));
        initializeWeights(weightsInputHidden, inputNodes, hiddenNodes);
        initializeWeights(weightsHiddenOutput, hiddenNodes, outputNodes);
        hiddenLayer.resize(hiddenNodes);
        outputLayer.resize(outputNodes);
    }

    std::vector<double> forwardPass(const std::vector<double>& inputs) {
        for (int i = 0; i < hiddenNodes; ++i) {
            hiddenLayer[i] = 0;
            for (int j = 0; j < inputNodes; ++j) {
                hiddenLayer[i] += inputs[j] * weightsInputHidden[j][i];
            }
            hiddenLayer[i] = sigmoid(hiddenLayer[i]);
        }

        for (int i = 0; i < outputNodes; ++i) {
            outputLayer[i] = 0;
            for (int j = 0; j < hiddenNodes; ++j) {
                outputLayer[i] += hiddenLayer[j] * weightsHiddenOutput[j][i];
            }
            outputLayer[i] = sigmoid(outputLayer[i]);
        }
        return outputLayer;
    }

    void train(const std::vector<double>& inputs, const std::vector<double>& targets, double learningRate) {
        std::vector<double> outputs = forwardPass(inputs);

        std::vector<double> outputErrors(outputNodes);
        std::vector<double> hiddenErrors(hiddenNodes);

        for (int i = 0; i < outputNodes; ++i) {
            outputErrors[i] = targets[i] - outputs[i];
        }

        for (int i = 0; i < hiddenNodes; ++i) {
            hiddenErrors[i] = 0;
            for (int j = 0; j < outputNodes; ++j) {
                hiddenErrors[i] += outputErrors[j] * weightsHiddenOutput[i][j];
            }
        }

        for (int i = 0; i < outputNodes; ++i) {
            for (int j = 0; j < hiddenNodes; ++j) {
                weightsHiddenOutput[j][i] += learningRate * outputErrors[i] * sigmoidDerivative(outputs[i]) * hiddenLayer[j];
            }
        }

        for (int i = 0; i < hiddenNodes; ++i) {
            for (int j = 0; j < inputNodes; ++j) {
                weightsInputHidden[j][i] += learningRate * hiddenErrors[i] * sigmoidDerivative(hiddenLayer[i]) * inputs[j];
            }
        }
    }

    void printWeights() {
        std::cout << "Hidden Layer Weights:\n";
        for (const auto& row : weightsInputHidden) {
            for (double w : row) {
                std::cout << w << " ";
            }
            std::cout << "\n";
        }
        std::cout << "Output Layer Weights:\n";
        for (const auto& row : weightsHiddenOutput) {
            for (double w : row) {
                std::cout << w << " ";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    NeuralNetwork nn(3, 3, 1);

    std::vector<std::vector<double>> trainingData = {
        {0, 0, 1}, {1, 1, 1}, {1, 0, 1}, {0, 1, 1}
    };
    std::vector<std::vector<double>> targets = {
        {0}, {1}, {1}, {0}
    };

    for (int i = 0; i < 10000; ++i) {
        for (size_t j = 0; j < trainingData.size(); ++j) {
            nn.train(trainingData[j], targets[j], 0.1);
        }
    }

    nn.printWeights();

    std::cout << "Testing Neural Network:\n";
    for (const auto& data : trainingData) {
        std::vector<double> result = nn.forwardPass(data);
        std::cout << "Input: [" << data[0] << ", " << data[1] << ", " << data[2] << "] Output: " << result[0] << "\n";
    }

    return 0;
}
