using namespace std;

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
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    int getDirection();
    Color* getColor();
    void setDirection(int d);
    void setPos(int x, int y);
    ~Data();
};

Data::Data(int x, int y, int width, int height, Color* color) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;

    this->direction = rand() % 4;
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

Data::~Data() {
}