#include "GameSession.h"

GameSession::GameSession(int fieldSize, int W, int H): fieldSize(fieldSize), W(W), H(H) {
    for (uint8_t i = 0; i < 3; i++)
    {
        RectangleShape rect;
        topColors.push_back(rect);
    }
    for (uint8_t i = 0; i < fieldSize; i++)
    {
        std::vector<Cell> tmp(fieldSize);
        gameField.push_back(tmp);
    }

    for (uint8_t i = 0; i < fieldSize; i++)
    {
        std::vector<Cell> tmp(fieldSize);
        gameCells.push_back(tmp);
    }
}

void GameSession::setObjects() {
    srand(time(NULL));
    int w = 100;
    Vector2f rectSize(95, 95);
    std::vector<Color> cellsColor{ Color::Yellow, Color::Color(252, 102, 0), Color::Red };
    int cellX = 0;
    int cellY = 0;
    for (uint8_t i = 0; i < 3; i++)
    {
        topColors[i].setSize(rectSize);
        topColors[i].setPosition(i * w * 2 + (W / 2 - fieldSize * w / 2), 5);
        topColors[i].setFillColor(cellsColor[i]);
    }
    for (uint8_t i = 0; i < fieldSize; i++)
    {
        for (uint8_t j = 0; j < fieldSize; j++)
        {
            if (i % 2 == 0 && j % 2 != 0)
            {
                Cell fieldCell(j * w + (W / 2 - fieldSize * w / 2), i * w + (H / 2 - fieldSize * w / 2), Color::Color(128, 128, 128));
                fieldCell.isBlocked = true;
                gameField[i][j] = fieldCell;
            }
            else
            {
                Cell fieldCell(j * w + (W / 2 - fieldSize * w / 2), i * w + (H / 2 - fieldSize * w / 2), Color::Black);
                gameField[i][j] = fieldCell;
            }
        }
    }
    for (uint8_t i = 0; i < 3; i++)
    {
        for (uint8_t j = 0; j < 5; j++)
        {
            do
            {
                cellX = rand() % 5;
                if (cellX % 2 != 0) {
                    rand() % 2 == 0 ? cellX -= 1 : cellX += 1;
                }
                cellY = rand() % 5;
            } while (gameField[cellY][cellX].isBlocked);
            Cell gameCell(cellX * w + (W / 2 - fieldSize * w / 2), cellY * w + (H / 2 - fieldSize * w / 2), cellsColor[i], i);
            gameCell.x = cellY;
            gameCell.y = cellX;
            gameCells[cellY][cellX] = gameCell;
            gameField[cellY][cellX].isBlocked = true;
        }
    }
}

void GameSession::updateProgress(RenderWindow& window) {
    Clock clock;
    bool isMove = false;
    bool isSelect = false;
    bool isWin = true;
    float tempX = 0;
    float tempY = 0;
    float distance = 0;
    int curI = 0;
    int curJ = 0;
    float time = 0.0;
    Vector2i pos;
    Event event;
    int srcI = 0;
    int srcJ = 0;
    int destI = 0;
    int destJ = 0;
    while (window.isOpen())
    {

        time = clock.getElapsedTime().asMicroseconds();

        clock.restart();
        time = time / 800;
        pos = Mouse::getPosition(window);
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == event.MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                for (uint8_t i = 0; i < fieldSize; i++)
                {
                    for (uint8_t j = 0; j < fieldSize; j++)
                    {
                        if (gameCells[i][j].rect.getGlobalBounds().contains(pos.x, pos.y))
                        {
                            for (uint8_t h = 0; h < fieldSize; h++)
                            {
                                for (uint8_t k = 0; k < fieldSize; k++)
                                {
                                    if (gameField[h][k].rect.getGlobalBounds().contains(pos.x, pos.y)) {
                                        srcI = h;
                                        srcJ = k;
                                    }
                                }
                            }
                            if (isSelect)
                            {
                                gameCells[curI][curJ].updateColor();
                            }
                            gameCells[i][j].rect.setFillColor(Color::Green);
                            isSelect = true;
                            curI = i;
                            curJ = j;
                        }
                    }
                }
            if (isSelect)
            {
                if (event.type == event.MouseButtonPressed && event.mouseButton.button == Mouse::Right)
                {
                    gameCells[curI][curJ].updateColor();
                    for (uint8_t i = 0; i < fieldSize; i++)
                    {
                        for (uint8_t j = 0; j < fieldSize; j++)
                        {
                            if (gameField[i][j].rect.getGlobalBounds().contains(pos.x, pos.y))
                            {
                                if (gameField[i][j].isBlocked || (srcI != i && srcJ != j) || abs(srcI - i) > 1 || abs(srcJ - j) > 1)
                                {
                                    isSelect = false;
                                    gameCells[curI][curJ].updateColor();
                                }
                                else
                                {
                                    gameField[srcI][srcJ].isBlocked = false;
                                    gameField[i][j].isBlocked = true;
                                    tempX = gameField[i][j].rect.getPosition().x;
                                    tempY = gameField[i][j].rect.getPosition().y;
                                    destI = i;
                                    destJ = j;
                                    isMove = true;
                                    isSelect = false;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (isMove)
        {
            if (!(isMove = gameCells[curI][curJ].updateLocation(time, tempX, tempY)))
            {
                gameCells[curI][curJ].x = destI;
                gameCells[curI][curJ].y = destJ;
            }
        }

        for (uint8_t i = 0; i < fieldSize; i++)
        {
            for (uint8_t j = 0; j < fieldSize; j += 2)
            {
                if (gameCells[i][j].color.second * 2 != gameCells[i][j].y)
                {
                    isWin = false;
                    break;
                }
            }
        }

        renderObjects(window);

        if (isWin)
        {
            std::cout << "Puzzle solved!" << std::endl;
            std::cout << "Restart to play again";
            sf::Time t = sf::seconds(5.f);
            sleep(t);
            window.close();
        }
        isWin = true;
    }
}

void GameSession::renderObjects(RenderWindow& window) {
    window.clear(Color::Color(220, 220, 220));
    for (uint8_t i = 0; i < 3; i++)
    {
        window.draw(topColors[i]);
    }
    for (uint8_t i = 0; i < fieldSize; i++)
    {
        for (uint8_t j = 0; j < fieldSize; j++)
        {
            window.draw(gameField[i][j].rect);
        }
    }
    for (uint8_t i = 0; i < fieldSize; i++)
    {
        for (uint8_t j = 0; j < fieldSize; j++)
        {
            window.draw(gameCells[i][j].rect);
        }
    }
    window.display();
}