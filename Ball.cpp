#include "Ball.h"

Ball::Ball(float t_X, float t_Y) {
    shape.setPosition(t_X, t_Y);
    shape.setRadius(this->ballRadius);
    shape.setFillColor(Color::White);
    shape.setOrigin(this->ballRadius, this->ballRadius); //change the origin of ball to centre
}

void Ball::draw(RenderTarget &target, RenderStates state) const {
    target.draw(this->shape, state);
}

int Ball::update(float window_X, float window_Y) {
    shape.move(this->velocity);

    if (this->left() < 0) {
        velocity.x = ballVelocity;
    }
    else if (this->right() > window_X) {
        velocity.x = -ballVelocity;
    }
    else if (this->top() < 0) {
        velocity.y = ballVelocity;
    }
    else if (this->bottom() > window_Y) { // GAME OVER
        return false;
    }

    return true;
}

void Ball::updatePosition(float t_X, float t_Y) {
    shape.setPosition(t_X, t_Y);
}

float Ball::left() {
    return this->shape.getPosition().x - shape.getRadius();
}

float Ball::right() {
    return this->shape.getPosition().x + shape.getRadius();
}

float Ball::top() {
    return this->shape.getPosition().y - shape.getRadius();
}

float Ball::bottom() {
    return this->shape.getPosition().y + shape.getRadius();
}

void Ball::moveDown() {
    this->velocity.y = ballVelocity;
};

void Ball::moveUp() {
    this->velocity.y = -ballVelocity;
};

void Ball::moveRight() {
    this->velocity.x = ballVelocity;
};

void Ball::moveLeft() {
    this->velocity.x = -ballVelocity;
};

Vector2f Ball::getPosition() {
    return shape.getPosition();
}