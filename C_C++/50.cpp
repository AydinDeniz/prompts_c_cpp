
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Qubit {
public:
    int bit;
    char basis;

    Qubit(int bit, char basis) : bit(bit), basis(basis) {}
};

class QuantumChannel {
public:
    static std::vector<Qubit> sendQubits(const std::vector<int>& bits, const std::vector<char>& bases) {
        std::vector<Qubit> qubits;
        for (size_t i = 0; i < bits.size(); i++) {
            qubits.emplace_back(bits[i], bases[i]);
        }
        return qubits;
    }

    static std::vector<int> measureQubits(const std::vector<Qubit>& qubits, const std::vector<char>& receiverBases) {
        std::vector<int> measuredBits;
        for (size_t i = 0; i < qubits.size(); i++) {
            if (qubits[i].basis == receiverBases[i]) {
                measuredBits.push_back(qubits[i].bit);
            } else {
                measuredBits.push_back(rand() % 2); // Random measurement if bases mismatch
            }
        }
        return measuredBits;
    }
};

class BB84Protocol {
public:
    static void execute() {
        srand(time(NULL));
        const int keyLength = 16;
        std::vector<int> aliceBits(keyLength);
        std::vector<char> aliceBases(keyLength);
        std::vector<char> bobBases(keyLength);

        for (int i = 0; i < keyLength; i++) {
            aliceBits[i] = rand() % 2;
            aliceBases[i] = (rand() % 2) ? '+' : 'x';
            bobBases[i] = (rand() % 2) ? '+' : 'x';
        }

        std::vector<Qubit> qubits = QuantumChannel::sendQubits(aliceBits, aliceBases);
        std::vector<int> bobMeasurements = QuantumChannel::measureQubits(qubits, bobBases);

        std::vector<int> sharedKey;
        for (int i = 0; i < keyLength; i++) {
            if (aliceBases[i] == bobBases[i]) {
                sharedKey.push_back(bobMeasurements[i]);
            }
        }

        std::cout << "Generated Quantum Key: ";
        for (int bit : sharedKey) {
            std::cout << bit;
        }
        std::cout << "\n";
    }
};

int main() {
    std::cout << "Executing BB84 Quantum Key Distribution Simulation...\n";
    BB84Protocol::execute();
    return 0;
}
