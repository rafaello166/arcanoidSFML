#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

#include "Ball.h"
#include "Paddle.h"
#include "Block.h"
#include "Menu.h"
#include <sstream>      /* stringstream*/

/*
 * Created by Rafaello166
 * https://github.com/rafaello166
*/

using namespace std;
using namespace sf;

// initial variables
enum STATES {
    MAIN_MENU, GAMEPLAY, WINNER, PAUSE, GAME_OVER, HOW_TO_PLAY, BEST_SCORES
};

char GAME_STATE = STATES::MAIN_MENU; // initial state

unsigned short int windowWidth{1024}, windowHeight{768}, frameLimit{60};

unsigned blocksX{10}, blocksY{6}, blockWidth{80}, blockHeight{30};

// functions
template<class T1, class T2> bool isIntersecting(T1 &A, T2 &B) { // collision test, intersection of two elements
    return A.right() >= B.left() && A.left() <= B.right()
           && A.bottom() >= B.top() && A.top() <= B.bottom();
}

bool collistionTest(Paddle &paddle, Ball &ball) { // collision test, paddle & ball
    if (!isIntersecting(paddle, ball)) return false;

    ball.moveUp();

    if (ball.getPosition().x < paddle.getPosition().x) {
        ball.moveLeft();
    }
    else if (ball.getPosition().x > paddle.getPosition().x) {
        ball.moveRight();
    }
    return true;
}

bool collistionTest(Block &block, Ball &ball) { // collision test, block & ball
    if (!isIntersecting(block, ball)) return false;

    block.destroy();

    float overlapLeft{ball.right() - block.left()};
    float overlapRight{block.right() - ball.left()};

    float overlapTop{ball.bottom() - block.top()};
    float overlapBottom{block.bottom() - ball.top()};

    bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
    bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

    float minOverlapX{ballFromLeft ? overlapLeft : overlapRight};
    float minOverlapY{ballFromTop ? overlapTop : overlapBottom};

    if (abs(minOverlapX) < abs(minOverlapY)) {
        ballFromLeft ? ball.moveLeft() : ball.moveRight();
    } else {
        ballFromTop ? ball.moveUp() : ball.moveDown();
    }

    return true;
}

void clearGame(vector<Block> &blocks, Ball &ball) { // SET ALL BLOCKS & BALL
    blocks.clear();
    for (int i = 1; i < blocksY; i++) { // iterate backward from 1 to have a greater distance from the top
        for (int j = 0; j < blocksX; j++)
            blocks.emplace_back((j + 1) * (blockWidth + 13), (i + 1) * (blockHeight + 5), blockWidth, blockHeight);
    }
    ball.updatePosition(400, 400);
}

void saveResults(float timeRun) {
    // data reading
    ifstream file("results.txt");

    int number_of_lines{};
    vector<vector<string>> dane;

    string stream;
    if (file.good()) {
        while (std::getline(file, stream))
            ++number_of_lines;

        file.clear(); // to help seekg to set on first character of file
        file.seekg(0);

        for (int j = 0; j < number_of_lines; j++) {
            dane.emplace_back();
            for (int i = 0; i < 4; i++) {
                file >> stream;
                dane[dane.size() - 1].emplace_back(stream);
            }
        }
    }
    file.close();

    // current time
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::stringstream dateStream;
    std::stringstream timeStream;
    dateStream << std::put_time(&tm, "%d-%m-%Y");
    timeStream << std::put_time(&tm, "%H:%M:%S");
    std::string date = dateStream.str();
    std::string time = timeStream.str();

    // adding current result & time to vector
    dane.emplace_back();
    dane[dane.size() - 1].emplace_back(to_string(number_of_lines));
    dane[dane.size() - 1].emplace_back(to_string(timeRun));
    dane[dane.size() - 1].emplace_back(date);
    dane[dane.size() - 1].emplace_back(time);

    // vector sorting by first column -> dane[x][1]
    for (int k = 0; k < (number_of_lines + 1); k++) {
        for (int j = 0; j < (number_of_lines + 1); j++) {
            if (dane[k][1] < dane[j][1])
                for (int i = 0; i < 4; i++) {
                    std::swap(dane[k][i], dane[j][i]);
                }
        }
    }

    // data saving
    ofstream output("results.txt");
    for (int j = 0; j < number_of_lines + 1; j++) {
        if (j <= 9) {
            for (int i = 0; i < 4; i++) {
                output << dane[j][i] << " ";
            }
            output << endl;
        }
    }

}

void getResult(sf::Text resultText[]) {
    fstream file("results.txt", std::ios::in);
    std::string strings[9][4];

    if (file.good()) {
        int amount_of_data{};

        string stream;
            while (file >> stream)
                ++amount_of_data;

        file.clear();
        file.seekg(0);

        if(!(amount_of_data % 4)) { // simple test to check the amount of input data
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 4; j++) {
                    file >> strings[i][j];
                }

                std::stringstream ss;
                if (!strings[i][0].empty()) {
                    ss << "\n" << std::setw(2) << i + 1 << ".\t\t\t"
                        << std::fixed << setprecision(2) << stof(strings[i][1])
                        << "\t\t" << strings[i][2] << "\t\t" << strings[i][3];

                    resultText[i].setString(ss.str());
                }
            }
        }
    }
    file.close();
}


