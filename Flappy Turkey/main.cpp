// pumpkin column generator is faulty

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

    int turkScore = 0;
    sf::Font minecraftia_font("Minecraftia_Regular.ttf");
    sf::Text score(minecraftia_font);
    score.setString(std::to_string(turkScore));
    score.setCharacterSize(75);
    score.setOutlineThickness(2.f);
    score.setPosition(sf::Vector2f(400 - 20, 100));


    std::vector<sf::Sprite> resetPumpStack(int startingXPos);
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


    sf::Texture ground;
    if (!ground.loadFromFile("ground_2.png")) {
        return -1;
    }
    ground.setSmooth(true);
    sf::Sprite movingGround(ground);
    movingGround.setPosition({ 0,10 });
    movingGround.setScale({ 2.2,2.2 });
    std::vector<sf::Sprite> movingGrounds = { movingGround };

    if (!pumpkin.loadFromFile("pumpkin.png")) {
        return -1;
    }
    pumpkin.setSmooth(true);
    sf::Sprite obstacle(pumpkin);


    std::vector<sf::Sprite> obstacleStack = resetPumpStack(780);
    int stackLow = randomVal(1, 6);
    int stackHigh = stackLow + randomVal(3, 4);

  
    std::vector<sf::Sprite> tempObstacleStack = resetPumpStack(1250);
    int stackLowT = randomVal(1, 6);
    int stackHighT = stackLow + randomVal(3, 4);

    sf::Clock jumpTime;
    sf::Clock clock;
    sf::Clock pumpkinClock;
    sf::Clock tempPumpkinClock;

    float time;
    float obsticle_dt;
    float obsticleT_dt;

    float GRAVITY = 2000.f;
    float TURKEY_ACCEL = 0.f;
    float TURKEY_VEL = 0.f;
    float VEL_BOOST = -280.0f;

    const float MAX_VEL = 1200.f;
    const float JUMP_COOLDOWN = 0.f;

    float birdRotation = 0.f;

    bool turkContact = false;

    bool check = false;
    bool checkT = false;
    std::cout << "Press 'R' to reset when you lose" << std::endl;


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
        window.draw(score);
        
        time = clock.restart().asSeconds();  // time since last frame
        if (movingGrounds[movingGrounds.size()-1].getPosition().x < -80) {
            sf::Sprite tempGround(ground);
            tempGround.setPosition({ 1170,10 });
            tempGround.setScale({ 2.2, 2.2 });
            movingGrounds.push_back(tempGround);
        }
        for (auto& low : movingGrounds) {
            low.move({ -30 * time, 0 });
            window.draw(low);

        }
        if (movingGrounds.size() > 5) {
            movingGrounds.erase(movingGrounds.begin());
        }

        if (turkContact) { 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                turkScore = 0;
                score.setString(std::to_string(turkScore));


                obstacleStack = resetPumpStack(780);
                stackLow = randomVal(1, 6);
                stackHigh = stackLow + randomVal(3, 4);


                tempObstacleStack = resetPumpStack(1250);
                stackLowT = randomVal(1, 6);
                stackHighT = stackLow + randomVal(3, 4);

                jumpTime.restart();
                clock.restart();
                pumpkinClock.restart();
                tempPumpkinClock.restart();

                time = 0.f;
                obsticle_dt = 0.f;;
                obsticleT_dt = 0.f;;

                GRAVITY = 2000.f;
                TURKEY_ACCEL = 0.f;
                TURKEY_VEL = 0.f;
                VEL_BOOST = -280.0f;

                birdRotation = 0.f;
                check = false;
                checkT = false;
                movingGround.setPosition({ 0,10 });
                movingGround.setScale({ 2.2,2.2 });
                movingGrounds = { movingGround };
                turkSprite.setPosition({ 170,280 });
                turkContact = false;

            }
            window.display();
            continue;
        }



        obsticle_dt = pumpkinClock.restart().asSeconds();
        if (obstacleStack[0].getPosition().x <= -130) {
            obstacleStack = resetPumpStack(780);
            stackLow = randomVal(1, 6);
            stackHigh = stackLow + randomVal(3, 4);
            check = false;
        }

        if (!check && obstacleStack[0].getPosition().x <= 150) {
            turkScore++;
            score.setString(std::to_string(turkScore));
            check = true;
        }

        for (int p = 0; p < obstacleStack.size();p++) {
            if (p > stackHigh || p < stackLow) {
                if (turkSprite.getGlobalBounds().contains(obstacleStack[p].getGlobalBounds().getCenter())) {
                    turkContact = true;
                }
                obstacleStack[p].move({-195 * obsticle_dt, 0});
                window.draw(obstacleStack[p]);
            }
        }

        obsticleT_dt = tempPumpkinClock.restart().asSeconds();
        if (tempObstacleStack[0].getPosition().x <= -130) {
            tempObstacleStack = resetPumpStack(780);
            stackLowT = randomVal(1, 6);
            stackHighT = stackLowT + randomVal(3, 4);
            checkT = false;
        }
        if (!checkT && tempObstacleStack[0].getPosition().x <= 150) {
            turkScore++;
            score.setString(std::to_string(turkScore));
            checkT = true;

        }
        for (int o = 0; o < tempObstacleStack.size();o++) {
            if (o > stackHighT || o < stackLowT) {
                if (turkSprite.getGlobalBounds().contains(tempObstacleStack[o].getGlobalBounds().getCenter())) {
                    turkContact = true;
                }
                tempObstacleStack[o].move({ -195 * obsticleT_dt, 0 });
                window.draw(tempObstacleStack[o]);
            }
        }

        //time = clock.restart().asSeconds();  // time since last frame

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

        if (turkSprite.getPosition().y < 4) {
            turkSprite.setPosition(sf::Vector2f(170.f, 20));
        }
        if (turkSprite.getPosition().y > 800 - 60) {
            TURKEY_VEL = 0;
            turkSprite.setPosition(sf::Vector2f(170.f, 800 - 60));
            birdRotation = 0;
        }
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

std::vector<sf::Sprite> resetPumpStack(int startingXPos) {
    std::vector<sf::Sprite> obstacleStack;
    for (int i = 0; i < 12; i++) {
        sf::Sprite obstacle(pumpkin);
        obstacle.setPosition(sf::Vector2f(startingXPos, -50 + 70 * i));
        obstacle.setScale({ .2,.2 });
        obstacleStack.push_back(obstacle);

    }
    return obstacleStack;
}