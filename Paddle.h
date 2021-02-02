#ifndef SFMLTEST_PADDLE_H
#define SFMLTEST_PADDLE_H
#include <SFML/Graphics.hpp>

using namespace sf;

class Paddle : public sf::Drawable {
    public:
        Paddle(float t_X, float t_Y);
        Paddle() = delete;
        ~Paddle() = default;

        void update(float window_X);

        float left();
        float right();
        float top();
        float bottom();

        Vector2f getPosition();
    private:
        RectangleShape shape;
        const float paddleWidth { 120.0f };
        const float paddleHeight { 20.0f };
        const float paddleVelocity { 8.0f };
        Vector2f velocity { paddleVelocity, 0.f};

        void draw(RenderTarget& target, RenderStates state) const override;
};


#endif SFMLTEST_PADDLE_H
