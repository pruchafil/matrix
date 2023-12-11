#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <random>

std::vector<std::vector<sf::Text>> texts;

int main() {
    ::ShowWindow(::GetConsoleWindow(), SW_SHOW);

    sf::RenderWindow window(sf::VideoMode(), "SFML works!", sf::Style::Fullscreen);

    sf::Font font;
    font.loadFromFile("./Antimatrix-BWBVw.ttf");

    std::mt19937 mt((std::random_device())());
    std::uniform_int_distribution< std::mt19937::result_type> textHeight(30, 100);
    std::uniform_int_distribution< std::mt19937::result_type> textChars('0', 'z');
    std::uniform_int_distribution< std::mt19937::result_type> textSize(15, 25);
    std::uniform_int_distribution< std::mt19937::result_type> width(0, window.getSize().x);

    for (size_t i = 0; i < window.getSize().x / 15; i++) {
        std::vector<sf::Text> innerVec;

        uint32_t height = textHeight(mt);
        uint32_t size = textSize(mt);
        uint32_t x = width(mt);

        for (size_t j = 0; j < height; j++) {
            sf::Color color((255 / height) * j, 255, (255 / height) * j);

            char c = textChars(mt);
            std::string str; str += c;
            sf::Text text(str, font);
            text.setFillColor(color);
            text.setCharacterSize(size);
            text.setPosition(x, j * size);
            innerVec.push_back(text);
        }

        texts.push_back(innerVec);
    }

    while (window.isOpen()) {
        sf::Event event;

        for (size_t i = 0ull; i < texts.size(); ++i) {
            bool outOfBounds = texts[i][0].getPosition().y > window.getSize().y;

            if (outOfBounds) {
                texts[i].clear();

                std::vector<sf::Text> innerVec;

                uint32_t height = textHeight(mt);
                uint32_t size = textSize(mt);
                uint32_t x = width(mt);

                for (size_t j = 0; j < height; j++) {
                    sf::Color color((255 / height) * j, 255, (255 / height) * j);

                    char c = textChars(mt);
                    std::string str; str += c;
                    sf::Text text(str, font);
                    text.setFillColor(color);
                    text.setCharacterSize(size);
                    text.setPosition(x, static_cast<int32_t>(j) * static_cast<int32_t>(size) - static_cast<int32_t>(height) * static_cast<int32_t>(size));
                    innerVec.push_back(text);
                }

                texts[i] = std::move(innerVec);
            }
        }

        for (auto& vec : texts) {
            for (auto& text : vec) {
                sf::Vector2f pos = text.getPosition();
                text.setPosition(pos.x, pos.y + text.getCharacterSize());
            }
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (const auto& vec : texts) {
            for (const auto& text : vec) {
                window.draw(text);
            }
        }

        window.display();
    }

    return 0;
}