#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random> // Include <random> header for random number generation
#include <stdexcept>

// Constants
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int MAX_POINTS = 100000;
const int VERTEX_RADIUS = 5;
const int POINT_SIZE = 1;
const int FONT_SIZE = 20;
const sf::Color VERTEX_COLOR = sf::Color::Blue;
const sf::Color POINT_COLOR = sf::Color::Yellow;
const sf::Color INSTRUCTION_COLOR = sf::Color::White;
const std::string FONT_FILE = "KOMIKAP_.ttf";
const int NUMBER_VERTICES = 3;
const sf::Vector2f STARTING_POSITION(20, 20);
const std::string GAME_NAME = "Chaos Game!!";

class Game {
protected:
    sf::RenderWindow window;
    std::vector<sf::Vector2f> vertices;
    std::vector<sf::Vector2f> points;
    sf::Font font;
    sf::Text instructions;
    std::mt19937 randomEngine; // Mersenne Twister random number generator engine

public:
    Game() : randomEngine(std::random_device()()) { // Initialize randomEngine with a random seed
        try {
            if (!font.loadFromFile(FONT_FILE)) {
                throw std::runtime_error("Failed to load font");
            }
        } catch (const std::exception& e) {
            handleException(e);
        }

        instructions.setFont(font);
        instructions.setString("Click on any three points on the screen to create the vertices for the triangle.");
        instructions.setCharacterSize(FONT_SIZE);
        instructions.setFillColor(INSTRUCTION_COLOR);
        instructions.setPosition(STARTING_POSITION);
    }

    virtual ~Game() {}

    virtual void run() {
        if (!window.isOpen()) {
            handleException(std::runtime_error("Failed to create window"));
            return;
        }
        while (window.isOpen()) {
            handleEvents();
            update();
            draw();
        }
    }

protected:
    virtual void handleEvents() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    void handleException(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        // Optionally, show a message box or log to a file
        window.close();
    }

    void drawVertices() {
        for (const auto &vertex : vertices) {
            sf::CircleShape circle(VERTEX_RADIUS);
            circle.setFillColor(VERTEX_COLOR);
            circle.setPosition(vertex);
            window.draw(circle);
        }
    }

    void drawPoints() {
        for (const auto &point : points) {
            sf::RectangleShape rect(sf::Vector2f(POINT_SIZE, POINT_SIZE));
            rect.setPosition(point);
            rect.setFillColor(POINT_COLOR);
            window.draw(rect);
        }
    }

    void drawMidpoints(int points_to_draw) {
        std::uniform_int_distribution<int> distribution(0, NUMBER_VERTICES - 1);
        for (int i = 0; i < points_to_draw; i++) {
            int vert = distribution(randomEngine); // Generate a random vertex index
            sf::Vector2f newPoint;
            newPoint.x = (vertices[vert].x + points.back().x) / 2;
            newPoint.y = (vertices[vert].y + points.back().y) / 2;
            points.push_back(newPoint);
        }
    }
};

class ChaosGame : public Game {
public:
    ChaosGame() : Game() {
        window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);
        if (!window.isOpen()) {
            handleException(std::runtime_error("Failed to create window"));
        }
    }

    void run() override {
        Game::run();
    }

protected:
    void handleEvents() override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
        }
    }

    void handleMouseClick(float mouseX, float mouseY) {
        if (vertices.size() < NUMBER_VERTICES) {
            vertices.push_back(sf::Vector2f(mouseX, mouseY));
            if (vertices.size() == NUMBER_VERTICES) {
                instructions.setString("Click on a fourth point to start the algorithm.");
            }
        } else if (points.size() < MAX_POINTS) {
            points.push_back(sf::Vector2f(mouseX, mouseY));
        }
    }

    void update() override {
        if (points.size() > 0 && points.size() < MAX_POINTS) {
            instructions.setString("Drawing Midpoints...");
            int points_to_draw = points.size() / 2 + 1; // Use descriptive variable name and remove magic number
            drawMidpoints(points_to_draw);
            std::cout << points.size() << " Total points rendered\n";
        }
        if (points.size() >= MAX_POINTS) {
            instructions.setString("Simulation Complete!");
        }
    }

    void draw() override {
        window.clear();
        drawVertices();
        drawPoints();
        window.draw(instructions);
        window.display();
    }
};

int main() {
    ChaosGame game;
    game.run();
    return 0;
}