int main() {
    sf::RenderWindow window{sf::VideoMode(windowWidth, windowHeight), L"Arcanoid - Rafał Ochorok",
                            sf::Style::Titlebar | sf::Style::Close};
    window.setFramerateLimit(frameLimit);

    sf::Clock Clock;// time measurement

    Menu menu(windowWidth, windowHeight);


    // SET BACKGROUNDs
    // MENU BACKGROUND
    string imagesPath = "../Images/";
    string photoLocation = imagesPath + "menu_background.png";
    sf::Sprite background;
    sf::Sprite backgroundAlfa; // it creates alfa efect - menu
    sf::Texture backgroundTexture;

    if (!backgroundTexture.loadFromFile(photoLocation)) {
        cout << "Nie znaleziono ścieżki: " << photoLocation << endl;
    }

    background.setTexture(backgroundTexture);
    background.setColor(sf::Color(255, 255, 255, 255));
//    background.setScale(0.45,0.45);
//    background.setPosition(100, 25);

    backgroundAlfa.setTexture(backgroundTexture);
    backgroundAlfa.setColor(sf::Color(255, 255, 255, 50));
    // MENU BACKGROUND


    // GAME PLAY BACKGROUND
    photoLocation = imagesPath + "game_play.png";
    sf::Sprite gamePlayBackground;
    sf::Sprite gamePlayBackgroundAlfa; // it creates alfa efect - gameplay
    sf::Texture gamePlayBackgroundTexture;

    if (!gamePlayBackgroundTexture.loadFromFile(photoLocation)) {
        cout << "Nie znaleziono ścieżki: " << photoLocation << endl;
    }

    gamePlayBackground.setTexture(gamePlayBackgroundTexture);
    gamePlayBackground.setColor(sf::Color(255, 255, 255, 255));
    gamePlayBackgroundAlfa.setTexture(gamePlayBackgroundTexture);
    gamePlayBackgroundAlfa.setColor(sf::Color(255, 255, 255, 40));
    // GAME PLAY BACKGROUND


    // PAUSE BACKGROUND
    photoLocation = imagesPath + "pause.png";
    sf::Sprite pauseBackground;
    sf::Texture pauseBackgroundTexture;

    if (!pauseBackgroundTexture.loadFromFile(photoLocation)) {
        cout << "Nie znaleziono ścieżki: " << photoLocation << endl;
    }

    pauseBackground.setTexture(pauseBackgroundTexture);
    pauseBackground.setColor(sf::Color(255, 255, 255, 255));
    // PAUSE BACKGROUND


    // RANKING BACKGROUND
    photoLocation = imagesPath + "ranking.png";
    sf::Sprite rankingBackground;
    sf::Texture rankingBackgroundTexture;

    if (!rankingBackgroundTexture.loadFromFile(photoLocation)) {
        cout << "Nie znaleziono ścieżki: " << photoLocation << endl;
    }

    rankingBackground.setTexture(rankingBackgroundTexture);
    rankingBackground.setColor(sf::Color(255, 255, 255, 255));
    // RANKING BACKGROUND


    // INSTRUCTION BACKGROUND
    photoLocation = imagesPath + "instruction.png";
    sf::Sprite instructionBackground;
    sf::Texture instructionBackgroundTexture;

    if (!instructionBackgroundTexture.loadFromFile(photoLocation)) {
        cout << "Nie znaleziono ścieżki: " << photoLocation << endl;
    }

    instructionBackground.setTexture(instructionBackgroundTexture);
    instructionBackground.setColor(sf::Color(255, 255, 255, 255));
    // INSTRUCTION BACKGROUND


    // WINNER BACKGROUND
    photoLocation = imagesPath + "winner.png";
    sf::Sprite winnerBackground;
    sf::Texture winnerBackgroundTexture;

    if (!winnerBackgroundTexture.loadFromFile(photoLocation)) {
        cout << "Nie znaleziono ścieżki: " << photoLocation << endl;
    }

    winnerBackground.setTexture(winnerBackgroundTexture);
    winnerBackground.setColor(sf::Color(255, 255, 255, 255));
    // WINNER BACKGROUND


    // GAME OVER BACKGROUND
    photoLocation = imagesPath + "game_over.png";
    sf::Sprite gameOverBackground;
    sf::Texture gameOverBackgroundTexture;

    if (!gameOverBackgroundTexture.loadFromFile(photoLocation)) {
        cout << "Nie znaleziono ścieżki: " << photoLocation << endl;
    }

    gameOverBackground.setTexture(gameOverBackgroundTexture);
    gameOverBackground.setColor(sf::Color(255, 255, 255, 255));
    // GAME OVER BACKGROUND

    sf::Text resultsText[10]; // ranking data

    // SET FONT
    // RANKING PAGE
    sf::Font font;
    string file = "../Fonts/luckiestguy.ttf";
    if (!font.loadFromFile(file)) {
        cout << "Nie znaleziono: " << file << endl;
    }

    for (int i = 0; i < 10; i++) {
        resultsText[i].setFont(font);
        resultsText[i].setPosition({windowWidth / 5.0f, windowHeight / 5.0f + 50 * static_cast<float>(i)});
        resultsText[i].setCharacterSize(40);
    }

    // BALL & PADDLE
    Ball ball(400, 300);
    Paddle paddle(windowWidth / 2.0f, windowHeight - 20.0f);

    vector<Block> blocks;
    clearGame(blocks, ball); // SET ALL BLOCKS TO DEFAULT

    while (window.isOpen()) {
        sf::Event event;

        window.pollEvent(event);
        if (event.type == sf::Event::Closed)
            window.close();

        window.clear(sf::Color::Black);

        switch (GAME_STATE) {
            case MAIN_MENU:
                if (event.type == sf::Event::KeyPressed) {
                    sf::sleep(sf::milliseconds(150));

                    if (event.key.code == sf::Keyboard::Up) {
                        menu.MoveUp();
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        menu.MoveDown();
                    }

                    //SET STATE
                    if (event.key.code == sf::Keyboard::Return) //enter
                    {
                        if (menu.GetPressedItem() == 0) // Graj!
                        {
                            GAME_STATE = STATES::GAMEPLAY;
                            Clock.restart(); // starts measuring time
                        }
                        if (menu.GetPressedItem() == 1) // Ranking
                        {
                            getResult(resultsText);
                            GAME_STATE = STATES::BEST_SCORES;
                        }
                        if (menu.GetPressedItem() == 2) // Instrukcja
                        {
                            GAME_STATE = STATES::HOW_TO_PLAY;
                        }
                        if (menu.GetPressedItem() == 3) // Wyjście
                        {
                            window.close();
                        }
                    }
                }

                window.draw(background); // background
                menu.draw(window);
                window.draw(backgroundAlfa); // backgroundAlfa on menu

                break;

            case GAMEPLAY:
                window.draw(gamePlayBackground); // background

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                    GAME_STATE = STATES::PAUSE;
                    sf::sleep(sf::milliseconds(150));
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                    clearGame(blocks, ball); // SET ALL BLOCKS
                    GAME_STATE = STATES::MAIN_MENU;
                    sf::sleep(sf::milliseconds(150));
                }

                // Code block due to initialization of the visibleBlocksNumber variable
                {
                    if (!ball.update(windowWidth, windowHeight)) { // !(GAME IS STILL RUNNING), bottom collision returns false
                        GAME_STATE = STATES::GAME_OVER;
                    }
                    paddle.update(windowWidth);
                    collistionTest(paddle, ball);

                    unsigned int visibleBlocksNumber{}; // The amount of visible blocks
                    for (auto &block : blocks) {
                        visibleBlocksNumber++;
                        if (collistionTest(block, ball)) break;
                    }

                    auto iterator = remove_if(begin(blocks), end(blocks),
                                              [](Block &block) { return block.isDestroyed(); });
                    blocks.erase(iterator, end(blocks));


                    if (!visibleBlocksNumber) { // there are no blocks
                        sf::Time second = Clock.getElapsedTime();
                        saveResults(second.asSeconds());

                        GAME_STATE = STATES::WINNER;
                    }
                }


                window.draw(ball);
                window.draw(paddle);

                for (auto &block : blocks)
                    window.draw(block);

                window.draw(gamePlayBackgroundAlfa); // background

                break;

            case GAME_OVER:
                clearGame(blocks, ball); // SET ALL BLOCKS

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                    GAME_STATE = STATES::MAIN_MENU;
                    sf::sleep(sf::milliseconds(150)); // sleep for solve problems with long pressing Enter
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                    GAME_STATE = STATES::MAIN_MENU;
                }

                window.draw(gameOverBackground); // background

                break;

            case WINNER:
                clearGame(blocks, ball); // SET ALL BLOCKS

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                    GAME_STATE = STATES::MAIN_MENU;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                    GAME_STATE = STATES::MAIN_MENU;
                }

                window.draw(winnerBackground); // background

                break;

            case HOW_TO_PLAY:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                    sf::sleep(sf::milliseconds(150));
                    GAME_STATE = STATES::MAIN_MENU;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                    GAME_STATE = STATES::MAIN_MENU;
                }

                window.draw(instructionBackground); // background

                break;

            case BEST_SCORES:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                    sf::sleep(sf::milliseconds(150));
                    GAME_STATE = STATES::MAIN_MENU;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                    GAME_STATE = STATES::MAIN_MENU;
                }

                window.draw(rankingBackground); // background

                for (int i = 0; i < 10; i++) {
                    window.draw(resultsText[i]);
                }

                break;

            case PAUSE:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                    GAME_STATE = STATES::GAMEPLAY;
                    sf::sleep(sf::milliseconds(150)); // sleep for solve problems with long pressing Escape
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                    GAME_STATE = STATES::MAIN_MENU;
                }

                window.draw(pauseBackground); // background

                break;
        }

        window.display();

    }

    return 0;
}