#include "Color.h"

// === Color Initializations
Color Colors::RED = Color(1.0, 0.0, 0.0, 1.0);
Color Colors::BLUE = Color(0.0, 0.0, 0.0, 1.0);
Color Colors::GREEN = Color(0.0, 1.0, 0.0, 1.0);
Color Colors::WHITE = Color(1.0, 1.0, 1.0, 1.0);
Color Colors::BLACK = Color(0.0, 0.0, 0.0, 1.0);

// ===== Constructor / Destructor ===== //
Color::Color(float _r, float _g, float _b, float _a)
{
	RGBA[0] = _r;
	RGBA[1] = _g;
	RGBA[2] = _b;
	RGBA[3] = _a;
}

Color::~Color()
{

}
// ==================================== //
