
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

using namespace std;

typedef complex<double> qbit;
const double SQRT2_INV = 1.0 / sqrt(2.0);

class QuantumCircuit {
public:
    QuantumCircuit(int qubits) : numQubits(qubits) {
        state.resize(1 << numQubits, qbit(0));
        state[0] = qbit(1);
    }

    void hadamard(int target) {
        vector<qbit> newState(state.size());
        for (size_t i = 0; i < state.size(); ++i) {
            size_t bit = (i >> target) & 1;
            newState[i] += SQRT2_INV * state[i];
            newState[i ^ (1 << target)] += (bit ? -SQRT2_INV : SQRT2_INV) * state[i];
        }
        state = newState;
    }

    void cnot(int control, int target) {
        vector<qbit> newState(state.size());
        for (size_t i = 0; i < state.size(); ++i) {
            if ((i >> control) & 1) {
                size_t newIndex = i ^ (1 << target);
                newState[newIndex] = state[i];
            } else {
                newState[i] = state[i];
            }
        }
        state = newState;
    }

    void measure() {
        double sum = 0.0;
        for (const auto& amp : state) sum += norm(amp);
        double randVal = ((double)rand() / RAND_MAX) * sum;

        double cumulative = 0.0;
        for (size_t i = 0; i < state.size(); ++i) {
            cumulative += norm(state[i]);
            if (randVal <= cumulative) {
                cout << "Measured state: |" << i << ">" << endl;
                break;
            }
        }
    }

    void printState() {
        cout << "Quantum State: " << endl;
        for (size_t i = 0; i < state.size(); ++i) {
            if (abs(state[i]) > 1e-6) {
                cout << "| " << i << " >: " << state[i] << endl;
            }
        }
    }

private:
    int numQubits;
    vector<qbit> state;
};

int main() {
    QuantumCircuit qc(2);

    qc.hadamard(0);
    qc.cnot(0, 1);

    qc.printState();
    qc.measure();

    return 0;
}
