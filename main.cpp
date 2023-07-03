#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
 
#ifdef _WIN32
#include <Windows.h>
#endif
using namespace std;
 
class Person
{
 
protected:
    vector<string>& texturePaths;
    vector<sf::Texture> textures;
    sf::Vector2f position;
    float scale;
    size_t currentTextureIndex;
    sf::Clock animationTimer;
    size_t idleTextureIndex;
    size_t TextureIndex;
    sf::Clock idleTimer;
 
public:
    sf::Sprite sprite;
    Person(vector<string>& texturePaths, const sf::Vector2f& position, float scale, size_t TextureIndex)
        : texturePaths(texturePaths), position(position), scale(scale), TextureIndex(TextureIndex), currentTextureIndex(0),
        idleTextureIndex(0)
    {
 
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
 
        if (texturePaths.empty())
        {
            throw runtime_error("Texture paths vector is empty");
        }
 
        if (texturePaths.size() <= currentTextureIndex)
        {
            throw runtime_error("Invalid current texture index");
        }
 
        sf::Texture texture;
        if (!texture.loadFromFile(texturePaths[currentTextureIndex]))
        {
            throw runtime_error("Failed to load texture: " + texturePaths[currentTextureIndex]);
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
class Enemy : public Person
{
public:
    Enemy(vector<string>& texturePaths, const sf::Vector2f& position, float scale, size_t TextureIndex)
        : Person(texturePaths, position, scale, TextureIndex)
    {
       
    }
 
   
};
 
class Player
{
public:
    Player(const vector<string>& texturePaths, const sf::Vector2f& position, float scale, float movementSpeed, sf::RenderWindow& window,Enemy& enemy,Enemy& enemy2,Enemy& enemy3)
        : movementSpeed(movementSpeed), movingLeft(false), movingRight(false), currentTextureIndex(0), animationTimer(), idleTimer(),window(window),enemy(enemy),enemy2(enemy2),enemy3(enemy3)
    {
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
 
        
        idleRightTextureIndex = 5;
        idleLeftTextureIndex = 6;
        idleIndex = 0;
        sprite.setTexture(textures[0]);
        idleTimer.restart();
 
        if (!goldTexture.loadFromFile("gold.png"))
        {
            throw runtime_error("Failed to load texture: gold.png");
        }
        
        goldSprite.setTexture(goldTexture);
        goldSprite.setScale(0.2f, 0.2f);
        
 
       
        if (!meatTexture.loadFromFile("meat.png"))
        {
            throw runtime_error("Failed to load texture: meat.png");
        }
       
        meatSprite.setTexture(meatTexture);
        meatSprite.setScale(0.2f, 0.2f);
 
        if (!weaponTexture.loadFromFile("weapon.png"))
        {
            throw runtime_error("Failed to load texture: weapon.psd");
        }
 
        weaponSprite.setTexture(weaponTexture);
        weaponSprite.setScale(0.2f, 0.2f);
 
        if (!scroll1Texture.loadFromFile("scroll1.psd"))
        {
            throw runtime_error("Failed to load texture: scroll1.psd");
        }
 
        scroll1Sprite.setTexture(scroll1Texture);
        scroll1Sprite.setScale(0.2f, 0.5f);
 
        if (!scroll2Texture.loadFromFile("scroll2.psd"))
        {
            throw runtime_error("Failed to load texture: scroll2.psd");
        }
 
        scroll2Sprite.setTexture(scroll2Texture);
        scroll2Sprite.setScale(0.2f, 0.5f);
 
 
 
        
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
 
        if (quest1Spawned == true)
        {
            window.draw(goldSprite);
            window.draw(meatSprite);
        }
        if (quest2Spawned == true) {
            window.draw(weaponSprite);
        }
        if (quest3Spawned == true) {
            window.draw(scroll1Sprite);
            window.draw(scroll2Sprite);
        }
 
        
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
        else if
            (key == sf::Keyboard::Enter) {
            checkChoice(enemy,enemy2,enemy3);
        }
        
    }
 
    void handleKeyRelease(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Left)
        {
            movingLeft = false;
            lookingLeft = true;
            lookingRight = false;
            if (!movingRight) {
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
            if (!movingLeft) {
                idleIndex = 1;
                sprite.setTexture(textures[idleRightTextureIndex]);
                idleTimer.restart();
            }
        }
        
    }
 
    void spawnGold()
    {
        goldSprite.setPosition(getPosition().x + 220,getPosition().y+50 );
        quest1Spawned = true;
    }
 
    void spawnMeat()
    {
        meatSprite.setPosition(getPosition().x - 120, getPosition().y+50 );
        
    }
 
    void spawnWeapon()
    {
        weaponSprite.setPosition(getPosition().x - 120, getPosition().y + 50);
        quest2Spawned = true;
    }
 
    void spawnScrolls()
    {
        scroll1Sprite.setPosition(getPosition().x - 200, getPosition().y);
        scroll2Sprite.setPosition(getPosition().x + 180, getPosition().y);
        quest3Spawned = true;
 
    }
 
 
    void checkChoice(Enemy& enemy, Enemy& enemy2, Enemy& enemy3)
    {
       if(quest1done==false){
        if (lookingLeft==true && canMove==false)
        {
            canMove = true;
            quest1done = true;
            goldSprite.setPosition(-600, 0);
            meatSprite.setPosition(enemy.getPosition().x - 70, enemy.getPosition().y + 50);
 
        }
 
        if (lookingRight == true && canMove == false)
        {
            window.close();
        }
       }
       else if (quest2done == false) {
           if (lookingRight==true && canMove == false)
           {
               canMove = true;
               quest2done = true;
               weaponSprite.setPosition(-600, 0);
               goldSprite.setPosition(enemy2.getPosition().x + 15, enemy2.getPosition().y + 110);
 
           }
 
           if (lookingLeft == true && canMove == false)
           {
               window.close();
           }
       }
       else if (quest3done == false) {
           if (lookingLeft == true && canMove == false)
           {
               canMove = true;
               quest3done = true;
               scroll1Sprite.setPosition(-600, 0);
               scroll2Sprite.setPosition(-600, 0);
 
           }
 
           if (lookingRight == true && canMove == false)
           {
               window.close();
           }
       }
    }
    
    void update(const sf::Sprite& secondBackgroundSprite, const Enemy& enemy, const Enemy& enemy2, const Enemy& enemy3) {
 
        if (canMove && getDistance(getPosition(), enemy.getPosition()) <= 300 && quest1done == false) {
            canMove = false;
            spawnGold();
            spawnMeat();
    
        }
 
        if (canMove && getDistance(getPosition(), enemy2.getPosition()) <= 300 && quest2done == false) {
            canMove = false;
            spawnGold();
            spawnWeapon();
 
        }
 
        if (canMove && getDistance(getPosition(), enemy3.getPosition()) <= 400 && quest3done == false) {
            canMove = false;
            spawnScrolls();
            
 
        }
 
        if (canMove && sprite.getPosition().x>8860) {
            canMove = false;
           
        }
 
        if (canMove) {
            if (movingLeft) {
                if (sprite.getPosition().x > 200) {
                    sprite.move(-movementSpeed, 0);
                }
            }
            else if (movingRight) {
               // if (sprite.getPosition().x + sprite.getGlobalBounds().width < secondBackgroundSprite.getPosition().x + secondBackgroundSprite.getGlobalBounds().width - 920) {
                    sprite.move(movementSpeed, 0);
                    if (animationTimer.getElapsedTime().asMilliseconds() >= 500) {
                        animateRight();
                    }
                //}
            }
            else {
                IdleAnimate();
            }
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
 
    bool quest1done = false;
    bool quest2done = false;
    bool quest3done = false;
private:
    bool canMove = true;
    vector<sf::Texture> textures;
    sf::Sprite sprite;
    float movementSpeed = 0.0f;
    bool movingLeft = false;
    bool movingRight = false;
    size_t currentTextureIndex;
    sf::Clock animationTimer;
    size_t idleRightTextureIndex;
    size_t idleLeftTextureIndex;
    sf::Clock idleTimer;
    int idleIndex;
    
    sf::Texture goldTexture;
    sf::Texture meatTexture;
    sf::Sprite meatSprite;
    sf::Sprite goldSprite;
    sf::Texture weaponTexture;
    sf::Texture scroll1Texture;
    sf::Texture scroll2Texture;
    sf::Sprite weaponSprite;
    sf::Sprite scroll1Sprite;
    sf::Sprite scroll2Sprite;
    bool quest1Spawned = false;
    bool quest2Spawned = false;
    bool quest3Spawned = false;
    bool lookingRight=false;
    bool lookingLeft=true;
    sf::RenderWindow& window;
    Enemy enemy;
    Enemy enemy2;
    Enemy enemy3;
 
 
    float getDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) const {
        float dx = pos2.x - pos1.x;
        float dy = pos2.y - pos1.y;
        return sqrt(dx * dx + dy * dy);
    }
 
};
 
 
class Scroll
{
private:
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
    float getDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) const {
        float dx = pos2.x - pos1.x;
        float dy = pos2.y - pos1.y;
        return sqrt(dx * dx + dy * dy);
    }
    sf::Vector2f getPosition() const
    {
        return sprite.getPosition();
    }
    float fadeInDuration = 2.0f;
    float fadeAlpha = 0.0f;
 
 
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
 
   
 
    
 
    void spawnFirstScroll()
    {
        sprite.setScale(1.7f, 1.7f);
        sprite.setTexture(textures[0]);
        sprite.setPosition(500, 56);
        
        spawnTimer.restart();
    }
 
    void spawnSecondScroll()
    {
        sprite.setScale(2.5f, 2.5f);
        sprite.setTexture(textures[1]);
        sprite.setPosition(1800, 186);
        
        spawnTimer.restart();
    }
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
 
    void despawnScroll()
    {
        spawned = true;
        sprite.setPosition(-600, 506);
    }
 
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
        if(!spawned){
 
            if (player.getPosition().x >= 6580 && player.getPosition().x <= 6620)
            {
                spawnFourthScroll();
 
 
            }
 
            if (player.quest3done == true)
            {
                if (!spawned)
                {
                    sprite.setTexture(textures[4]);
 
                    
                    if(!restarted){
                     fourthscrolltimer.restart();
                     restarted = true;
                    }
                    
                    if(fourthscrolltimer.getElapsedTime().asSeconds()>=3.0f)
                        despawnScroll();
 
                    enemy.sprite.setPosition(-600, 0);
                    
                }
 
            }
        }
    }
    
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
 
 
 
 
 
    void draw(sf::RenderWindow& window)
    {
        window.draw(sprite);
        
    }
};
 
int main()
{
 
    
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Ink adventure");
    sf::RectangleShape fadeRect(sf::Vector2f(window.getSize().x, window.getSize().y));
    
    fadeRect.setFillColor(sf::Color::Black);
    fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
    const sf::Time totalDuration = sf::seconds(13.0f);
    const sf::Time frameTime = sf::seconds(1.0f / 60.0f);
    sf::Clock clock;
    
    sf::Font font;
    if (!font.loadFromFile("8bitlim.ttf"))
    {
        // Error handling if font loading fails
        return -1;
    }
    window.setFramerateLimit(60);
#ifdef _WIN32
    HWND hwnd = reinterpret_cast<HWND>(window.getSystemHandle());
    ShowWindow(hwnd, SW_MAXIMIZE);
#endif
 
   
    sf::Text text("Authors -\n"
        "David Barsegyan\n"
        "Giorgi Gogochishvili\n"
        "Incas..Adventure", font, 34);
    text.setFillColor(sf::Color::White);
    text.setPosition(9000,
        500);
 
    bool fadeIn = true;
    
 
    float playerMovementSpeed = 8.0f;
 
    vector<string> texturePaths = {
        "chasqui-left.png",
        "chasqui.png",
        "rightmv1.png",
        "rightmv2.png",
        "rightmv3.png",
        "idlechasquiright.png",
        "idlechasquileft.png",
        "background.psd",
        "king.png",
        "general.png",
        "jariskac.png"
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
   
    
   
    Person General(texturePaths, sf::Vector2f(0, 506), 0.4f, 9);
    
    Enemy enemy(enemyTexturePaths, sf::Vector2f(1800, 706), 0.8f, 0);
    Enemy enemy2(enemyTexturePaths, sf::Vector2f(4300, 590), 1.1f, 1);
    Enemy enemy3(enemyTexturePaths, sf::Vector2f(7000, 560), 0.5f, 2);
    Player player(texturePaths, sf::Vector2f(200, 626), 0.4f, playerMovementSpeed, window,enemy,enemy2,enemy3);
    
    
    Scroll firstScroll(scrollTexturePaths);
    Scroll secondScroll(scrollTexturePaths);
    Scroll thirdScroll(scrollTexturePaths);
    Scroll fourthScroll(scrollTexturePaths);
    Scroll lastScroll(scrollTexturePaths);
    
  
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
    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    view.setCenter(player.getPosition().x + window.getSize().x / 4, window.getSize().y / 2);
 
    sf::Vector2f kingPosition = sf::Vector2f(secondBackgroundSprite.getPosition().x + secondBackgroundSprite.getGlobalBounds().width - 400, 556);
    sf::Vector2f jariskaci1Position = sf::Vector2f(secondBackgroundSprite.getPosition().x + secondBackgroundSprite.getGlobalBounds().width - 235, 506);
    sf::Vector2f jariskaci2Position = sf::Vector2f(secondBackgroundSprite.getPosition().x + secondBackgroundSprite.getGlobalBounds().width - 700, 506);
    Person King(texturePaths, kingPosition, 0.4f, 8);
    Person jariskaci1(texturePaths, jariskaci1Position, 0.4f, 10);
    Person jariskaci2(texturePaths, jariskaci2Position, 0.4f, 10);
 
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
        if(lastScroll.fadeIn==true){
        sf::Clock clock;
            while (clock.getElapsedTime() < totalDuration)
            {
                const sf::Time elapsed = clock.getElapsedTime();
                const float alpha = elapsed / totalDuration;
                const sf::Uint8 currentAlpha = static_cast<sf::Uint8>(alpha * 255);
 
                // Update the background color with the current alpha value
                window.clear(sf::Color(0, 0, 0, currentAlpha));
                window.draw(text);
                window.display();
 
                // Wait for the frame time interval
            
            }
        }
 
        player.update(secondBackgroundSprite, enemy, enemy2, enemy3);
        firstScroll.updatefirstscroll(player);
        secondScroll.updatesecondscroll(player);
        thirdScroll.updatethirdscroll(player);
        fourthScroll.updatefourthscroll(player, enemy3);
        lastScroll.updatefifthscroll(player);
        view.setCenter(player.getPosition().x + window.getSize().x / 4, window.getSize().y / 2);
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
 
       
 
        window.display();
    }
 
    return 0;
}
