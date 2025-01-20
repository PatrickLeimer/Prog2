#include <iostream>
#include <SFML/Graphics.hpp>
#include "Methods.h"
using namespace sf;


int main() {
    string name;

    if(not welcomeScreen(name)) {
        return 0;
    }

    gameWindow(name);


    // RenderWindow welcome_window(VideoMode(800, 600), "MineSweeper", Style::Close); //Windows

    // CircleShape shape(200.f);
    // shape.setFillColor(Color::Blue);
    //
    // while(welcome_window.isOpen()) {
    //     Event event{};
    //     while(welcome_window.pollEvent(event)) {
    //         if(event.type == Event::Closed) {
    //             welcome_window.close();
    //         }
    //     }
    //
    //     welcome_window.clear();
    //     welcome_window.draw(shape);
    //     welcome_window.display();
    // }
    return 0;
}

