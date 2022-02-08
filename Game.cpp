#include "Game.h"
#include <memory>
#include <sstream>
#include <iostream>

Game::Game(){
    std::cout << "Constructing game" << std::endl;
    this->initalizeVariables();
    this->initFonts();
    this->initText();
    this->initWindow();
    this->initEnemies();
}
Game::~Game(){
    std::cout << "Destructing game" << std::endl;
}

// Game logic
void Game::update(){
    this->pollEvents();
    this->updateMousePositions();
    this->updateText();
    this->updateEnemies();
}

/*
    Gets global mouse position with (0,0) at top left of window
    Tracks mouse outside window as well
*/
void Game::updateMousePositions(){
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView   = this->window->mapPixelToCoords(this->mousePosWindow);
    std::cout << "\t\t\t\t MousePosView " <<mousePosView.x << ' ' << mousePosView.y << '\r';
}

/*
    Handles any game input done within window
*/
void Game::pollEvents(){
    while(this->window->pollEvent(this->event))
        {
            float w, h;
            float wW, wH;
            switch(this->event.type)
            {
                // Window Events
                // This is event based
                // Real time KBM input can be gotten elsewhere
                case sf::Event::Closed:
                    this->window->close();
                    break;
                case sf::Event::Resized:
                    this->windowHeight = this->event.size.height;
                    this->windowWidth  = this->event.size.width;
                    w = static_cast<float>(this->windowWidth);
                    h = static_cast<float>(this->windowHeight);
                    wW = sf::VideoMode::getDesktopMode().width;
                    wH = sf::VideoMode::getDesktopMode().height;
                    view.setSize({w,h});
                    view.setCenter({w/2.f,h/2.f});
                    this->window->setView(view);
                    this->window->setPosition({static_cast<int>(wW - w)/2, static_cast<int>(wH - h)/2});
                    break;

                // Keyboard events
                case sf::Event::KeyPressed:
                    switch(this->event.key.code)
                    {
                        case sf::Keyboard::A:
                            break;
                        case sf::Keyboard::B:
                            break;
                        case sf::Keyboard::Escape:
                            this->window->close();
                            break;
                    }
                        
                    break;

                // Mouse Events
                case sf::Event::MouseWheelScrolled:
                    if( (this->windowScale > 1 && this->event.mouseWheelScroll.delta < 1) || (this->windowScale < 1 && this->event.mouseWheelScroll.delta > 0) ){
                        this->windowScale = 1;
                    }
                    this->windowScale += 0.01 * this->event.mouseWheelScroll.delta;
                    this->windowWidth *= this->windowScale;
                    this->windowHeight *= this->windowScale;
                    this->window->setSize(sf::Vector2u(this->windowWidth,this->windowHeight));
                    break;
                case sf::Event::MouseButtonPressed:
                    switch(this->event.mouseButton.button)
                    {
                        case sf::Mouse::Left:
                            break;
                        case sf::Mouse::Right:
                            break;
                        default:
                            break;
                    }
                    break;
                case sf::Event::MouseMoved:
                    break;
                case sf::Event::MouseEntered:
                    std::cout << "Mouse entered window" << std::endl;
                    break;
                case sf::Event::MouseLeft:
                    std::cout << "Mouse left window" << std::endl;
                    break;


                default:
                    break;

            }
        }
}

// Visualizations
void Game::render(){
    this->window->clear(sf::Color(0,0,0,255));
    // Draw game
    this->renderSprites();  
    this->renderEnemies();
    this->renderText();
    this->window->display();
}

/*
    Defines basic pre-reqs to run window and game logic
*/
void Game::initalizeVariables(){
    this->window                = nullptr;
    this->windowHeight          = 600;
    this->windowWidth           = 800;
    this->videomode.height      = this->windowHeight;
    this->videomode.width       = this->windowWidth;
    this->windowScale           = 1.0;
    this->windowTitle           = "snow clicker";
    this->frameLimit            = 240;
    this->windowStyle           = sf::Style::Default;
    // Experimental
    this->mousePosWindow        = sf::Vector2i(this->windowWidth/2, this->windowHeight/2);
    this->aniFrame              = 0;
    this->cursorTex.loadFromFile("textures/longmid.png");
    //Game Logic
    this->enemySpawnTimerMax    = sf::seconds(0.25f);
    this->enemySpawnTimer       = enemySpawnClock.restart();
    this->maxEnemies            = 5;
    this->points                = 0;
}

/*
    Function to create and launch SFML window via variables defined in
    initializeVariables();
*/
void Game::initWindow(){
    this->window = std::make_unique<sf::RenderWindow>(this->videomode,this->windowTitle, this->windowStyle);
    this->window->setFramerateLimit(frameLimit);
    this->window->setMouseCursorVisible(true);
    this->view = this->window->getDefaultView();
    sf::Image image;
    image.loadFromFile("textures/rune.png");
    this->window->setIcon(121,123,image.getPixelsPtr());
}

void Game::initFonts(){
    if(!this->font.loadFromFile("fonts/times.ttf")){
        std::cerr << "Failed to load fonts" << std::endl;
    }
}

