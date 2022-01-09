#ifndef IMAGE_2D_H
#define IMAGE_2D_H

#include "Rect.h"
#include "Texture.h"

namespace Azul
{

class Image : public DLink
{
public:
	enum Name
	{
		Alien_Blue,
		Alien_Green,
		Alien_Red,
		Stitch,
		Not_Initialized
	};

public:
	Image();
	Image(Image::Name imageName, Texture::Name texName, Rect rect);
	Image(Image::Name imageName, Texture *pTexture, Rect rect);
	virtual ~Image() override;

	void Set(Image::Name imageName, Texture::Name texName, Rect rect);
	void Set(Image::Name imageName, Texture *pTexture, Rect rect);

	virtual bool Compare(DLink *pTarget) override;
	virtual void Dump() override;
	virtual void Wash() override;

public:
	Image::Name	imageName;
	Texture		*pText;
	Rect		imageRect;

};

}

#endif
