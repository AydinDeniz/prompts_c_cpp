
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

class NeuralNetwork {
public:
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize, double learningRate)
        : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize), learningRate(learningRate) {
        srand(static_cast<unsigned>(time(0)));
        
        // Initialize weights and biases with small random values
        weightsInputHidden = randomMatrix(hiddenSize, inputSize);
        weightsHiddenOutput = randomMatrix(outputSize, hiddenSize);
        biasHidden = randomMatrix(hiddenSize, 1);
        biasOutput = randomMatrix(outputSize, 1);
    }

    // Forward pass
    std::vector<double> predict(const std::vector<double>& input) {
        hiddenLayer = sigmoid(matrixAdd(matrixMultiply(weightsInputHidden, input), biasHidden));
        outputLayer = sigmoid(matrixAdd(matrixMultiply(weightsHiddenOutput, hiddenLayer), biasOutput));
        return outputLayer;
    }

    // Train using backpropagation
    void train(const std::vector<double>& input, const std::vector<double>& target) {
        predict(input);

        // Calculate output layer error and gradient
        std::vector<double> outputError = vectorSubtract(target, outputLayer);
        std::vector<double> outputGradient = hadamardProduct(sigmoidDerivative(outputLayer), outputError);
        outputGradient = scalarMultiply(outputGradient, learningRate);
        
        // Calculate hidden-output weight delta
        std::vector<std::vector<double>> hiddenOutputDelta = outerProduct(outputGradient, hiddenLayer);
        weightsHiddenOutput = matrixAdd(weightsHiddenOutput, hiddenOutputDelta);
        biasOutput = vectorAdd(biasOutput, outputGradient);

        // Calculate hidden layer error and gradient
        std::vector<double> hiddenError = matrixMultiply(transpose(weightsHiddenOutput), outputError);
        std::vector<double> hiddenGradient = hadamardProduct(sigmoidDerivative(hiddenLayer), hiddenError);
        hiddenGradient = scalarMultiply(hiddenGradient, learningRate);

        // Calculate input-hidden weight delta
        std::vector<std::vector<double>> inputHiddenDelta = outerProduct(hiddenGradient, input);
        weightsInputHidden = matrixAdd(weightsInputHidden, inputHiddenDelta);
        biasHidden = vectorAdd(biasHidden, hiddenGradient);
    }

private:
    int inputSize, hiddenSize, outputSize;
    double learningRate;
    std::vector<double> hiddenLayer, outputLayer;
    std::vector<std::vector<double>> weightsInputHidden, weightsHiddenOutput;
    std::vector<double> biasHidden, biasOutput;

    // Activation function and its derivative
    double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }
    double sigmoidDerivative(double x) { return x * (1 - x); }
    
    // Helper functions for vector and matrix operations
    std::vector<double> sigmoid(const std::vector<double>& vec) {
        std::vector<double> result(vec.size());
        for (size_t i = 0; i < vec.size(); ++i) result[i] = sigmoid(vec[i]);
        return result;
    }

    std::vector<double> sigmoidDerivative(const std::vector<double>& vec) {
        std::vector<double> result(vec.size());
        for (size_t i = 0; i < vec.size(); ++i) result[i] = sigmoidDerivative(vec[i]);
        return result;
    }

    std::vector<std::vector<double>> randomMatrix(int rows, int cols) {
        std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols));
        for (auto& row : matrix) for (auto& val : row) val = ((rand() % 100) / 100.0) - 0.5;
        return matrix;
    }

    // Matrix and vector math utilities
    std::vector<double> matrixMultiply(const std::vector<std::vector<double>>& mat, const std::vector<double>& vec) {
        std::vector<double> result(mat.size(), 0.0);
        for (size_t i = 0; i < mat.size(); ++i) for (size_t j = 0; j < vec.size(); ++j) result[i] += mat[i][j] * vec[j];
        return result;
    }

    std::vector<double> vectorAdd(const std::vector<double>& a, const std::vector<double>& b) {
        std::vector<double> result(a.size());
        for (size_t i = 0; i < a.size(); ++i) result[i] = a[i] + b[i];
        return result;
    }

    std::vector<double> vectorSubtract(const std::vector<double>& a, const std::vector<double>& b) {
        std::vector<double> result(a.size());
        for (size_t i = 0; i < a.size(); ++i) result[i] = a[i] - b[i];
        return result;
    }

    std::vector<double> scalarMultiply(const std::vector<double>& vec, double scalar) {
        std::vector<double> result(vec.size());
        for (size_t i = 0; i < vec.size(); ++i) result[i] = vec[i] * scalar;
        return result;
    }

    std::vector<std::vector<double>> outerProduct(const std::vector<double>& a, const std::vector<double>& b) {
        std::vector<std::vector<double>> result(a.size(), std::vector<double>(b.size()));
        for (size_t i = 0; i < a.size(); ++i) for (size_t j = 0; j < b.size(); ++j) result[i][j] = a[i] * b[j];
        return result;
    }

    std::vector<std::vector<double>> matrixAdd(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b) {
        std::vector<std::vector<double>> result(a.size(), std::vector<double>(a[0].size()));
        for (size_t i = 0; i < a.size(); ++i) for (size_t j = 0; j < a[0].size(); ++j) result[i][j] = a[i][j] + b[i][j];
        return result;
    }
};

int main() {
    NeuralNetwork nn(2, 2, 1, 0.5);
    
    std::vector<std::vector<double>> trainingData = {
        {0, 0}, {0},
        {0, 1}, {1},
        {1, 0}, {1},
        {1, 1}, {0}
    };
    
    for (int epoch = 0; epoch < 10000; ++epoch) {
        for (size_t i = 0; i < trainingData.size(); i += 2) {
            nn.train(trainingData[i], trainingData[i + 1]);
        }
    }
    
    std::cout << "Prediction for (0, 0): " << nn.predict({0, 0})[0] << std::endl;
    std::cout << "Prediction for (0, 1): " << nn.predict({0, 1})[0] << std::endl;
    std::cout << "Prediction for (1, 0): " << nn.predict({1, 0})[0] << std::endl;
    std::cout << "Prediction for (1, 1): " << nn.predict({1, 1})[0] << std::endl;

    return 0;
}
