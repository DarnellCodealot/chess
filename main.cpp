#include "libs.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess?", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    Board board;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                board.Click(sf::Mouse::getPosition(window));
            }

            board.SockFish();
        }

        window.clear();
        board.Draw(window);
        window.display();
    }

    return 0;
}