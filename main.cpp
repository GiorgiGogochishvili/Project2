#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <Windows.h>


using namespace std;
// libraries and namespaces 

class visualize // abstract class vizualize
{  
protected:
    virtual void draw(sf::RenderWindow& window) = 0; 
    virtual sf::Vector2f getPosition() const = 0; 
};
class Person : visualize // derived class Person that inherits from the visualize
{
protected:
    vector<string>& texturePaths;  // Vector of textures we load
    vector<sf::Texture> textures;  // Vector of loaded textures
    sf::Vector2f position;         // Position of the person
    float scale;                   // Scale of the person
    int currentTextureIndex;    // Index of the current texture being displayed
    int TextureIndex;    // Index of the default texture 
    int IdleTextureIndex;
    sf::Clock IdleTimer;


public:
    sf::Sprite sprite;  // Sprite object for the person

    Person(vector<string>& texturePaths, const sf::Vector2f& position, float scale, int TextureIndex) // non-animated
        : texturePaths(texturePaths), position(position), scale(scale), TextureIndex(TextureIndex), 
        currentTextureIndex(0)
    {
        // Load textures from the provided paths
        for (const auto& texturePath : texturePaths)
        {
            sf::Texture texture;
            if (!texture.loadFromFile(texturePath))
            {
                // Error handling if loading fails for any of the textures
                throw runtime_error("Failed to load texture: " + texturePath);
            }
            textures.push_back(texture);
        }

        // Check if texturePaths vector is empty
        if (texturePaths.empty())
        {
            throw runtime_error("Texture paths vector is empty");
        }

        // Check if currentTextureIndex is out of range
        if (texturePaths.size() <= currentTextureIndex)
        {
            throw runtime_error("Invalid current texture index");
        }

        // Set the texture and properties for the sprite
        sprite.setTexture(textures[TextureIndex]);
        sprite.setScale(scale, scale);
        sprite.setPosition(position);
    }

    Person(vector<string>& texturePaths, const sf::Vector2f& position, float scale, int TextureIndex,int IdleTextureIndex) // animated
        : texturePaths(texturePaths), position(position), scale(scale), TextureIndex(TextureIndex), IdleTextureIndex(IdleTextureIndex),
        currentTextureIndex(0)
    {
        // Load textures from the provided paths
        for (const auto& texturePath : texturePaths)
        {
            sf::Texture texture;
            if (!texture.loadFromFile(texturePath))
            {
                // Error handling if loading fails for any of the textures
                throw runtime_error("Failed to load texture: " + texturePath);
            }
            textures.push_back(texture);
        }

        // Check if texturePaths vector is empty
        if (texturePaths.empty())
        {
            throw runtime_error("Texture paths vector is empty");
        }

        // Check if currentTextureIndex is out of range
        if (texturePaths.size() <= currentTextureIndex)
        {
            throw runtime_error("Invalid current texture index");
        }

        // Set the texture and properties for the sprite
        sprite.setTexture(textures[TextureIndex]);
        sprite.setScale(scale, scale);
        sprite.setPosition(position);
    }

    // Function to draw the sprite 
    void draw(sf::RenderWindow& window) override
    {
        window.draw(sprite);
    }

    // Function to return the position of the sprite
    sf::Vector2f getPosition() const override
    {
        return sprite.getPosition();
    }

    void update() // Update for King and General // animates
    {
        IdleAnimatePerson();
    }
  
    void IdleAnimatePerson()
    {
        
        if (IdleTimer.getElapsedTime().asMilliseconds() >= 1000)
        {
            
            if (currentTextureIndex == IdleTextureIndex)
            {
                currentTextureIndex = TextureIndex;
            }
            else
            {
                currentTextureIndex = IdleTextureIndex;
            }

           
            sprite.setTexture(textures[currentTextureIndex]);
            IdleTimer.restart();
        }
    }
    
};

class Enemy : public Person // derived class Enemy that inherits from the Person
{
public:
    Enemy(vector<string>& texturePaths, const sf::Vector2f& position, float scale, int TextureIndex)
        : Person(texturePaths, position, scale, TextureIndex)
    {

    }


};

