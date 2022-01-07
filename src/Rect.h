#ifndef RECT_H
#define RECT_H

namespace Azul
{
struct Rect
{
	Rect(float x, float y, float width, float height);
	Rect();
	Rect(const Rect &);
	Rect &operator=(const Rect &);
	~Rect() = default;

	void Clear();

	float x;
	float y;
	float width;
	float height;
};
}

#endif