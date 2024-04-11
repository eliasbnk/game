#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random> 
#include <ctime>
#include <stdexcept>

using namespace sf;
using namespace std;

class ChaosGame {
private:
    RenderWindow Window;
    vector<Vector2f> Vertices;
    vector<Vector2f> Points;
    Font Font;
    Text Instructions;

    const Vector2u WINDOW_SIZE = {1920, 1080};
    const std::string WINDOW_TITLE = "Chaos Game!!";
    const std::string FONT_FILE = "KOMIKAP_.ttf";
    const unsigned int FONT_SIZE = 20;
    const Color TEXT_COLOR = Color::White;
    const Vector2f TEXT_POSITION = {20, 20};
    const int MAX_VERTICES = 3;
    const int MAX_POINTS = 100000;
    const float CIRCLE_RADIUS = 5;
    const Color CIRCLE_COLOR = Color::Blue;
    const Color RECT_COLOR = Color::Yellow;

public:
    ChaosGame() : Window(VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), WINDOW_TITLE) {
        // Load font with error handling
        if (!Font.loadFromFile(FONT_FILE)) {
            throw runtime_error("Failed to load font file: " + FONT_FILE);
        }
        Instructions.setFont(Font);
        Instructions.setString("Click on any three points on the screen to create the vertices for the triangle.");
        Instructions.setCharacterSize(FONT_SIZE);
        Instructions.setFillColor(TEXT_COLOR);
        Instructions.setPosition(TEXT_POSITION);
    }

    void Run() {
        srand(time(0));

        while (Window.isOpen()) {
            HandleEvents();
            Update();
            Draw();
        }
    }

private:
    void HandleEvents() {
        Event Event;
        while (Window.pollEvent(Event)) {
            if (Event.type == Event::Closed)
                Window.close();
            if (Event.type == Event::MouseButtonPressed && Event.mouseButton.button == Mouse::Left)
                HandleMouseClick(Event.mouseButton.x, Event.mouseButton.y);
        }
    }

    void HandleMouseClick(float MouseX, float MouseY) {
        if (Vertices.size() < MAX_VERTICES) {
            Vertices.push_back(Vector2f(MouseX, MouseY));
            if (Vertices.size() == MAX_VERTICES) {
                Instructions.setString("Click on a fourth point to start the algorithm.");
            }
        } else if (Points.empty()) {
            Points.push_back(Vector2f(MouseX, MouseY));
        }
    }

    void Update() {
        if (Points.size() > 0 && Points.size() < MAX_POINTS) {
            Instructions.setString("Drawing Midpoints...");
            // Use a more modern random number generator
            random_device Rd;
            mt19937 Gen(Rd());
            uniform_int_distribution<> Distrib(0, 2);

            int PointsToDraw = Points.size() / 1 + 1;
            for (int i = 0; i < PointsToDraw; i++) {
                int Vert = Distrib(Gen);

                Vector2f NewPoint;
                NewPoint.x = (Vertices[Vert].x + Points.back().x) / 2;
                NewPoint.y = (Vertices[Vert].y + Points.back().y) / 2;
                Points.push_back(NewPoint);
            }
            cout << Points.size() << " Total points rendered\n";
        }

        if (Points.size() > MAX_POINTS) {
            Instructions.setString("Simulation Complete!");
        }
    }

    void Draw() {
        Window.clear();

        for (const auto& Vertex : Vertices) {
            CircleShape Circle(CIRCLE_RADIUS);
            Circle.setFillColor(CIRCLE_COLOR);
            Circle.setPosition(Vertex);
            Window.draw(Circle);
        }

        for (const auto& Point : Points) {
            RectangleShape Rect(Vector2f(1, 1));
            Rect.setPosition(Point);
            Rect.setFillColor(RECT_COLOR);
            Window.draw(Rect);
        }

        Window.draw(Instructions);

        Window.display();
    }
};

int main() {
    try {
        ChaosGame Game;
        Game.Run();
    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1; // indicating failure
    }
    return 0;
}
