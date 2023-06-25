#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

class Person
{

protected:
    std::vector<std::string>& texturePaths;
    std::vector<sf::Texture> textures;
    sf::Vector2f position;
    float scale;  
    sf::Sprite sprite;
    std::size_t currentTextureIndex;
    sf::Clock animationTimer;
    std::size_t idleTextureIndex;
    std::size_t TextureIndex;
    sf::Clock idleTimer;

public:
    Person(std::vector<std::string>& texturePaths, const sf::Vector2f& position, float scale,size_t TextureIndex)
        : texturePaths(texturePaths), position(position), scale(scale), TextureIndex(TextureIndex), currentTextureIndex(0),
        idleTextureIndex(0)
    {

        for (const auto& texturePath : texturePaths)
        {
            sf::Texture texture;
            if (!texture.loadFromFile(texturePath))
            {
                // Error handling if loading fails for any of the textures
                throw std::runtime_error("Failed to load texture: " + texturePath);
            }
            textures.push_back(texture);
        }

        if (texturePaths.empty())
        {
            throw std::runtime_error("Texture paths vector is empty");
        }

        if (texturePaths.size() <= currentTextureIndex)
        {
            throw std::runtime_error("Invalid current texture index");
        }

        sf::Texture texture;
        if (!texture.loadFromFile(texturePaths[currentTextureIndex]))
        {
            throw std::runtime_error("Failed to load texture: " + texturePaths[currentTextureIndex]);
        }

        // currentTextureIndex = idleTextureIndex;
        sprite.setTexture(textures[TextureIndex]);
        sprite.setScale(scale, scale);
        sprite.setPosition(position);
    }


    sf::Vector2f getPosition() const
    {
        return sprite.getPosition();
    }

     void draw(sf::RenderWindow& window)
     {
         window.draw(sprite);
     }

    /*void IdleAnimatePerson()
    { 
            if (idleTimer.getElapsedTime().asMilliseconds() >= 1000)
            {
                if (currentTextureIndex == idleTextureIndex ) {
                    currentTextureIndex = TextureIndex;
                }
                else {
                    currentTextureIndex = idleTextureIndex;
                }
                sprite.setTexture(textures[currentTextureIndex]);
                idleTimer.restart();
            }
    }*/
};


class Player
{
public:
    Player(const std::vector<std::string>& texturePaths, const sf::Vector2f& position, float scale, float movementSpeed)
        : movementSpeed(movementSpeed), movingLeft(false), movingRight(false), currentTextureIndex(0), animationTimer(), idleTimer()
    {
        for (const auto& texturePath : texturePaths)
        {
            sf::Texture texture;
            if (!texture.loadFromFile(texturePath))
            {
                // Error handling if loading fails for any of the textures
                throw std::runtime_error("Failed to load texture: " + texturePath);
            }
            textures.push_back(texture);
        }

        sprite.setScale(scale, scale);
        sprite.setPosition(position);
        sprite.setTexture(textures[currentTextureIndex]);

        // Set the idle texture initially
        idleRightTextureIndex = 5;
        idleLeftTextureIndex = 6;
        idleIndex = 0;
        sprite.setTexture(textures[0]);
        idleTimer.restart();
    }

    void move(float x, float y)
    {
        sprite.move(x, y);
    }

    void setTextureLeft()
    {
        currentTextureIndex = 0;  // Set the texture index for left movement
        sprite.setTexture(textures[currentTextureIndex]);
    }

    void setTextureRight()
    {
        currentTextureIndex = 1;  // Set the texture index for right movement
        sprite.setTexture(textures[currentTextureIndex]);
    }

    void animateRight()
    {
        currentTextureIndex++;
        if (currentTextureIndex >= textures.size() - 2)
        {
            currentTextureIndex = 2;  // Skip the first two textures (chasqui.png and chasqui-left.png)
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
            if (!movingRight) {
                idleIndex = 2;
                sprite.setTexture(textures[idleLeftTextureIndex]);
                idleTimer.restart();
            }
        }
        else if (key == sf::Keyboard::Right)
        {
            movingRight = false;
            if (!movingLeft) {
                idleIndex = 1;
                sprite.setTexture(textures[idleRightTextureIndex]);
                idleTimer.restart();
            }
        }
    }

