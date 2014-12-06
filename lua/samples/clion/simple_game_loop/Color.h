#include <SDL_stdinc.h>

enum class Palette {
    Red,
    Green,
    Blue,
    Black,
    White,
    SIZE
};

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