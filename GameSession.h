#pragma once
#include "Cell.h"

class GameSession {
	public:
        int W = 0;
        int H = 0;
        int fieldSize = 0;
        std::string windowName;
        std::vector<RectangleShape> topColors;
        std::vector<std::vector<Cell>> gameField;
        std::vector<std::vector<Cell>> gameCells;

        GameSession(int fieldSize, int W, int H);
        void setObjects();
        void updateProgress(RenderWindow& window);

    private:
        void renderObjects(RenderWindow &window);
};