
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

class Debugger {
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
            detectIssues(line, lineNumber);
        }

        file.close();
    }

    void detectIssues(const std::string& line, int lineNumber) {
        std::vector<std::pair<std::regex, std::string>> patterns = {
            {std::regex("scanf\("), "Warning: scanf() detected, consider using safer alternatives like fgets()."},
            {std::regex("gets\("), "Warning: gets() is unsafe, use fgets() instead."},
            {std::regex("strcpy\("), "Warning: strcpy() can cause buffer overflows, consider using strncpy()."},
            {std::regex("= NULL"), "Warning: Null pointer assignment, ensure proper memory allocation before use."}
        };

        for (const auto& pattern : patterns) {
            if (std::regex_search(line, pattern.first)) {
                std::cout << "Line " << lineNumber << ": " << pattern.second << std::endl;
            }
        }
    }
};

int main() {
    Debugger debugger;
    debugger.analyzeFile("sample_code.c");

    return 0;
}
