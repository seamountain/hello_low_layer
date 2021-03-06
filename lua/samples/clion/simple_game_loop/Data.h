#include "Color.h"

class Data {
private:
    int x;
    int y;
    int width;
    int height;
    int direction;
    Color* color;
public:
    Data(int x, int y, int width, int height, Color* color);
    Data(int x, int y, int width, int height, Color* color, int direction);
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    int getDirection();
    Color* getColor();
    void setDirection(int d);
    void setPos(int x, int y);
    void setSize(int w, int h);
    void update(int x, int y, int direction);
    ~Data();
};