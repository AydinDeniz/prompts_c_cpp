
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

class SimpleDB {
public:
    SimpleDB(const std::string& filename) : filename(filename) {}

    // Function to create or overwrite a new table file
    void createTable(const std::vector<std::string>& columns) {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Failed to create table file." << std::endl;
            return;
        }
        for (const auto& col : columns) {
            file << col << ",";
        }
        file << "\n";
        file.close();
    }

    // Function to add a record to the table
    void addRecord(const std::vector<std::string>& values) {
        std::ofstream file(filename, std::ios::app);
        if (!file) {
            std::cerr << "Failed to open table file." << std::endl;
            return;
        }
        for (const auto& val : values) {
            file << val << ",";
        }
        file << "\n";
        file.close();
    }

    // Function to select records based on a condition
    void selectRecords(const std::string& column, const std::string& value) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Failed to open table file." << std::endl;
            return;
        }

        std::string line;
        std::getline(file, line); // Read header
        std::vector<std::string> columns = split(line, ',');

        auto it = std::find(columns.begin(), columns.end(), column);
        if (it == columns.end()) {
            std::cerr << "Column not found." << std::endl;
            return;
        }
        int colIndex = std::distance(columns.begin(), it);

        std::cout << "Selected records:" << std::endl;
        while (std::getline(file, line)) {
            std::vector<std::string> record = split(line, ',');
            if (record[colIndex] == value) {
                for (const auto& field : record) {
                    std::cout << field << " ";
                }
                std::cout << std::endl;
            }
        }
        file.close();
    }

private:
    std::string filename;

    // Utility function to split a line by delimiter
    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
};

int main() {
    SimpleDB db("database.csv");

    // Define columns and create table
    std::vector<std::string> columns = {"ID", "Name", "Age", "City"};
    db.createTable(columns);

    // Add some records
    db.addRecord({"1", "Alice", "30", "New York"});
    db.addRecord({"2", "Bob", "25", "Los Angeles"});
    db.addRecord({"3", "Charlie", "35", "Chicago"});
    db.addRecord({"4", "Diana", "28", "New York"});

    // Select records with City = "New York"
    db.selectRecords("City", "New York");

    return 0;
}
