#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#pragma warning(disable : 4996)

using namespace sf;

class Cell {
	public:
		int x = 0;
		int y = 0;
		RectangleShape rect;
		bool isBlocked = false;
		std::pair <Color, int> color;

		Cell ();
		Cell (float x, float y, Color color, int colorId = 0);
		bool updateLocation(float& time, float& tempX, float& tempY);
		void updateColor();
};
