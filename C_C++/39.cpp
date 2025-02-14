
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <thread>

class DatabaseNode {
private:
    std::unordered_map<std::string, std::string> data;
    std::mutex nodeMutex;

public:
    void insert(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(nodeMutex);
        data[key] = value;
        std::cout << "Inserted [" << key << " : " << value << "] in node" << std::endl;
    }

    std::string get(const std::string& key) {
        std::lock_guard<std::mutex> lock(nodeMutex);
        if (data.find(key) != data.end()) {
            return data[key];
        }
        return "Key not found";
    }
};

class DistributedDatabase {
private:
    std::vector<DatabaseNode> nodes;
    int numNodes;

    int getShardIndex(const std::string& key) {
        int hashValue = 0;
        for (char c : key) {
            hashValue += c;
        }
        return hashValue % numNodes;
    }

public:
    DistributedDatabase(int n) : numNodes(n) {
        nodes.resize(n);
    }

    void insert(const std::string& key, const std::string& value) {
        int nodeIndex = getShardIndex(key);
        nodes[nodeIndex].insert(key, value);
    }

    std::string get(const std::string& key) {
        int nodeIndex = getShardIndex(key);
        return nodes[nodeIndex].get(key);
    }

    void simulateLoadBalancing(int operations) {
        srand(time(NULL));
        for (int i = 0; i < operations; i++) {
            int randomNode = rand() % numNodes;
            std::string key = "key" + std::to_string(rand() % 100);
            std::string value = "value" + std::to_string(rand() % 1000);

            nodes[randomNode].insert(key, value);
        }
        std::cout << "Load balancing simulation complete." << std::endl;
    }
};

void performDatabaseOperations(DistributedDatabase& db) {
    db.insert("user1", "Alice");
    db.insert("user2", "Bob");
    db.insert("user3", "Charlie");

    std::cout << "Fetching user1: " << db.get("user1") << std::endl;
    std::cout << "Fetching user3: " << db.get("user3") << std::endl;
}

int main() {
    DistributedDatabase db(5);

    std::thread t1(performDatabaseOperations, std::ref(db));
    std::thread t2(&DistributedDatabase::simulateLoadBalancing, &db, 10);

    t1.join();
    t2.join();

    return 0;
}
