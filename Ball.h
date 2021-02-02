#ifndef SFMLTEST_BALL_H
#define SFMLTEST_BALL_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Ball : public sf::Drawable {
    public:
        Ball(float t_X, float t_Y); //coordinates
        Ball() = delete;
        ~Ball() = default;

        int update(float window_X, float window_Y);

        void updatePosition(float t_X, float t_Y);

        void moveUp();
        void moveDown();
        void moveRight();
        void moveLeft();

        Vector2f getPosition();

        float left();
        float right();
        float top();
        float bottom();
    private:
        CircleShape shape;
        const float ballRadius { 10.0f };
        const float ballVelocity { 6.0f };
        Vector2f velocity { ballVelocity, ballVelocity};

        void draw(RenderTarget& target, RenderStates state) const override;
};


#endif SFMLTEST_BALL_H
