#ifndef SFMLTEST_BLOCK_H
#define SFMLTEST_BLOCK_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Block : public sf::Drawable {
    public:
        Block(float t_X, float t_Y, float t_Width, float t_Height);
        Block() = delete;
        ~Block() = default;

        Vector2f getPosition();

        float left();
        float right();
        float top();
        float bottom();

        bool isDestroyed();
        void destroy();
        Vector2f getSize();
    private:
        virtual void draw(sf::RenderTarget& targer, sf::RenderStates states) const override;
        RectangleShape shape;
        bool destroyed {false};
};


#endif SFMLTEST_BLOCK_H
