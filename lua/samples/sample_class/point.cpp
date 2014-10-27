class Point {
private:
    int x;
    int y;
public:
    Point();
    ~Point();
    void set_value(int x, int y);
    void print_value();
};

Point::Point() {
    this->x = 0;
    this->y = 0;
}

Point::~Point() {

}

void Point::set_value(int x, int y) {
    this->x = x;
    this->y = y;
}

void Point::print_value() {
    printf("x: %i, y: %i\n", this->x, this->y);
}