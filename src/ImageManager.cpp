#include "ImageManager.h"

namespace Azul
{
ImageManager *ImageManager::posInstance = nullptr;

ImageManager::ImageManager(int reserveNum, int reserveGrow)
	: ManBase(reserveGrow)
{
	this->proFillReservedPool(reserveNum);

	this->poNodeCompare = new Image();
}

ImageManager::~ImageManager()
{
	delete this->poNodeCompare;
	this->poNodeCompare = nullptr;

	Iterator *pIt = this->baseGetActiveIterator();

	DLink *pNode = pIt->First();

	while (!pIt->IsDone())
	{
		Image *pDeleteMe = (Image *)pIt->Curr();
		pNode = pIt->Next();
		delete pDeleteMe;
	}

	pIt = this->baseGetReserveIterator();

	pNode = pIt->First();

	while (!pIt->IsDone())
	{
		Image *pDeleteMe = (Image *)pIt->Curr();
		pNode = pIt->Next();
		delete pDeleteMe;
	}
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void ImageManager::Create(int reserveNum, int reserveGrow)
{
	// make sure values are ressonable
	assert(reserveNum >= 0);
	assert(reserveGrow > 0);

	// initialize the singleton here
	assert(posInstance == nullptr);

	// Do the initialization
	if (posInstance == nullptr)
	{
		posInstance = new ImageManager(reserveNum, reserveGrow);
	}
}

void ImageManager::Destroy()
{
	ImageManager *pMan = ImageManager::privGetInstance();
	assert(pMan != nullptr);
	AZUL_UNUSED_VAR(pMan);

	delete ImageManager::posInstance;
	ImageManager::posInstance = nullptr;
}

Image *ImageManager::Add(Image *pImage)
{
	ImageManager *pMan = ImageManager::privGetInstance();

	Image *pNode = (Image *)pMan->baseAddToFront();
	assert(pNode != nullptr);

	// Initialize the date
	pNode->Set(pImage->imageName, pImage->pText, pImage->imageRect);

	return pNode;
}

Image *ImageManager::Find(Image::Name _name)
{
    ImageManager *pMan = ImageManager::privGetInstance();
    assert(pMan != nullptr);

    // Compare functions only compares two Nodes

    // So:  Use the Compare Node - as a reference
    //      use in the Compare() function
    pMan->poNodeCompare->imageName = _name;

    Image *pData = (Image *)pMan->baseFind(pMan->poNodeCompare);
    return pData;
}


void ImageManager::Remove(Image *pNode)
{
    assert(pNode != nullptr);

    ImageManager *pMan = ImageManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseRemove(pNode);
}

void ImageManager::Dump()
{
    ImageManager *pMan = ImageManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseDump();
}

//Private Methods
ImageManager *ImageManager::privGetInstance()
{
	//Safety
	assert(posInstance = nullptr);

	return posInstance;
}

DLink *ImageManager::derivedCreateNode()
{
	DLink *pNodeBase = new Image();
	assert(pNodeBase != nullptr);

	return pNodeBase;
}

} //namespace Azul