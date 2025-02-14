#include <iostream>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <random>

class Connection {
public:
    Connection(int id) : id_(id), in_use_(false) {}
    
    int getId() const { return id_; }

    void executeQuery(const std::string& query) {
        std::cout << "Connection " << id_ << " executing query: " << query << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 100));
    }

    bool isInUse() const { return in_use_; }
    void setInUse(bool in_use) { in_use_ = in_use; }

private:
    int id_;
    bool in_use_;
};

class ConnectionPool {
public:
    ConnectionPool(size_t size) {
        for (size_t i = 0; i < size; ++i) {
            connections_.emplace_back(new Connection(i + 1));
            available_connections_.push(connections_.back().get());
        }
    }

    ~ConnectionPool() {
        for (auto& conn : connections_) {
            delete conn;
        }
    }

    Connection* acquire() {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]() { return !available_connections_.empty(); });

        Connection* conn = available_connections_.front();
        available_connections_.pop();
        conn->setInUse(true);

        return conn;
    }

    void release(Connection* conn) {
        std::lock_guard<std::mutex> lock(mtx_);
        conn->setInUse(false);
        available_connections_.push(conn);
        cv_.notify_one();
    }

    size_t availableConnections() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return available_connections_.size();
    }

private:
    std::vector<Connection*> connections_;
    std::queue<Connection*> available_connections_;
    mutable std::mutex mtx_;
    std::condition_variable cv_;
};

void databaseTask(ConnectionPool& pool, int client_id) {
    Connection* conn = pool.acquire();

    conn->executeQuery("SELECT * FROM table WHERE client_id = " + std::to_string(client_id));

    pool.release(conn);
}

int main() {
    const int CLIENT_COUNT = 10;
    ConnectionPool pool(3);

    std::vector<std::thread> clients;
    for (int i = 0; i < CLIENT_COUNT; ++i) {
        clients.emplace_back(std::thread(databaseTask, std::ref(pool), i + 1));
    }

    for (auto& client : clients) {
        client.join();
    }

    return 0;
}