#pragma once
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
/*
    Game engine 
*/
class Game
{
private:
    //Variables
    std::unique_ptr<sf::RenderWindow>   window;
    //sf::RenderWindow*   window;
    sf::Event                           event;
    sf::VideoMode                       videomode;
    std::string                         windowTitle;
    int                                 windowHeight;
    int                                 windowWidth;
    int                                 frameLimit;
    double                              windowScale;
    int                                 windowStyle;

    //Game Logic
    sf::Clock   enemySpawnClock;
    sf::Time    enemySpawnTimerMax;
    sf::Time    enemySpawnTimer;
    
    int         maxEnemies;
    int         points;

    //Game objects
    std::shared_ptr<sf::RectangleShape>                   enemy;
    std::vector<std::shared_ptr<sf::RectangleShape>>    enemyList;

    //Experimental
    sf::Vector2i                        mousePosWindow;
    sf::Vector2f                        mousePosView;  //position on screen to float
    int                                 aniFrame;
    sf::View                            view;
    sf::Clock                           clock;
    sf::Clock                           timeClock;
    sf::Texture                         cursorTex;

    // Resources
    sf::Font                            font;
    // Text
    sf::Text                            clockText;
    sf::Text                            scoreText;

    //Functions
    void initalizeVariables();
    void initFonts();
    void initText();
    void updateText();
    void initWindow();
    void updateMousePositions();
    
    void initEnemies();
    void spawnEnemies();
    void updateEnemies();
    void renderEnemies();

    //std::vector<sf::Sprite>             spritesToRender;
    void renderSprites();
    void renderCursor();
    void renderText();



public:
    Game();
    virtual ~Game();

    void update();
    void pollEvents();
    void render();

    //Acessors
    const bool running() const;
};