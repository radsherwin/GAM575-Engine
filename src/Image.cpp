#include "Image.h"
#include "ImageManager.h"
#include "TextureManager.h"

namespace Azul
{
Image::Image()
{
	this->imageName = Image::Name::Not_Initialized;
	this->imageRect.Clear();
	this->pText = nullptr;
}

Image::Image(Image::Name _imageName, Texture::Name _texName, Rect _rect)
{
	this->imageName = _imageName;
	this->pText = TextureManager::Find(_texName);
	this->imageRect = _rect;
}

Image::Image(Image::Name _imageName, Texture *_pText, Rect _rect)
{
	this->imageName = _imageName;
	this->pText = _pText;
	this->imageRect = _rect;
}

void Image::Set(Image::Name _imageName, Texture *_pText, Rect _rect)
{
	this->imageName = _imageName;
	this->pText = _pText;
	this->imageRect = _rect;
}

void Image::Set(Image::Name _imageName, Texture::Name _texName, Rect _rect)
{
	this->imageName = _imageName;
	this->pText = TextureManager::Find(_texName);
	this->imageRect = _rect;
}

bool Image::Compare(DLink *pTarget)
{
	// This is used in ManBase.Find()
	assert(pTarget != nullptr);

	Image *pDataB = (Image *)pTarget;

	bool status = false;

	if (this->imageName == pDataB->imageName)
	{
		status = true;
	}

	return status;
}

void Image::Dump()
{
	Trace::out("      Image(%p)\n", this);

	// Data:
	//Trace::out("      Name: %s \n", StringMe(this->name));
	//Trace::out("         x: %d \n", this->x);

	DLink::Dump();
}

void Image::Wash()
{
	this->imageName = Image::Name::Not_Initialized;
	this->imageRect.Clear();
	this->pText = nullptr;
}
}