    void update(const sf::Sprite& secondBackgroundSprite)
    {
        if (movingLeft)
        {
            if (sprite.getPosition().x > 200)
            {
                sprite.move(-movementSpeed, 0);
            }

        }
        else if (movingRight)
        {

            if (sprite.getPosition().x + sprite.getGlobalBounds().width < secondBackgroundSprite.getPosition().x + secondBackgroundSprite.getGlobalBounds().width - 920) {
                sprite.move(movementSpeed, 0);
                if (animationTimer.getElapsedTime().asMilliseconds() >= 500)
                {
                    animateRight();
                }
            }
        }
        else {

            IdleAnimate();

        }



    }

    void IdleAnimate()
    {
        if (idleIndex == 1)
        {
            if (idleTimer.getElapsedTime().asMilliseconds() >= 1000)
            {
                if (idleRightTextureIndex == 5) {
                    idleRightTextureIndex = 1;
                }
                else {
                    idleRightTextureIndex = 5;
                }
                sprite.setTexture(textures[idleRightTextureIndex]);
                idleTimer.restart();
            }
        }
        else if (idleIndex == 2)
        {
            if (idleTimer.getElapsedTime().asMilliseconds() >= 1000)
            {
                if (idleLeftTextureIndex == 6) {
                    idleLeftTextureIndex = 0;
                }
                else {
                    idleLeftTextureIndex = 6;
                }
                sprite.setTexture(textures[idleLeftTextureIndex]);
                idleTimer.restart();
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

    sf::Vector2f getPosition() const
    {
        return sprite.getPosition();
    }


private:
    std::vector<sf::Texture> textures;
    sf::Sprite sprite;
    float movementSpeed;
    bool movingLeft;
    bool movingRight;
    std::size_t currentTextureIndex;
    sf::Clock animationTimer;
    std::size_t idleRightTextureIndex;
    std::size_t idleLeftTextureIndex;
    sf::Clock idleTimer;
    int idleIndex;

};




int main()
{

    sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Ink adventure");


    float playerMovementSpeed = 0.6f;

    std::vector<std::string> texturePaths = {
        "chasqui-left.png",
        "chasqui.png",
        "rightmv1.png",
        "rightmv2.png",
        "rightmv3.png",
        "idlechasquiright.png",
        "idlechasquileft.png",
        "background.psd",
        "king.png",
        "general.png"
    };

    Player player(texturePaths, sf::Vector2f(200, 626), 0.4f, playerMovementSpeed);
    Person General(texturePaths, sf::Vector2f(0, 506), 0.4f, 9);
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.psd"))
    {
        // Error handling if loading fails for the background texture
        throw std::runtime_error("Failed to load texture: background.psd");
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(1.0f, 1.0f);
    backgroundSprite.setPosition(0, 0);

    sf::Sprite secondBackgroundSprite;
    secondBackgroundSprite.setTexture(backgroundTexture);
    secondBackgroundSprite.setScale(1.0f, 1.0f);
    secondBackgroundSprite.setPosition(backgroundSprite.getGlobalBounds().width, 0);
    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    view.setCenter(player.getPosition().x + window.getSize().x / 4, window.getSize().y / 2);

    sf::Vector2f kingPosition = sf::Vector2f(secondBackgroundSprite.getPosition().x + secondBackgroundSprite.getGlobalBounds().width - 400, 556);
    Person King(texturePaths, kingPosition, 0.4f, 8);

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

        player.update(secondBackgroundSprite);  // Update player animation and position

        view.setCenter(player.getPosition().x + window.getSize().x / 4, window.getSize().y / 2);
        window.setView(view);

        window.draw(backgroundSprite);
        window.draw(secondBackgroundSprite);

        General.draw(window);
        King.draw(window);
        player.draw(window);




        window.display();
    }

    return 0;
}