class Player : visualize // Derived class Player that inherits from visualize, main class of the game, has everything according to the player and his objects
{
public:
   
    Player(const vector<string>& texturePaths, const sf::Vector2f& position, float scale, float movementSpeed, sf::RenderWindow& window, Enemy& enemy, Enemy& enemy2, Enemy& enemy3)
        : movementSpeed(movementSpeed), movingLeft(false), movingRight(false), currentTextureIndex(0), animationTimer(), idleTimer(), window(window), enemy(enemy), enemy2(enemy2), enemy3(enemy3)
    {
        // Load textures from the provided texture paths
        for (const auto& texturePath : texturePaths)
        {
            sf::Texture texture;
            if (!texture.loadFromFile(texturePath))
            {
                // Error handling if loading fails for any of the textures
                throw runtime_error("Failed to load texture: " + texturePath);
            }
            textures.push_back(texture);
        }

        sprite.setScale(scale, scale);
        sprite.setPosition(position);
        sprite.setTexture(textures[currentTextureIndex]);

        // Set the texture indexes for idle animations
        idleRightTextureIndex = 5;
        idleLeftTextureIndex = 6;
        idleIndex = 0;
        sprite.setTexture(textures[0]);
        idleTimer.restart();

        // Load gold texture
        if (!goldTexture.loadFromFile("gold.png"))
        {
            throw runtime_error("Failed to load texture: gold.png");
        }
        goldSprite.setTexture(goldTexture);
        goldSprite.setScale(0.2f, 0.2f);

        // Load meat texture
        if (!meatTexture.loadFromFile("meat.png"))
        {
            throw runtime_error("Failed to load texture: meat.png");
        }
        meatSprite.setTexture(meatTexture);
        meatSprite.setScale(0.2f, 0.2f);

        // Load weapon texture
        if (!weaponTexture.loadFromFile("weapon.png"))
        {
            throw runtime_error("Failed to load texture: weapon.psd");
        }
        weaponSprite.setTexture(weaponTexture);
        weaponSprite.setScale(0.2f, 0.2f);

        // Load scroll1 texture
        if (!scroll1Texture.loadFromFile("scroll1.psd"))
        {
            throw runtime_error("Failed to load texture: scroll1.psd");
        }
        scroll1Sprite.setTexture(scroll1Texture);
        scroll1Sprite.setScale(0.2f, 0.5f);

        // Load scroll2 texture
        if (!scroll2Texture.loadFromFile("scroll2.psd"))
        {
            throw runtime_error("Failed to load texture: scroll2.psd");
        }
        scroll2Sprite.setTexture(scroll2Texture);
        scroll2Sprite.setScale(0.2f, 0.5f);
    }

   

    // Set the texture for left movement
    void setTextureLeft()
    {
        currentTextureIndex = 0;
        sprite.setTexture(textures[currentTextureIndex]);
        texturesetleft = true;
        texturesetright = false;
    }

    // Set the texture for right movement
    void setTextureRight()
    {
        currentTextureIndex = 1;
        sprite.setTexture(textures[currentTextureIndex]);
        texturesetright = true;
        texturesetleft = false;
    }

    // Animate right
    

    void animateRight() {
        
            if (animationTimer.getElapsedTime().asMilliseconds() >= 750 && animationTimer.getElapsedTime().asMilliseconds() <= 1500)
            {
                currentTextureIndex = 2;
                sprite.setTexture(textures[currentTextureIndex]);
            }
            else if(animationTimer.getElapsedTime().asMilliseconds() >= 1501)
            {
                currentTextureIndex = 1;
                sprite.setTexture(textures[currentTextureIndex]);
                animationTimer.restart();
            }
        
    }

    // Animate left

    void animateLeft() {

        if (animationTimer.getElapsedTime().asMilliseconds() >= 750 && animationTimer.getElapsedTime().asMilliseconds() <= 1500)
        {
            currentTextureIndex = 4;
            sprite.setTexture(textures[currentTextureIndex]);
        }
        else if (animationTimer.getElapsedTime().asMilliseconds() >= 1501)
        {
            currentTextureIndex = 0;
            sprite.setTexture(textures[currentTextureIndex]);
            animationTimer.restart();
        }

    }
   
