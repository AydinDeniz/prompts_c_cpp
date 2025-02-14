
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <openssl/sha.h>

class Block {
public:
    std::string hash;
    std::string previousHash;
    std::string data;

    Block(std::string prevHash, std::string fileData) : previousHash(prevHash), data(fileData) {
        hash = calculateHash();
    }

    std::string calculateHash() {
        std::string input = previousHash + data;
        unsigned char digest[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)input.c_str(), input.size(), digest);

        char mdString[SHA256_DIGEST_LENGTH * 2 + 1];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
            sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);
        
        return std::string(mdString);
    }
};

class DecentralizedStorage {
    std::vector<Block> blockchain;
    std::unordered_map<std::string, std::string> fileStorage;

public:
    DecentralizedStorage() {
        blockchain.push_back(Block("0", "Genesis Block"));
    }

    void storeFile(const std::string& filename, const std::string& content) {
        std::string prevHash = blockchain.back().hash;
        Block newBlock(prevHash, content);
        blockchain.push_back(newBlock);
        fileStorage[newBlock.hash] = content;

        std::cout << "Stored file '" << filename << "' with hash: " << newBlock.hash << std::endl;
    }

    void retrieveFile(const std::string& hash) {
        if (fileStorage.find(hash) != fileStorage.end()) {
            std::cout << "Retrieved File: " << fileStorage[hash] << std::endl;
        } else {
            std::cout << "File not found!" << std::endl;
        }
    }
};

int main() {
    DecentralizedStorage storage;

    storage.storeFile("file1.txt", "This is secure file content.");
    storage.storeFile("file2.txt", "Another blockchain-based file.");

    storage.retrieveFile(storage.blockchain[1].hash);

    return 0;
}
