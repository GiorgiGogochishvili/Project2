#include <SFML/Graphics.hpp>

class Player
{
public:
    Player(const std::string& texturePath, const sf::Vector2f& position, float scale, float movementSpeed)
        : movementSpeed(movementSpeed), movingLeft(false), movingRight(false)
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

    void move(float x, float y)
    {
        sprite.move(x, y);
    }

    void handleMovement()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            move(-movementSpeed, 0);
            movingLeft = true;
            movingRight = false;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            move(movementSpeed, 0);
            movingLeft = false;
            movingRight = true;
        }
        else
        {
            movingLeft = false;
            movingRight = false;
        }
    }

    bool isMovingHorizontally() const
    {
        return movingLeft || movingRight;
    }

    bool isMovingLeft() const
    {
        return movingLeft;
    }

    bool isMovingRight() const
    {
        return movingRight;
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(sprite);
    }

private:
    sf::Texture texture;
    sf::Sprite sprite;
    float movementSpeed;
    bool movingLeft;
    bool movingRight;
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
            sprite.move(-scrollSpeed * movementSpeed, 0);  // Move left with the same speed as the player
        }
        else if (isMovingRight)
        {
            sprite.move(scrollSpeed * movementSpeed, 0);  // Move right with the same speed as the player
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

    Player player("chasqui.png", sf::Vector2f(0, 530), 0.4f, playerMovementSpeed);
    Floor floor("idk.psd", sf::Vector2f(0, 624), window.getSize().x / static_cast<float>(530), 0.2f);  // Add a scroll speed of 0.2f

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        player.handleMovement();

        window.clear(sf::Color(135, 206, 235));
        player.draw(window);

        if (player.isMovingHorizontally())
        {
            floor.update(playerMovementSpeed, player.isMovingLeft(), player.isMovingRight());
        }

        floor.draw(window);
        window.display();
    }

    return 0;
}
