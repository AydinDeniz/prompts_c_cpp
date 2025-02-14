
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Player {
public:
    int skillLevel;
    int winCount;
    int lossCount;

    Player(int skill) : skillLevel(skill), winCount(0), lossCount(0) {}

    void recordWin() { winCount++; }
    void recordLoss() { lossCount++; }

    void adjustDifficulty() {
        if (winCount > lossCount) {
            skillLevel += 1;
        } else if (lossCount > winCount) {
            skillLevel -= 1;
        }
    }

    void displayStatus() {
        std::cout << "Player Skill: " << skillLevel << " | Wins: " << winCount << " | Losses: " << lossCount << std::endl;
    }
};

class GameBalancer {
    std::vector<Player> players;

public:
    GameBalancer(int numPlayers) {
        srand(time(NULL));
        for (int i = 0; i < numPlayers; i++) {
            players.emplace_back(rand() % 10 + 1);
        }
    }

    void simulateMatch() {
        int p1 = rand() % players.size();
        int p2 = rand() % players.size();

        if (p1 == p2) return;

        if (players[p1].skillLevel > players[p2].skillLevel) {
            players[p1].recordWin();
            players[p2].recordLoss();
        } else {
            players[p1].recordLoss();
            players[p2].recordWin();
        }

        players[p1].adjustDifficulty();
        players[p2].adjustDifficulty();
    }

    void displayPlayers() {
        for (auto &player : players) {
            player.displayStatus();
        }
    }
};

int main() {
    GameBalancer balancer(5);

    for (int i = 0; i < 10; i++) {
        balancer.simulateMatch();
    }

    balancer.displayPlayers();
    return 0;
}
