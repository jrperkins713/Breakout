#include <utility>
#include <SFML/Graphics.hpp>
namespace cs3520 {

    class SceneObject {
    public:
        SceneObject(int, int, int, int);
        bool collision(SceneObject& obj);
        std::pair<int, int> getPos();
        int getWidth();
        int getHeight();
        
    protected:
        int height;
        int width;
        std::pair<int, int> pos;

    };
    class Paddle;

    class Block: public SceneObject {
    public:
        Block(int, int, int, int, int, const sf::Color&);
        void draw(sf::RenderWindow& window);
        int getScore();
    private:
        int value;
        sf::Color color;
    };

    class Ball: public SceneObject {
    public:
        Ball(int radius); 
        Ball(); 
        void draw(sf::RenderWindow& window);
        bool move(Paddle& player);
        void handleBlockCollision(SceneObject block);
        int&  getSpeed();
    private:
        void bounceHorizontal();
        void bounceVertical();
        int radius;
        int speed;
        float direction;
        bool hitTop;
    };

    class Paddle: public SceneObject {
    public:
        Paddle();
        Paddle(int width);  
        void draw(sf::RenderWindow& window);
        void move(bool, bool);
    private:
        int speed;
        
    };
}

