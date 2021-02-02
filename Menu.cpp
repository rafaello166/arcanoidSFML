#include <iostream>
#include "Menu.h"

using namespace std;

Menu::Menu(float width, float height) {
    string file = "../Fonts/luckiestguy.ttf";
    if (!font.loadFromFile(file)) {
        cout << "Nie znaleziono: " << file << endl;
    }

    menu[0].setFont(font);
    menu[0].setColor(sf::Color::Yellow);
    menu[0].setString("Graj!");
    menu[0].setCharacterSize(90);
    menu[0].setPosition(sf::Vector2f(width / 2 - 5 * 21, height / (MAX_NUMBER_OF_ITEMS + 0.4) * 1));

    menu[1].setFont(font);
    menu[1].setColor(sf::Color::White);
    menu[1].setString("Ranking");
    menu[1].setCharacterSize(50);
    menu[1].setPosition(sf::Vector2f(width / 2 - 5 * 18, height / (MAX_NUMBER_OF_ITEMS + 0.7) * 2));

    menu[2].setFont(font);
    menu[2].setColor(sf::Color::White);
    menu[2].setString("Instrukcja");
    menu[2].setCharacterSize(50);
    menu[2].setPosition(sf::Vector2f(width / 2 - 5 * 25, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));

    menu[3].setFont(font);
    menu[3].setColor(sf::Color::White);
    menu[3].setString(L"Wyjście");
    menu[3].setCharacterSize(50);
    menu[3].setPosition(sf::Vector2f(width / 2 - 5 * 17, height / (MAX_NUMBER_OF_ITEMS + 1.2) * 4));

    selectedItemIndex = 0;
}

void Menu::MoveUp() {
    if (selectedItemIndex - 1 >= 0) {
        menu[selectedItemIndex].setColor(sf::Color::White);
        selectedItemIndex--;
        menu[selectedItemIndex].setColor(sf::Color::Yellow);
    }
}

void Menu::MoveDown() {
    if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS) {
        menu[selectedItemIndex].setColor(sf::Color::White);
        selectedItemIndex++;
        menu[selectedItemIndex].setColor(sf::Color::Yellow);
    }
}

void Menu::draw(sf::RenderWindow &window) {
    for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++) {
        window.draw(menu[i]);
    }
}