
#include <SFML/Graphics.hpp>
#include <deque>
#include <iostream>
#include <cstdlib>
#include <ctime>

const int WIDTH = 800;
const int HEIGHT = 600;
const int GRID_SIZE = 20;

enum Direction { UP, DOWN, LEFT, RIGHT };

class SnakeGame {
public:
    SnakeGame() : window(sf::VideoMode(WIDTH, HEIGHT), "Snake Game"), direction(RIGHT), score(0) {
        resetGame();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            if (clock.getElapsedTime().asSeconds() > 0.1f) {
                update();
                clock.restart();
            }
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::RectangleShape snakeBlock;
    sf::RectangleShape foodBlock;
    std::deque<sf::Vector2i> snake;
    sf::Vector2i food;
    Direction direction;
    sf::Clock clock;
    int score;

    void resetGame() {
        snake.clear();
        snake.push_back(sf::Vector2i(WIDTH / (2 * GRID_SIZE), HEIGHT / (2 * GRID_SIZE)));
        direction = RIGHT;
        score = 0;
        spawnFood();

        snakeBlock.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        snakeBlock.setFillColor(sf::Color::Green);

        foodBlock.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        foodBlock.setFillColor(sf::Color::Red);
    }

    void spawnFood() {
        int maxX = WIDTH / GRID_SIZE;
        int maxY = HEIGHT / GRID_SIZE;
        food = sf::Vector2i(rand() % maxX, rand() % maxY);
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::W:
                        if (direction != DOWN) direction = UP;
                        break;
                    case sf::Keyboard::S:
                        if (direction != UP) direction = DOWN;
                        break;
                    case sf::Keyboard::A:
                        if (direction != RIGHT) direction = LEFT;
                        break;
                    case sf::Keyboard::D:
                        if (direction != LEFT) direction = RIGHT;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void update() {
        // Move the snake
        sf::Vector2i newHead = snake.front();
        switch (direction) {
            case UP: newHead.y -= 1; break;
            case DOWN: newHead.y += 1; break;
            case LEFT: newHead.x -= 1; break;
            case RIGHT: newHead.x += 1; break;
        }

        // Check for collisions
        if (newHead.x < 0 || newHead.x >= WIDTH / GRID_SIZE ||
            newHead.y < 0 || newHead.y >= HEIGHT / GRID_SIZE || 
            std::find(snake.begin(), snake.end(), newHead) != snake.end()) {
            std::cout << "Game Over! Score: " << score << std::endl;
            resetGame();
            return;
        }

        // Check for food
        if (newHead == food) {
            score++;
            spawnFood();
        } else {
            snake.pop_back(); // Remove last block if no food eaten
        }

        snake.push_front(newHead);
    }

    void render() {
        window.clear();

        // Draw the snake
        for (auto& block : snake) {
            snakeBlock.setPosition(block.x * GRID_SIZE, block.y * GRID_SIZE);
            window.draw(snakeBlock);
        }

        // Draw the food
        foodBlock.setPosition(food.x * GRID_SIZE, food.y * GRID_SIZE);
        window.draw(foodBlock);

        window.display();
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    SnakeGame game;
    game.run();
    return 0;
}
