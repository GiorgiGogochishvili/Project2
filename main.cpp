#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

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
Expand All
	@@ -23,6 +25,13 @@ class Player
        sprite.setScale(scale, scale);
        sprite.setPosition(position);
        sprite.setTexture(textures[currentTextureIndex]);

        // Set the idle texture initially
        idleRightTextureIndex = 5;
        idleLeftTextureIndex = 6;
        idleIndex = 0;
        sprite.setTexture(textures[1]);
        idleTimer.restart();
    }

    void move(float x, float y)
Expand All
	@@ -32,22 +41,22 @@ class Player

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
Expand Down
Expand Up
	@@ -79,10 +88,20 @@ class Player
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

Expand All
	@@ -91,6 +110,7 @@ class Player
        if (movingLeft)
        {
            sprite.move(-movementSpeed, 0);

        }
        else if (movingRight)
        {
Expand All
	@@ -99,6 +119,35 @@ class Player
            {
                animateRight();
            }

        }
        else if (idleIndex == 1)
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

Expand All
	@@ -112,6 +161,12 @@ class Player
        return movingRight;
    }

    sf::Vector2f getPosition() const
    {
        return sprite.getPosition();
    }


private:
    std::vector<sf::Texture> textures;
    sf::Sprite sprite;
Expand All
	@@ -120,6 +175,11 @@ class Player
    bool movingRight;
    std::size_t currentTextureIndex;
    sf::Clock animationTimer;
    std::size_t idleRightTextureIndex;
    std::size_t idleLeftTextureIndex;
    sf::Clock idleTimer;
    int idleIndex;

};


Expand All
	@@ -144,11 +204,11 @@ class Floor
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

Expand All
	@@ -157,6 +217,8 @@ class Floor
        window.draw(sprite);
    }



private:
    sf::Texture texture;
    sf::Sprite sprite;
Expand All
	@@ -166,6 +228,7 @@ class Floor
int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Ink adventure");
    /*Floor floor("floor.psd", sf::Vector2f(0, 624), window.getSize().x / static_cast<float>(530), 0.2f);*/

    float playerMovementSpeed = 0.1f;

Expand All
	@@ -174,11 +237,27 @@ int main()
        "chasqui.png",
        "rightmv1.png",
        "rightmv2.png",
        "rightmv3.png",
        "idlechasquiright.png",
        "idlechasquileft.png",
        "background.psd"
    };

    Player player(texturePaths, sf::Vector2f(0, 626), 0.4f, playerMovementSpeed);


    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.psd"))
    {
        // Error handling if loading fails for the background texture
        throw std::runtime_error("Failed to load texture: background.psd");
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(1.0f, 1.0f);

    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    view.setCenter(player.getPosition().x + window.getSize().x / 4, window.getSize().y / 2);

    while (window.isOpen())
    {
Expand All
	@@ -201,16 +280,25 @@ int main()

        window.clear(sf::Color(135, 206, 235));

        player.update();  // Update player animation and position

        view.setCenter(player.getPosition().x + window.getSize().x / 4, window.getSize().y / 2);
        window.setView(view);

        window.draw(backgroundSprite);

        /*if (player.isMovingLeft() || player.isMovingRight())
        {
            floor.update(playerMovementSpeed, player.isMovingLeft(), player.isMovingRight());
        }
        floor.draw(window);*/

        player.draw(window);




        window.display();
    }
