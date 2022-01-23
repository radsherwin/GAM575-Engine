#include "FontManager.h"
#include "File.h"
#include "protoData.h"

FontManager *FontManager::posInstance = nullptr;

FontManager::FontManager(int reserveNum, int reserveGrow) : ManBase(reserveGrow)
{
    this->proFillReservedPool(reserveNum);
    this->poNodeCompare = new Font();
}

FontManager::~FontManager()
{
    delete this->poNodeCompare;
    this->poNodeCompare = nullptr;

    // iterate through the list and delete
    Iterator *pIt = this->baseGetActiveIterator();

    DLink *pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        Font *pDeleteMe = (Font *)pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }

    pIt = this->baseGetReserveIterator();

    pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        Font *pDeleteMe = (Font *)pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void FontManager::Create(int reserveNum /* = 3 */, int reserveGrow /* = 1 */)
{
    assert(reserveNum > 0);
    assert(reserveGrow > 0);

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if (posInstance == nullptr)
    {
        posInstance = new FontManager(reserveNum, reserveGrow);
    }
}

void FontManager::Destroy()
{
    FontManager *pMan = FontManager::privGetInstance();
    assert(pMan != nullptr);
    AZUL_UNUSED_VAR(pMan);

    delete FontManager::posInstance;
    FontManager::posInstance = nullptr;
}

Font *FontManager::Add(const char *const pFilePath, Font::Name fontName)
{
    FontManager *pMan = FontManager::privGetInstance();

    //Read and recreate model data
    Azul::File::Handle fh;
    Azul::File::Error err;
    assert(pFilePath);
    err = Azul::File::Open(fh, pFilePath, Azul::File::Mode::READ);
    assert(err == Azul::File::Error::SUCCESS);
    err = Azul::File::Seek(fh, Azul::File::Position::END, 0);
    assert(err == Azul::File::Error::SUCCESS);
    DWORD FileLength;
    err = Azul::File::Tell(fh, FileLength);
    assert(err == Azul::File::Error::SUCCESS);
    char *poNewBuff = new char[FileLength]();
    assert(poNewBuff);
    err = Azul::File::Seek(fh, Azul::File::Position::BEGIN, 0);
    assert(err == Azul::File::Error::SUCCESS);
    err = Azul::File::Read(fh, poNewBuff, FileLength);
    assert(err == Azul::File::Error::SUCCESS);
    err = Azul::File::Close(fh);
    assert(err == Azul::File::Error::SUCCESS);

    std::string strIn(poNewBuff, FileLength);
    protoData_proto pB_proto;
    pB_proto.ParseFromString(strIn);
    protoData pB;
    pB.Deserialize(pB_proto);

    delete[] poNewBuff;

    Font *pNode = (Font *)pMan->baseAddToFront();
    pNode->name = fontName;
    assert(pNode != nullptr);

    pMan->privLoadFont(pNode, pB);

    return pNode;
}

Font *FontManager::Find(Font::Name fontName)
{
    FontManager *pMan = FontManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->poNodeCompare->name = fontName;

    Font *pData = (Font *)pMan->baseFind(pMan->poNodeCompare);

    return pData;
}

void FontManager::Remove(Font *pNode)
{
    assert(pNode != nullptr);

    FontManager *pMan = FontManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseRemove(pNode);
}

void FontManager::Dump()
{
    FontManager *pMan = FontManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseDump();
}

FontManager *FontManager::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

void FontManager::privLoadFont(Font *pFont, protoData &pD)
{
    unsigned int fontCount = pD.fontCount;
    pFont->fontSize = fontCount;

    // Fill sizes
    pFont->x = new unsigned int[fontCount];
    pFont->y = new unsigned int[fontCount];
    pFont->width = new unsigned int[fontCount];
    pFont->height = new unsigned int[fontCount];

    for (int i = 0; i < (int)pD.fontCount; i++)
    {
        fontData *fD = &pD.pFontData[i];

        pFont->x[i] = fD->x;
        pFont->y[i] = fD->y;
        pFont->width[i] = fD->width;
        pFont->height[i] = fD->height;
    }
}

DLink *FontManager::derivedCreateNode()
{
    DLink *pNodeBase = new Font();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}