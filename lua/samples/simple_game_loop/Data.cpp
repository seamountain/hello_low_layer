using namespace std;

class Data {
private:
    int x;
    int y;
    int width;
    int height;
    Color* color;
public:
    Data(int x, int y, int width, int height, Color* color);
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    Color* getColor();
    ~Data();
};

Data::Data(int x, int y, int width, int height, Color* color) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;
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

Data::~Data() {
}