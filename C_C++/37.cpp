
#include <iostream>
#include <vector>
#include <string>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

class Identity {
public:
    std::string name;
    std::string publicKey;

    Identity(std::string name, std::string publicKey) : name(name), publicKey(publicKey) {}

    void display() {
        std::cout << "Identity: " << name << "\nPublic Key: " << publicKey << "\n";
    }
};

class IdentityBlockchain {
    std::vector<std::string> blocks;

public:
    void registerIdentity(const Identity &id) {
        std::string data = id.name + id.publicKey;
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)data.c_str(), data.size(), hash);

        char hashString[SHA256_DIGEST_LENGTH * 2 + 1];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(&hashString[i * 2], "%02x", (unsigned int)hash[i]);
        }

        blocks.push_back(std::string(hashString));
        std::cout << "Identity Registered: " << id.name << " - Hash: " << hashString << "\n";
    }
};

int main() {
    IdentityBlockchain blockchain;

    Identity user1("Alice", "AlicePublicKey");
    Identity user2("Bob", "BobPublicKey");

    blockchain.registerIdentity(user1);
    blockchain.registerIdentity(user2);

    return 0;
}