void Game::initText(){
    //Clock Text
    this->clockText.setFont(font);
    this->clockText.setCharacterSize(42);
    this->clockText.setFillColor(sf::Color::Red);
    this->clockText.setStyle(sf::Text::Bold | sf::Text::Underlined);

    //Score Text
    this->scoreText.setFont(font);
    this->scoreText.setCharacterSize(42);
    this->scoreText.setFillColor(sf::Color::Red);
    this->scoreText.setStyle(sf::Text::Regular);
    this->scoreText.setPosition(sf::Vector2f(this->windowWidth - scoreText.getGlobalBounds().width*1.2,0));
}

void Game::updateText(){
    std::stringstream ss;
    ss << "Points: " << this->points;
    this->scoreText.setPosition(sf::Vector2f(this->windowWidth - scoreText.getGlobalBounds().width*1.2,0));
    this->scoreText.setString(ss.str());
    ss.str(std::string());
    ss << "Time: " << this->timeClock.getElapsedTime().asSeconds();
    this->clockText.setString(ss.str());
}

void Game::renderText(){
    this->window->draw(this->clockText);
    this->window->draw(this->scoreText);
    auto windh = this->window->getSystemHandle();
}

void Game::initEnemies(){
    
}

/*
    Creates enemy at random x value near top of window
*/
void Game::spawnEnemies(){
    this->enemy = std::make_shared<sf::RectangleShape>();
    std::shared_ptr<sf::RectangleShape> addEnemy = this->enemy;
    addEnemy->setPosition(sf::Vector2f(
        static_cast<float>(rand()%static_cast<int>(this->window->getSize().x - this->enemy->getSize().x)),
        0.f
        //static_cast<float>(rand()%static_cast<int>(this->window->getSize().y/20 - this->enemy->getSize().y))
    ));
    addEnemy->setSize(sf::Vector2f(10.f,10.f));
    this->enemyList.push_back(addEnemy);
}

/*
    Updates frame to frame
*/
void Game::updateEnemies(){
    if(true){
        this->enemySpawnTimer = this->enemySpawnClock.getElapsedTime();
        if(this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            this->enemySpawnClock.restart();
            this->spawnEnemies();
        }
    }
    for(auto i = 0; i < this->enemyList.size(); i++)
    {
        bool deleted = false;
        auto& e = this->enemyList[i];
        e->move(sf::Vector2f(0.f,3.f));

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if(e->getGlobalBounds().contains(this->mousePosView))
            {
                deleted = true;
                this->points += 10;
            }
        }

        if(e->getPosition().x < 0) e->move(sf::Vector2f(5.f,0.f));
        if(e->getPosition().x > this->windowWidth*0.99) e->move(sf::Vector2f(-5.f,0.f));

        if(e->getPosition().y > this->windowHeight*0.99){
            e->setPosition(sf::Vector2f(e->getPosition().x, this->windowHeight*0.99));
        }

        if(deleted == true)
        {
            this->enemyList.erase(this->enemyList.begin() + i);
        }
    }
}

void Game::renderEnemies(){
    for(auto &e : this->enemyList){
        this->window->draw(*e);
    }
}

void Game::renderCursor(){
    sf::Sprite sprite;
    sprite.setTexture(this->cursorTex);
    sprite.setTextureRect(sf::IntRect(0,0+343*aniFrame,431,343));
    sprite.scale(sf::Vector2f(0.35f,0.35f));
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    sprite.setPosition(sf::Vector2f(this->mousePosWindow.x - spriteBounds.width/2 ,this->mousePosWindow.y - spriteBounds.height/2));
    this->window->draw(sprite);
    this->aniFrame++;
    if(this->clock.getElapsedTime().asMilliseconds() > 24){
         this->aniFrame++; this->clock.restart();
         }
    if(this->aniFrame > 15) this->aniFrame = 0;  
}

void Game::renderSprites(){
    sf::Texture backTex;
    if(!backTex.loadFromFile("textures/wall.png")) return;
    backTex.setSmooth(true);
    backTex.setRepeated(true);
    sf::Sprite backSprite;
    backSprite.setTexture(backTex);
    backSprite.setTextureRect({0,0,this->windowWidth, this->windowHeight});
    this->window->draw(backSprite);

    sf::Texture logoTex;
    if(!logoTex.loadFromFile("textures/egypt_logo.png")) return;
    logoTex.setSmooth(true);
    sf::Sprite logoSprite;
    logoSprite.setTexture(logoTex);
    
    logoSprite.setPosition(sf::Vector2f(this->windowWidth/2 - logoTex.getSize().x/4 ,this->windowHeight/4 - logoTex.getSize().y/10));
    logoSprite.setScale(0.5f,0.5f);
    this->window->draw(logoSprite);

    sf::Texture brickTex;
    if(!brickTex.loadFromFile("textures/bricks.png")) return;
    brickTex.setSmooth(true);
    brickTex.setRepeated(true);
    sf::Sprite brickLeft;
    brickLeft.setTexture(brickTex);
    brickLeft.setTextureRect(sf::IntRect({0,0,90,this->windowHeight}));
    this->window->draw(brickLeft);
    
    sf::Sprite brickRight;
    brickRight.setTexture(brickTex);
    brickRight.setTextureRect(sf::IntRect({0,0,90,this->windowHeight}));
    brickRight.setOrigin({static_cast<float>(this->windowWidth),static_cast<float>(this->windowHeight)});
    brickRight.setRotation(180);
    this->window->draw(brickRight);

    this->renderCursor();
}

const bool Game::running() const{
    return this->window->isOpen();
}