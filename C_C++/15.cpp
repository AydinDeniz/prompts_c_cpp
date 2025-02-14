
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

class CodeObfuscator {
public:
    static std::string obfuscate(const std::string& code) {
        std::string obfuscated;
        for (char c : code) {
            obfuscated += "\\" + std::to_string((int)c);
        }
        return obfuscated;
    }

    static std::string deobfuscate(const std::string& obfuscated) {
        std::string code;
        size_t pos = 0;
        while (pos < obfuscated.size()) {
            if (obfuscated[pos] == '\\') {
                int ascii = std::stoi(obfuscated.substr(pos + 1, 3));
                code += (char)ascii;
                pos += 4;
            } else {
                pos++;
            }
        }
        return code;
    }
};

int main() {
    std::string sourceCode = "int main() { return 0; }";
    std::string obfuscated = CodeObfuscator::obfuscate(sourceCode);
    std::string deobfuscated = CodeObfuscator::deobfuscate(obfuscated);

    std::cout << "Original Code:\n" << sourceCode << "\n";
    std::cout << "Obfuscated Code:\n" << obfuscated << "\n";
    std::cout << "Deobfuscated Code:\n" << deobfuscated << "\n";

    return 0;
}
