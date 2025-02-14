
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>

class ExploitDetector {
public:
    void analyzeFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << filename << std::endl;
            return;
        }

        std::string line;
        int lineNumber = 0;
        while (std::getline(file, line)) {
            lineNumber++;
            detectVulnerabilities(line, lineNumber);
        }

        file.close();
    }

    void detectVulnerabilities(const std::string& line, int lineNumber) {
        std::vector<std::pair<std::regex, std::string>> patterns = {
            {std::regex("gets\("), "Critical: Unsafe gets() function detected! Use fgets() instead."},
            {std::regex("strcpy\("), "High: Buffer overflow risk in strcpy(), consider using strncpy()."},
            {std::regex("sprintf\("), "High: Unbounded sprintf() detected, consider snprintf()."},
            {std::regex("system\("), "Medium: Untrusted input passed to system() call, potential command injection."},
            {std::regex("exec\("), "Medium: Potential code execution vulnerability detected with exec()."},
            {std::regex("malloc\("), "Low: Heap memory allocation detected, ensure proper free() usage."}
        };

        for (const auto& pattern : patterns) {
            if (std::regex_search(line, pattern.first)) {
                std::cout << "Line " << lineNumber << ": " << pattern.second << std::endl;
            }
        }
    }
};

int main() {
    ExploitDetector detector;
    detector.analyzeFile("binary_analysis.c");

    return 0;
}
