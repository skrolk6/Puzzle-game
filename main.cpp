#include "Cell.h"
#include "GameSession.h"

int main() {
    int W = 1280;
    int H = 720;
    int fieldSize = 5;
    RenderWindow window(VideoMode(W,H), "Puzzle game");

    GameSession newGame(fieldSize, W, H);
    newGame.setObjects();
    newGame.updateProgress(window);

	return 0;
}