    void animateDeath() {
        
       
            if (deathTimer.getElapsedTime().asMilliseconds() >= 300 && deathTimer.getElapsedTime().asMilliseconds() <= 799){
            currentTextureIndex = 1;
            sprite.setTexture(textures[currentTextureIndex]);
            }
            else if (deathTimer.getElapsedTime().asMilliseconds() >= 800 && deathTimer.getElapsedTime().asMilliseconds() <= 1299)
            {
                currentTextureIndex = 11;
                sprite.setTexture(textures[currentTextureIndex]);
            }
            else if (deathTimer.getElapsedTime().asMilliseconds() >= 1300 && deathTimer.getElapsedTime().asMilliseconds() <= 1799)
            {
                currentTextureIndex = 12;
                sprite.setTexture(textures[currentTextureIndex]);
            }
            else if (deathTimer.getElapsedTime().asMilliseconds() >= 1800 && deathTimer.getElapsedTime().asMilliseconds() <= 2699)
            {
                currentTextureIndex = 13;
                sprite.setTexture(textures[currentTextureIndex]);
            }
            else if (deathTimer.getElapsedTime().asMilliseconds() >= 2700 && deathTimer.getElapsedTime().asMilliseconds() <= 5999)
            {
                currentTextureIndex = 14;
                sprite.setTexture(textures[currentTextureIndex]);
            }
            else if (deathTimer.getElapsedTime().asMilliseconds() >= 6000)
            {
                window.close();
            }
        

    }

    // Drawing the player and objects
    void draw(sf::RenderWindow& window) override
    {
        window.draw(sprite);

        // Drawing quest objects
        if (quest1Spawned == true)
        {
            window.draw(goldSprite);
            window.draw(meatSprite);
        }
        if (quest2Spawned == true)
        {
            window.draw(weaponSprite);
        }
        if (quest3Spawned == true)
        {
            window.draw(scroll1Sprite);
            window.draw(scroll2Sprite);
        }
    }

