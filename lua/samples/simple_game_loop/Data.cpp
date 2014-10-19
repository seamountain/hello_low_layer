class Data {
private:
    int x;
    int y;
    int width;
    int height;
public:
    Data(int x, int y, int width, int height);
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    ~Data();
};

Data::Data(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
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

Data::~Data() {
}