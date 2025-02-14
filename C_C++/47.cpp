
#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <ctime>

struct Task {
    int id;
    int priority;
    std::string description;

    bool operator<(const Task& other) const {
        return priority < other.priority;  // Higher priority tasks are executed first
    }
};

class TaskScheduler {
private:
    std::priority_queue<Task> taskQueue;
    std::vector<std::thread> workerThreads;
    std::mutex queueMutex;
    std::condition_variable taskCondition;
    bool stopProcessing = false;

public:
    TaskScheduler(int numWorkers) {
        for (int i = 0; i < numWorkers; i++) {
            workerThreads.emplace_back(&TaskScheduler::workerThread, this, i);
        }
    }

    ~TaskScheduler() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stopProcessing = true;
        }
        taskCondition.notify_all();

        for (auto& thread : workerThreads) {
            thread.join();
        }
    }

    void addTask(int id, int priority, const std::string& description) {
        std::unique_lock<std::mutex> lock(queueMutex);
        taskQueue.push({id, priority, description});
        taskCondition.notify_one();
    }

    void workerThread(int workerId) {
        while (true) {
            Task task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                taskCondition.wait(lock, [this]() { return !taskQueue.empty() || stopProcessing; });

                if (stopProcessing && taskQueue.empty()) return;

                task = taskQueue.top();
                taskQueue.pop();
            }

            std::cout << "[Worker " << workerId << "] Executing Task " << task.id << ": " << task.description << " (Priority: " << task.priority << ")\n";
            std::this_thread::sleep_for(std::chrono::seconds(rand() % 3 + 1));
        }
    }

    void simulateFailureAndRecovery() {
        std::cout << "[INFO] Simulating worker failure...\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "[INFO] Restarting failed worker...\n";
        workerThreads.emplace_back(&TaskScheduler::workerThread, this, workerThreads.size());
    }
};

int main() {
    srand(time(NULL));
    TaskScheduler scheduler(3);

    scheduler.addTask(1, 3, "Process user request");
    scheduler.addTask(2, 1, "Database backup");
    scheduler.addTask(3, 5, "Real-time analytics processing");
    scheduler.addTask(4, 2, "System log cleanup");

    std::this_thread::sleep_for(std::chrono::seconds(5));
    scheduler.simulateFailureAndRecovery();

    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}