    // Events on the key press
    void handleKeyPress(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Left)
        {
            movingLeft = true;
            movingRight = false;
            if(texturesetleft == false)
            setTextureLeft();
        }
        else if (key == sf::Keyboard::Right)
        {
            movingLeft = false;
            movingRight = true;
            if(texturesetright == false)
            setTextureRight();
        }
        else if (key == sf::Keyboard::Enter)
        {
            checkChoice(enemy, enemy2, enemy3);
        }
    }

    // Events on key  release
    void handleKeyRelease(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Left)
        {
            movingLeft = false;
            lookingLeft = true;
            lookingRight = false;
            if (!movingRight)
            {
                idleIndex = 2;
                sprite.setTexture(textures[idleLeftTextureIndex]);
                idleTimer.restart();
            }
        }
        else if (key == sf::Keyboard::Right)
        {
            movingRight = false;
            lookingRight = true;
            lookingLeft = false;
            if (!movingLeft)
            {
                idleIndex = 1;
                sprite.setTexture(textures[idleRightTextureIndex]);
                idleTimer.restart();
            }
        }
    }

    // Spawn the gold
    void spawnGold()
    {
        goldSprite.setPosition(getPosition().x + 220, getPosition().y + 50);
        quest1Spawned = true;
    }

    // Spawn the meat
    void spawnMeat()
    {
        meatSprite.setPosition(getPosition().x - 120, getPosition().y + 50);
    }

    // Spawn the weapon
    void spawnWeapon()
    {
        weaponSprite.setPosition(getPosition().x - 120, getPosition().y + 50);
        quest2Spawned = true;
    }

    // Spawn scrolls
    void spawnScrolls()
    {
        scroll1Sprite.setPosition(getPosition().x - 200, getPosition().y);
        scroll2Sprite.setPosition(getPosition().x + 180, getPosition().y);
        quest3Spawned = true;
    }

    // Check the player's choice during the quest
    void checkChoice(Enemy& enemy, Enemy& enemy2, Enemy& enemy3)
    {
        if (quest1done == false)
        {
            if (lookingLeft == true && canMove == false)
            {
                canMove = true;
                quest1done = true;
                goldSprite.setPosition(-600, 0);
                meatSprite.setPosition(enemy.getPosition().x - 70, enemy.getPosition().y + 50);
            }

            if (lookingRight == true && canMove == false)
            {
                deathTimer.restart();
                death = true;
                movingLeft = false;
                movingRight = false;
            }
        }
        else if (quest2done == false)
        {
            if (lookingRight == true && canMove == false)
            {
                canMove = true;
                quest2done = true;
                weaponSprite.setPosition(-600, 0);
                goldSprite.setPosition(enemy2.getPosition().x + 15, enemy2.getPosition().y + 110);
            }

            if (lookingLeft == true && canMove == false)
            {
                deathTimer.restart();
                death = true;
                movingLeft = false;
                movingRight = false;
            }
        }
        else if (quest3done == false)
        {
            if (lookingLeft == true && canMove == false)
            {
                canMove = true;
                quest3done = true;
                scroll1Sprite.setPosition(-600, 0);
                scroll2Sprite.setPosition(-600, 0);
                enemy3.sprite.setPosition(-600, 0);
            }

            if (lookingRight == true && canMove == false)
            {
                deathTimer.restart();
                death = true;
                movingLeft = false;
                movingRight = false;
            }
        }
    }

    // Update function
    void update(const sf::Sprite& secondBackgroundSprite, const Enemy& enemy, const Enemy& enemy2, const Enemy& enemy3)
    {
        // quest 1
        if (canMove && getDistance(getPosition(), enemy.getPosition()) <= 300 && quest1done == false)
        {
            canMove = false;
            spawnGold();
            spawnMeat();
        }

        // quest 2
        if (canMove && getDistance(getPosition(), enemy2.getPosition()) <= 300 && quest2done == false)
        {
            canMove = false;
            spawnGold();
            spawnWeapon();
        }

        // quest 3
        if (canMove && getDistance(getPosition(), enemy3.getPosition()) <= 400 && quest3done == false)
        {
            canMove = false;
            spawnScrolls();
        }

        // Stop movement at the end of the world
        if (canMove && sprite.getPosition().x > 8860)
        {
            canMove = false;
        }

        // Move the player
        if (canMove)
        {
            if (movingLeft)
            {
                if (sprite.getPosition().x > 200)
                {
                    sprite.move(-movementSpeed, 0);
                    animateLeft();
                }
            }
            else if (movingRight)
            {
                sprite.move(movementSpeed, 0);
                animateRight();
                
            }
            else
            {
                IdleAnimate();
            }
        }

        if (canMove == false) {
            if (death == true) {
                animateDeath();
            }
        }
    }

    // Animate idle
    void IdleAnimate()
    {
        if (idleIndex == 1)
        {
            if (idleTimer.getElapsedTime().asMilliseconds() >= 1000)
            {
                if (idleRightTextureIndex == 5)
                {
                    idleRightTextureIndex = 1;
                }
                else
                {
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
                if (idleLeftTextureIndex == 6)
                {
                    idleLeftTextureIndex = 0;
                }
                else
                {
                    idleLeftTextureIndex = 6;
                }
                sprite.setTexture(textures[idleLeftTextureIndex]);
                idleTimer.restart();
            }
        }
    }

   
 

    //Current position of the player
    sf::Vector2f getPosition() const override
    {
        return sprite.getPosition();
    }

    // Main variables
    bool quest1done = false;
    bool quest2done = false;
    bool quest3done = false;
    bool quest1Spawned = false;
    bool quest2Spawned = false;
    bool quest3Spawned = false;
    bool canMove = true;
    bool lookingLeft = false;
    bool lookingRight = false;
    bool texturesetleft = true;
    bool texturesetright = false;
private:
    //Nain variables
    float movementSpeed;
    bool movingLeft;
    bool movingRight;
    int currentTextureIndex;
    vector<sf::Texture> textures;
    sf::Sprite sprite;
    sf::Clock animationTimer;
    sf::Clock idleTimer;
    int idleRightTextureIndex;
    int idleLeftTextureIndex;
    int idleIndex;
    sf::Clock deathTimer;
    bool death = false;
    sf::RenderWindow& window;
    Enemy& enemy;
    Enemy& enemy2;
    Enemy& enemy3;

    // Textures for quest objects
    sf::Texture goldTexture;
    sf::Texture meatTexture;
    sf::Texture weaponTexture;
    sf::Texture scroll1Texture;
    sf::Texture scroll2Texture;

    // Sprites for quest objects
    sf::Sprite goldSprite;
    sf::Sprite meatSprite;
    sf::Sprite weaponSprite;
    sf::Sprite scroll1Sprite;
    sf::Sprite scroll2Sprite;
    //get distance
    float getDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) const {
        float dx = pos2.x - pos1.x;
        float dy = pos2.y - pos1.y;
        return sqrt(dx * dx + dy * dy);
    }
};



    class Scroll : visualize
    {
    private:
        // Main variables
        vector<sf::Texture> textures;
        sf::Sprite sprite;
        sf::Clock spawnTimer;
        sf::Clock despawnTimer;
        bool spawned;
        float counttime;
        float timespawned;
        bool textSpawned = false;
        sf::Text text;
        bool quest1done;
        bool restarted = false;
        sf::Clock fourthscrolltimer;
        // Get the position of the scroll
        sf::Vector2f getPosition() const override
        {
            return sprite.getPosition();
        }
  


    public:
        bool gameStarted = false;
        bool fadeIn = false;

        Scroll(const vector<string>& scrollTexturePaths)
        {
            for (const auto& texturePath : scrollTexturePaths)
            {
                sf::Texture texture;
                if (!texture.loadFromFile(texturePath))
                {
                    throw runtime_error("Failed to load texture: " + texturePath);
                }
                textures.push_back(texture);
            }





            sprite.setPosition(-600, 506);
        }




        // Spawn the first scroll
        void spawnFirstScroll()
        {
            sprite.setScale(1.7f, 1.7f);
            sprite.setTexture(textures[0]);
            sprite.setPosition(500, 56);

            spawnTimer.restart();
        }
        // Spawn the second scroll
        void spawnSecondScroll()
        {
            sprite.setScale(2.5f, 2.5f);
            sprite.setTexture(textures[1]);
            sprite.setPosition(1800, 186);

            spawnTimer.restart();
        }
        // etc...
        void spawnThirdScroll()
        {
            sprite.setScale(0.6f, 0.6f);
            sprite.setTexture(textures[2]);
            sprite.setPosition(4100, 26);

            spawnTimer.restart();
        }
        void spawnFourthScroll()
        {
            sprite.setScale(0.6f, 0.6f);
            sprite.setTexture(textures[3]);
            sprite.setPosition(6700, 26);

            spawnTimer.restart();
        }

        void spawnFifthScroll()
        {
            sprite.setScale(0.6f, 0.6f);
            sprite.setTexture(textures[5]);
            sprite.setPosition(8700, 26);

            spawnTimer.restart();
        }

        void spawnSixthScroll()
        {
            sprite.setScale(0.6f, 0.6f);
            sprite.setTexture(textures[6]);
            sprite.setPosition(9000, 26);

            spawnTimer.restart();
        }
        // Scroll despawn function
        void despawnScroll()
        {
            spawned = true;
            sprite.setPosition(-600, 506);
            
        }

        // Update functions for all scrolls
        void updatefirstscroll(const Player& player)
        {
            if (!spawned && spawnTimer.getElapsedTime().asSeconds() >= 3.0f)
            {
                timespawned = spawnTimer.getElapsedTime().asSeconds();
                spawnFirstScroll();
            }
            if (despawnTimer.getElapsedTime().asSeconds() >= timespawned + 6.0f)
            {
                despawnScroll();
                gameStarted = true;
            }

        }
        void updatesecondscroll(const Player& player)
        {

            if (!spawned) {
                if (player.getPosition().x >= 1480 && player.getPosition().x <= 1520)
                {
                    spawnSecondScroll();


                }

                if (player.quest1done == true)
                {

                    despawnScroll();

                }
            }
        }

        void updatethirdscroll(const Player& player)
        {

            if (!spawned) {
                if (player.getPosition().x >= 3980 && player.getPosition().x <= 4020)
                {
                    spawnThirdScroll();


                }

                if (player.quest2done == true)
                {
                    despawnScroll();

                }
            }
        }

        void updatefourthscroll(const Player& player, Enemy& enemy)
        {
            if (!spawned) {

                if (player.getPosition().x >= 6580 && player.getPosition().x <= 6620)
                {
                    spawnFourthScroll();


                }
                // a bit different here,enemy should dissapear too,and another scroll should spawn
                if (player.quest3done == true)
                {
                    if (!spawned)
                    {
                        sprite.setTexture(textures[4]);


                        if (!restarted) {
                            fourthscrolltimer.restart();
                            restarted = true;
                        }

                        if (fourthscrolltimer.getElapsedTime().asSeconds() >= 3.0f)
                            despawnScroll();

                        

                    }

                }
            }
        }
        //Last 2 scrolls,dialogue between Chasqui and King,Ending,screen becomes black
        void updatefifthscroll(const Player& player)
        {
            if (!spawned) {

                if (player.getPosition().x >= 8850 && player.getPosition().x <= 8870)
                {



                    spawnFifthScroll();

                    if (!restarted) {
                        fourthscrolltimer.restart();
                        restarted = true;
                    }

                    if (fourthscrolltimer.getElapsedTime().asSeconds() >= 5.0f) {
                        spawnSixthScroll();

                    }
                    if (fourthscrolltimer.getElapsedTime().asSeconds() >= 10.0f)
                        fadeIn = true;
                }
            }
        }

        // Again draw function
        void draw(sf::RenderWindow& window) override
        {
            window.draw(sprite);

        }
    };


