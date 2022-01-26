#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include "ManBase.h"
#include "Image.h"

namespace Azul
{

class ImageManager : public ManBase
{
public:
	static void Create(int reserveNum = 0, int reserveGrow = 1);
	static void Destroy();
	//static Image *Add(Image *pImage);
	static Image *Add(Image::Name imageName, Texture::Name texName, Rect rec);
	static void Add(Image *pImage);
	static Image *Find(Image::Name name);
	
	static void Remove(Image *pNode);
	static void Dump();

private:
	static ImageManager *privGetInstance();

	ImageManager() = delete;
	ImageManager(const ImageManager &) = delete;
	ImageManager &operator=(const ImageManager &) = delete;
	virtual ~ImageManager();

	ImageManager(int reserveNum, int reserveGrow);

protected:
	DLink *derivedCreateNode() override;

private:
	Image *poNodeCompare;
	static ImageManager *posInstance;
};

}

#endif //IMAGE_MANAGER_H