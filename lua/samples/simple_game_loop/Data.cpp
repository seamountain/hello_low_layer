#include "Data.h"

Data::Data(int x, int y, int width, int height, Color* color) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;

    this->direction = rand() % 4;
}

Data::Data(int x, int y, int width, int height, Color* color, int direction) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;

    this->direction = direction;
}

int Data::getX() {
    return this->x;
}

int Data::getY() {
    return this->y;
}

int Data::getWidth() {
    return this->width;
}

int Data::getHeight() {
    return this->height;
}

Color* Data::getColor() {
    return this->color;
}

int Data::getDirection() {
    return this->direction;
}

void Data::setDirection(int d) {
    this->direction = d;
}

void Data::setPos(int x, int y) {
    this->x = x;
    this->y = y;
}

void Data::setSize(int w, int h) {
    this->width = w;
    this->height = h;
}

void Data::update(int x, int y, int direction) {
    this->x = x;
    this->y = y;
    this->direction = direction;
}

Data::~Data() {
}