void run() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Inca adventure"); // Game window
    sf::RectangleShape fadeRect(sf::Vector2f(window.getSize().x, window.getSize().y)); // Black screen
    sf::Time totalDuration = sf::seconds(13.0f);
    //^ For black screen to appear in the end
   
    sf::Font font;
    if (!font.loadFromFile("8bitlim.ttf"))
    {
        // Error handling if font loading fails
        throw runtime_error("Failed to load text: 8bitlim.ttf ");
    }
    window.setFramerateLimit(60);
    // Full size of screen(not full screen) used windows.h library
    ShowWindow(window.getSystemHandle(), SW_SHOWMAXIMIZED); // https://github.com/SFML/SFML/issues/744


    // Last text(about authors)
    sf::Text text("Authors -\n"
        "David Barsegyan\n"
        "Giorgi Gogochishvili\n"
        "Inca..Adventure", font, 34);
    text.setFillColor(sf::Color::White);
    text.setPosition(9000,
        500);

    bool fadeIn = true;


    float playerMovementSpeed = 8.0f;
    // Paths to the game textures (container)
    vector<string> texturePaths = {
        "chasqui-left.png",
        "chasqui.png",
        "rightmv4.png",
        "rightmv2.png",
        "leftmv1.png",
        "idlechasquiright.png",
        "idlechasquileft.png",
        "background.psd",
        "king.png",
        "general.png",
        "jariskac.png",
        "death1.png",
        "death2.png",
        "death3.png",
        "death4.png",
        "generalmv.psd",
        "kingmv.png"
    };

    vector<string> scrollTexturePaths = {
        "generaltext.png",
        "pumatext.png",
        "robbertext.psd",
        "wizardtext.psd",
        "wizarddeath.psd",
        "chasquitext.psd",
        "kingtext.psd"
    };

    vector<string> enemyTexturePaths = {
       "puma.png",
       "robbert.png",
       "wizard.png"


    };

    
    // Entities of the game
    Person General(texturePaths, sf::Vector2f(0, 506), 0.4f, 9,15);
    Enemy enemy(enemyTexturePaths, sf::Vector2f(1800, 706), 0.8f, 0);
    Enemy enemy2(enemyTexturePaths, sf::Vector2f(4300, 590), 1.1f, 1);
    Enemy enemy3(enemyTexturePaths, sf::Vector2f(7000, 560), 0.5f, 2);
    Player player(texturePaths, sf::Vector2f(200, 626), 0.4f, playerMovementSpeed, window, enemy, enemy2, enemy3);
    Scroll firstScroll(scrollTexturePaths);
    Scroll secondScroll(scrollTexturePaths);
    Scroll thirdScroll(scrollTexturePaths);
    Scroll fourthScroll(scrollTexturePaths);
    Scroll lastScroll(scrollTexturePaths);

    // Backgrounds
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.psd"))
    {

        throw runtime_error("Failed to load texture: background.psd");
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(1.0f, 1.0f);
    backgroundSprite.setPosition(0, 0);

    sf::Sprite secondBackgroundSprite;
    secondBackgroundSprite.setTexture(backgroundTexture);
    secondBackgroundSprite.setScale(1.0f, 1.0f);
    secondBackgroundSprite.setPosition(backgroundSprite.getGlobalBounds().width, 0);
   
    // Another entities of the game
    sf::Vector2f kingPosition = sf::Vector2f(secondBackgroundSprite.getPosition().x + secondBackgroundSprite.getGlobalBounds().width - 400, 556);
    sf::Vector2f jariskaci1Position = sf::Vector2f(secondBackgroundSprite.getPosition().x + secondBackgroundSprite.getGlobalBounds().width - 235, 506);
    sf::Vector2f jariskaci2Position = sf::Vector2f(secondBackgroundSprite.getPosition().x + secondBackgroundSprite.getGlobalBounds().width - 700, 506);
    Person King(texturePaths, kingPosition, 0.4f, 8,16);
    Person jariskaci1(texturePaths, jariskaci1Position, 0.4f, 10);
    Person jariskaci2(texturePaths, jariskaci2Position, 0.4f, 10);
    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));// view,specify rectangular area from (0,0) to max width/height of screen
    while (window.isOpen())
    {

        sf::Event event;
        // Handle events
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (firstScroll.gameStarted == true) {
                    player.handleKeyPress(event.key.code);
                }
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                if (firstScroll.gameStarted == true) {
                    player.handleKeyRelease(event.key.code);
                }
            }

        }
        // Black screen effect with text
        if (lastScroll.fadeIn == true) {
            sf::Clock clock;
            while (clock.getElapsedTime() < totalDuration)
            {
      

                

                // Update the background color 
                window.clear(sf::Color(0, 0, 0, 255));
                window.draw(text);
                window.display();

                

            }
            window.close();
        }
        // Updating and drawing
        player.update(secondBackgroundSprite, enemy, enemy2, enemy3);
        firstScroll.updatefirstscroll(player);
        secondScroll.updatesecondscroll(player);
        thirdScroll.updatethirdscroll(player);
        fourthScroll.updatefourthscroll(player, enemy3);
        lastScroll.updatefifthscroll(player);
        General.update();
        King.update();
        view.setCenter(player.getPosition().x + window.getSize().x / 4, window.getSize().y / 2); // update view
        window.setView(view);
        window.clear(sf::Color(135, 206, 235));
        window.draw(backgroundSprite);
        window.draw(secondBackgroundSprite);
        firstScroll.draw(window);
        secondScroll.draw(window);
        thirdScroll.draw(window);
        fourthScroll.draw(window);
        lastScroll.draw(window);
        General.draw(window);
        King.draw(window);
        player.draw(window);
        jariskaci1.draw(window);
        jariskaci2.draw(window);
        enemy.draw(window);
        enemy2.draw(window);
        enemy3.draw(window);


        // Display
        window.display();
    }

}

int main()
{

    run(); // Game run

   

    return 0;
}


