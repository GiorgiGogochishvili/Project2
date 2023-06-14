#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include <thread>

class Player
{
public:
    Player(const std::vector<std::string>& texturePaths, const sf::Vector2f& position, float scale, float movementSpeed)
        : movementSpeed(movementSpeed), movingLeft(false), movingRight(false), currentTextureIndex(0), animationTimer()
    {
        for (const auto& texturePath : texturePaths)
        {
            sf::Texture texture;
            if (!texture.loadFromFile(texturePath))
            {
              
                throw std::runtime_error("Failed to load texture: " + texturePath);
            }
            textures.push_back(texture);
        }

        sprite.setScale(scale, scale);
        sprite.setPosition(position);
        sprite.setTexture(textures[currentTextureIndex]);
    }

    void move(float x, float y)
    {
        sprite.move(x, y);
    }

    void setTextureLeft()
    {
        currentTextureIndex = 0;  
        sprite.setTexture(textures[currentTextureIndex]);
    }

    void setTextureRight()
    {
        currentTextureIndex = 1;  
        sprite.setTexture(textures[currentTextureIndex]);
    }

    void animateRight()
    {
        currentTextureIndex++;
        if (currentTextureIndex >= textures.size())
        {
            currentTextureIndex = 2;  
        }
        sprite.setTexture(textures[currentTextureIndex]);
        animationTimer.restart();
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(sprite);
    }

    void handleKeyPress(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Left)
        {
            movingLeft = true;
            movingRight = false;
            setTextureLeft();
        }
        else if (key == sf::Keyboard::Right)
        {
            movingLeft = false;
            movingRight = true;
            setTextureRight();
        }
    }

    void handleKeyRelease(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Left)
        {
            movingLeft = false;
        }
        else if (key == sf::Keyboard::Right)
        {
            movingRight = false;
        }
    }

    void update()
    {
        if (movingLeft)
        {
            sprite.move(-movementSpeed, 0);
        }
        else if (movingRight)
        {
            sprite.move(movementSpeed, 0);
            if (animationTimer.getElapsedTime().asMilliseconds() >= 500)
            {
                animateRight();
            }
        }
    }

    bool isMovingLeft() const
    {
        return movingLeft;
    }

    bool isMovingRight() const
    {
        return movingRight;
    }

private:
    std::vector<sf::Texture> textures;
    sf::Sprite sprite;
    float movementSpeed;
    bool movingLeft;
    bool movingRight;
    std::size_t currentTextureIndex;
    sf::Clock animationTimer;
};


class Floor
{
public:
    Floor(const std::string& texturePath, const sf::Vector2f& position, float scale, float scrollSpeed)
        : scrollSpeed(scrollSpeed)
    {
        if (!texture.loadFromFile(texturePath))
        {
            // Error handling if loading fails for the texture
            throw std::runtime_error("Failed to load texture: " + texturePath);
        }

        sprite.setTexture(texture);
        sprite.setScale(scale, scale);
        sprite.setPosition(position);
    }

    void update(float movementSpeed, bool isMovingLeft, bool isMovingRight)
    {
        if (isMovingLeft)
        {
            sprite.move(-scrollSpeed * movementSpeed, 0);  
        }
        else if (isMovingRight)
        {
            sprite.move(scrollSpeed * movementSpeed, 0); 
        }
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(sprite);
    }

private:
    sf::Texture texture;
    sf::Sprite sprite;
    float scrollSpeed;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Ink adventure");

    float playerMovementSpeed = 0.1f;

    std::vector<std::string> texturePaths = {
        "chasqui-left.png",
        "chasqui.png",
        "rightmv1.png",
        "rightmv2.png",
        "rightmv3.png"
    };

    Player player(texturePaths, sf::Vector2f(0, 426), 0.4f, playerMovementSpeed);
    Floor floor("idk.psd", sf::Vector2f(0, 624), window.getSize().x / static_cast<float>(530), 0.2f); 

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                player.handleKeyPress(event.key.code);
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                player.handleKeyRelease(event.key.code);
            }
        }

        window.clear(sf::Color(135, 206, 235));

        player.update();  

        player.draw(window);

        if (player.isMovingLeft() || player.isMovingRight())
        {
            floor.update(playerMovementSpeed, player.isMovingLeft(), player.isMovingRight());
        }

        floor.draw(window);
        window.display();
    }

    return 0;
}
