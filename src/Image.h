#ifndef IMAGE_H
#define IMAGE_H

#include "Texture.h"
#include "DLink.h"
#include "Rect.h"

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
