#include "model.hpp"
#include <cmath>
#include <iostream>

namespace cs3520 {
    SceneObject::SceneObject(int height, int width, int x, int y): 
                             height(height), width(width), pos(x, y){
    }

    bool SceneObject::collision(SceneObject& obj) {
        return ((pos.first >= obj.pos.first && pos.first < obj.pos.first + obj.width) 
                || (pos.first + width > obj.pos.first && pos.first + width < obj.pos.first + obj.width))
            && ((pos.second >= obj.pos.second && pos.second < obj.pos.second + obj.height )
                || (pos.second + height > obj.pos.second && pos.second + height < obj.pos.second + obj.height));
    }

    std::pair<int, int> SceneObject::getPos() {
        return pos;
    }

    int SceneObject::getWidth() {
        return width;
    }

    int SceneObject::getHeight() {
        return height;
    }

    Ball::Ball(int radius): SceneObject(2*radius, 2*radius, 250, 400), radius(radius), speed(5), direction(.5), hitTop(false){
    }

    Ball::Ball(): Ball(8){}

    void Ball::draw(sf::RenderWindow& window) {
        sf::CircleShape circle(radius);
        circle.setFillColor(sf::Color(255, 0, 0));
        circle.setPosition(pos.first, pos.second);
        window.draw(circle);
    }

    void Ball::bounceVertical() {
        if(direction < M_PI/2) {
            direction = 3*M_PI/2 + (M_PI/2 - direction);
        } else {
            direction = 3*M_PI/2 - (direction - M_PI/2);
        }
    }

    void Ball::bounceHorizontal() {
        if(direction < M_PI) {
            direction = M_PI - direction;
        } else {
            direction = M_PI + 2*M_PI - direction;
        }
    }

    bool Ball::move(Paddle& player) {
        bool hitSomthing = false;
        if(pos.first > 500-2*radius || pos.first < 0) {
            bounceHorizontal();
            hitSomthing = true;
        }

        if(pos.second < 0) {
            bounceVertical();
            hitSomthing = true;
            if(!hitTop) {
                hitTop = true;
                speed *= 1.5;
            }
            
        }

        if(collision(player)) {
            direction = (1.0 - ((pos.first + radius - player.getPos().first) 
                                / (float)(player.getWidth()))) 
                        * M_PI/2 
                        + M_PI/4;
            hitSomthing = true;
        }

        pos.first += std::cos(direction) * speed;
        pos.second -= std::sin(direction) * speed;
        return hitSomthing;
    }

    void Ball::handleBlockCollision(SceneObject block) {
        int centerX = pos.first + radius;
        int blockX = block.getPos().first;
        if(centerX < blockX || centerX > blockX + block.getWidth()) {
            bounceHorizontal();
        } else {
            bounceVertical();
        }
    }

    int& Ball::getSpeed() {
        return speed;
    }

    Paddle::Paddle(): SceneObject(10, 150, 250, 450), speed(5){
    }

    Paddle::Paddle(int width): SceneObject(10, width, 250, 450), speed(5){
    }

    void Paddle::draw(sf::RenderWindow& window) {
        sf::RectangleShape rectangle(sf::Vector2f(width, height));
        rectangle.setFillColor(sf::Color(150, 50, 250));
        rectangle.setPosition(pos.first, pos.second);
        window.draw(rectangle);
    }

    void Paddle::move(bool left, bool right) {
        if(left) {
            pos.first = std::max(pos.first - speed, 0);
        }

        if(right) {
            pos.first =std::min(pos.first + speed, 500 - width);
        }
    }

    Block::Block(int x, int y, int h, int w, int value, const sf::Color& color): SceneObject(x,y,h,w), value(value), color(color){}

    void Block::draw(sf::RenderWindow& window) {
        sf::RectangleShape rectangle(sf::Vector2f(width, height));
        rectangle.setFillColor(color);
        rectangle.setPosition(pos.first, pos.second);
        window.draw(rectangle);
    }

    int Block::getScore() {
        return value;
    }
}