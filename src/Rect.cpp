#include "Rect.h"

namespace Azul
{

Rect::Rect(float _x, float _y, float _width, float _height)
	: x(_x), y(_y), width(_width), height(_height)
{}

Rect::Rect()
	: x(0.f), y(0.f), width(0.f), height(0.f)
{}

Rect::Rect(const Rect &r)
{
	this->x = r.x;
	this->y = r.y;
	this->height = r.height;
	this->width = r.width;
}

Rect &Rect::operator=(const Rect &r)
{
	if(this != &r)
	{
		this->x = r.x;
		this->y = r.y;
		this->height = r.height;
		this->width = r.width;
	}

	return *this;
}

void Rect::Clear()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->height = 0.0f;
	this->width = 0.0f;
}

}