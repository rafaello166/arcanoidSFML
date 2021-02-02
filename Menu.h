#ifndef SFMLTEST_MENU_H
#define SFMLTEST_MENU_H

#include <SFML/Graphics.hpp>

#define MAX_NUMBER_OF_ITEMS 4

class Menu {
    public:
        Menu(float width, float height);
        ~Menu() = default;

        void draw(sf::RenderWindow &window);
        void MoveUp();
        void MoveDown();

        int GetPressedItem() { return selectedItemIndex; }
    private:
        int selectedItemIndex;
        sf::Font font;
        sf::Text menu[MAX_NUMBER_OF_ITEMS];
};


#endif SFMLTEST_MENU_H
