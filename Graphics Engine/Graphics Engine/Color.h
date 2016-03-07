#pragma once
class Color
{
public:
	// === Constructor / Destructor === //
	Color(float _r, float _g, float _b, float _a);
	~Color();
	// ================================ //

	// === Member
	float RGBA[4];
};

class Colors
{
public:
	// === Static Colors
	static Color RED;
	static Color BLUE;
	static Color GREEN;
	static Color WHITE;
	static Color BLACK;
};

