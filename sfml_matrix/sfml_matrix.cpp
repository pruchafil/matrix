#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <random>

int main() {
    std::vector<std::vector<sf::Text>> texts;

    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);

    constexpr float speed = 10.0;

    const uint32_t windowSizeX = sf::VideoMode::getDesktopMode().width;
    const uint32_t windowSizeY = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(windowSizeX, windowSizeY), "Matrix", sf::Style::Fullscreen);

    sf::Font font;
    font.loadFromFile("./Antimatrix-BWBVw.ttf");

    std::mt19937 mt((std::random_device())());
    std::uniform_int_distribution<std::mt19937::result_type> textHeightUid(windowSizeY / 50u, windowSizeY / 10u);
    std::uniform_int_distribution<std::mt19937::result_type> textCharsUid('0', 'z');
    std::uniform_int_distribution<std::mt19937::result_type> textSizeUid(windowSizeX / 150u, windowSizeX / 60u);
    std::uniform_int_distribution<std::mt19937::result_type> textXUid(0u, windowSizeX);

    texts.reserve(windowSizeX / 15);

    for (size_t i = 0ull; i < windowSizeX / 15; ++i) {
        std::vector<sf::Text> innerVec;

        const uint8_t height = textHeightUid(mt);
        const uint32_t size = textSizeUid(mt);
        const uint32_t x = textXUid(mt);

        innerVec.reserve(height);

        for (uint8_t j = 0; j < height; ++j) {
            sf::Color color((255 / height) * j, 255, (255 / height) * j);

            char textChar;
            do {
                textChar = textCharsUid(mt);
            } while (!isalnum(textChar));

            sf::Text text(textChar, font);
            text.setFillColor(color);
            text.setCharacterSize(size);

            text.setPosition(static_cast<float>(x), static_cast<float>(j) * size);

            innerVec.push_back(std::move(text));
        }

        texts.push_back(std::move(innerVec));
    }

    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Time deltaTime = deltaClock.restart();
        sf::Event event;

        window.clear();

        for (auto& vec : texts) {
            for (auto& text : vec) {
                sf::Vector2f pos = text.getPosition();
                text.setPosition(pos.x, pos.y + (text.getCharacterSize() * deltaTime.asSeconds() * speed));
                window.draw(text);
            }

            bool outOfBounds = vec[0].getPosition().y > windowSizeY;

            if (outOfBounds) {
                vec.clear();

                std::vector<sf::Text> innerVec;

                const uint8_t height = textHeightUid(mt);
                const uint32_t size = textSizeUid(mt);
                const uint32_t x = textXUid(mt);

                vec.reserve(height);

                for (uint8_t j = 0; j < height; ++j) {
                    sf::Color color((255 / height) * j, 255, (255 / height) * j);

                    char textChar;
                    do {
                        textChar = textCharsUid(mt);
                    } while (!isalnum(textChar));

                    sf::Text text(textChar, font);
                    text.setFillColor(color);
                    text.setCharacterSize(size);

                    float newY = j * static_cast<float>(size) - static_cast<float>(height) * size;
                    text.setPosition(static_cast<float>(x), newY);

                    vec.push_back(std::move(text));
                }
            }
        }

        window.display();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    return 0;
}