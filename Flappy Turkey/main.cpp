#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>


sf::Texture pumpkin;

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Flappy Turkey");


    std::vector<sf::Sprite> resetPumpStack();
    int randomVal(int low, int high);


    sf::Texture turkey;
    if (!turkey.loadFromFile("turk.png")) {
        return -1;
    }
    turkey.setSmooth(true);
    sf::Sprite turkSprite(turkey);
    turkSprite.setPosition({ 170,280 });
    turkSprite.setScale({ .25,.25 });
    turkSprite.setOrigin({ 170 / 2.f, 280 / 2.f });


    sf::Texture bg;
    if (!bg.loadFromFile("background.png")) {
        return -1;
    }
    bg.setSmooth(true);
    sf::Sprite background(bg);
    background.setPosition({ 0,0 });
    background.setScale({ .8,.8 });


    if (!pumpkin.loadFromFile("pumpkin.png")) {
        return -1;
    }
    pumpkin.setSmooth(true);
    sf::Sprite obstacle(pumpkin);
    //obstacle.setPosition({ 0,0 });
    //obstacle.setScale({ .8,.8 });

    std::vector<sf::Sprite> obstacleStack = resetPumpStack();
    int stackLow = randomVal(1, 6);
    int stackHigh = stackLow + randomVal(3, 4);

  
    std::vector<sf::Sprite> tempObstacleStack;


    sf::Clock jumpTime;
    sf::Clock clock;
    sf::Clock pumpkinClock;

    float time;
    float obsticle_dt;

    float GRAVITY = 2000.f;
    float TURKEY_ACCEL = 0.f;
    float TURKEY_VEL = 0.f;
    float VEL_BOOST = -280.0f;

    const float MAX_VEL = 1200.f;
    const float JUMP_COOLDOWN = 0.f;

    float birdRotation = 0.f;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }


        window.clear(sf::Color::Black);
        window.draw(background);
        obsticle_dt = pumpkinClock.restart().asSeconds();
        if (obstacleStack[0].getPosition().x <= 0) {
            obstacleStack = resetPumpStack();
            resetPumpStack();
            stackLow = randomVal(1, 6);
            stackHigh = stackLow + randomVal(3, 4);
        }
        for (int p = 0; p < obstacleStack.size();p++) {
            if (p > stackHigh || p < stackLow) {
                obstacleStack[p].move({-195 * obsticle_dt, 0});
                window.draw(obstacleStack[p]);
            }
        }

        time = clock.restart().asSeconds();  // time since last frame

        if (jumpTime.getElapsedTime().asSeconds() >= JUMP_COOLDOWN && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            TURKEY_ACCEL = VEL_BOOST / time; // a = delta v / delta t
            TURKEY_VEL = VEL_BOOST + TURKEY_ACCEL * time; // // v = v + at
            birdRotation += 10;
            jumpTime.restart();
        }
        else {
            TURKEY_ACCEL = GRAVITY; 
            TURKEY_VEL = TURKEY_VEL + TURKEY_ACCEL * time; // v = v + at
            birdRotation -= 0.04f;
        }
        
        if (TURKEY_VEL > MAX_VEL) TURKEY_VEL = MAX_VEL;
        if (TURKEY_VEL < -MAX_VEL) TURKEY_VEL = -MAX_VEL;

        if (birdRotation < -55) birdRotation = -55;
        if (birdRotation > 45) birdRotation = 45;
        turkSprite.setRotation(sf::degrees(-birdRotation));

        turkSprite.setPosition(sf::Vector2f( 170.f, turkSprite.getPosition().y + TURKEY_VEL * time + .5*TURKEY_ACCEL*(time*time))); // x = x + v + .5at^2
        window.draw(turkSprite);
        // end the current frame
        window.display();
    }
}


int randomVal(int low, int high) {
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(low, high);
    return distrib(gen);
}

std::vector<sf::Sprite> resetPumpStack() {
    std::vector<sf::Sprite> obstacleStack;
    for (int i = 0; i < 12; i++) {
        sf::Sprite obstacle(pumpkin);
        obstacle.setPosition(sf::Vector2f(700, -50 + 70 * i));
        obstacle.setScale({ .2,.2 });
        obstacleStack.push_back(obstacle);

    }
    return obstacleStack;
}