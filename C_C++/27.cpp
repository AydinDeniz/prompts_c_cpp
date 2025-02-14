
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

class MalwareSandbox {
public:
    void analyzeFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            checkForMaliciousPatterns(line);
        }

        file.close();
    }

    void checkForMaliciousPatterns(const std::string& line) {
        std::vector<std::string> suspiciousPatterns = {"system(", "exec(", "fork()", "rm -rf", "wget ", "curl "};
        
        for (const auto& pattern : suspiciousPatterns) {
            if (line.find(pattern) != std::string::npos) {
                std::cout << "Potential malicious code detected: " << pattern << " found in line: " << line << std::endl;
            }
        }
    }
};

int main() {
    MalwareSandbox sandbox;
    sandbox.analyzeFile("suspicious_script.sh");

    return 0;
}
