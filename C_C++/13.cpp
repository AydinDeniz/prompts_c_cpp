
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <ctime>

class Journal {
public:
    void logOperation(const std::string& operation) {
        std::ofstream journalFile("journal.log", std::ios::app);
        if (journalFile.is_open()) {
            journalFile << std::time(0) << " " << operation << std::endl;
            journalFile.close();
        }
    }
};

class BTreeIndex {
    std::map<std::string, long> index; // File name to byte offset mapping

public:
    void insert(const std::string& filename, long offset) {
        index[filename] = offset;
    }

    long search(const std::string& filename) {
        return index.count(filename) ? index[filename] : -1;
    }

    void displayIndex() {
        for (const auto& entry : index) {
            std::cout << entry.first << " -> " << entry.second << std::endl;
        }
    }
};

class FileSystem {
    std::fstream storage;
    BTreeIndex index;
    Journal journal;

public:
    FileSystem(const std::string& storageFile) {
        storage.open(storageFile, std::ios::in | std::ios::out | std::ios::app);
        if (!storage) {
            std::cerr << "Failed to open storage file." << std::endl;
            exit(1);
        }
    }

    void createFile(const std::string& filename, const std::string& content) {
        long offset = storage.tellp();
        storage << filename << " " << content << std::endl;
        index.insert(filename, offset);
        journal.logOperation("CREATE " + filename);
    }

    std::string readFile(const std::string& filename) {
        long offset = index.search(filename);
        if (offset == -1) return "File not found.";

        storage.seekg(offset);
        std::string name, content;
        storage >> name;
        std::getline(storage, content);

        return content;
    }

    void displayFileSystem() {
        index.displayIndex();
    }

    ~FileSystem() {
        storage.close();
    }
};

int main() {
    FileSystem fs("storage.dat");

    fs.createFile("file1.txt", "Hello, World!");
    fs.createFile("file2.txt", "Data structures in C++.");
    
    std::cout << "Reading file1.txt: " << fs.readFile("file1.txt") << std::endl;
    std::cout << "Reading file2.txt: " << fs.readFile("file2.txt") << std::endl;

    std::cout << "File System Index: " << std::endl;
    fs.displayFileSystem();

    return 0;
}
