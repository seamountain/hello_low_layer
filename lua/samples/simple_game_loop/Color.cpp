#include "Palette.h"

class Color {
public:
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
    Color(int r, int g, int b, int a);
    Color(Palette selected);
private:
    void set_color(int r, int g, int b, int a);
};

Color::Color(int r, int g, int b, int a) {
    set_color(r, g, b, a);
}

Color::Color(Palette selected) {
    // uint8_t 0 - 255
    switch  (selected) {
        case Palette::Red:
            set_color(240, 70, 70, 255);
            break;
        case Palette::Green:
            set_color(30, 200, 30, 255);
            break;
        case Palette::Blue:
            set_color(50, 100, 200, 255);
            break;
        case Palette::Black:
            set_color(100, 100, 100, 255);
            break;
        case Palette::White:
            set_color(220, 220, 220, 255);
            break;
        default:
            set_color(0, 0, 0, 255); // default color
            break;
    }
}

void Color::set_color(int r, int g, int b, int a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}