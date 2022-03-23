#include "Cell.h"

Cell::Cell() {

}

Cell::Cell(float X, float Y, Color color, int colorId) {
    this->color.first = color;
    this->color.second = colorId;
    Vector2f rectSize(95, 95);
    rect.setSize(rectSize);
    rect.setPosition(X,Y);
    rect.setFillColor(color);  
}

bool Cell::updateLocation(float& time, float& tempX, float& tempY) {
    float distance = 0;
    distance = sqrt((tempX - rect.getPosition().x) * (tempX - rect.getPosition().x) + (tempY - rect.getPosition().y) * (tempY - rect.getPosition().y));
    if (distance > 0.3) {
        rect.setPosition(rect.getPosition().x + (0.4 * time * (tempX - rect.getPosition().x) / distance), rect.getPosition().y + (0.4 * time * (tempY - rect.getPosition().y) / distance));
        return true;
    }
    else
        return false;
}

void Cell::updateColor() {
    rect.setFillColor(color.first);
}