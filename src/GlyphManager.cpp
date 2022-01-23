#include "GlyphManager.h"
#include "File.h"
#include "protoData.h"

GlyphManager *GlyphManager::posInstance = nullptr;

GlyphManager::GlyphManager(int reserveNum, int reserveGrow) : ManBase(reserveGrow)
{
    this->proFillReservedPool(reserveNum);
    this->poNodeCompare = new Glyph();
}

GlyphManager::~GlyphManager()
{
    delete this->poNodeCompare;
    this->poNodeCompare = nullptr;

    // iterate through the list and delete
    Iterator *pIt = this->baseGetActiveIterator();

    DLink *pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        Glyph *pDeleteMe = (Glyph *)pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }

    pIt = this->baseGetReserveIterator();

    pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        Glyph *pDeleteMe = (Glyph *)pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void GlyphManager::Create(int reserveNum /* = 3 */, int reserveGrow /* = 1 */)
{
    assert(reserveNum > 0);
    assert(reserveGrow > 0);

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if (posInstance == nullptr)
    {
        posInstance = new GlyphManager(reserveNum, reserveGrow);
    }
}

void GlyphManager::Destroy()
{
    GlyphManager *pMan = GlyphManager::privGetInstance();
    assert(pMan != nullptr);
    AZUL_UNUSED_VAR(pMan);

    delete GlyphManager::posInstance;
    GlyphManager::posInstance = nullptr;
}

Glyph *GlyphManager::Add(const char *const pFilePath, Glyph::Name GlyphName)
{
    GlyphManager *pMan = GlyphManager::privGetInstance();

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

    Glyph *pNode = (Glyph *)pMan->baseAddToFront();
    pNode->glyphName = GlyphName;
    assert(pNode != nullptr);

    pMan->privLoadGlyph(pNode, pB);

    return pNode;
}

Glyph *GlyphManager::Find(Glyph::Name GlyphName)
{
    GlyphManager *pMan = GlyphManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->poNodeCompare->glyphName = GlyphName;

    Glyph *pData = (Glyph *)pMan->baseFind(pMan->poNodeCompare);

    return pData;
}

// Create 
Glyph *GlyphManager::Find(const char *const pText, Glyph::Name glyphName)
{
    return nullptr;
}

void GlyphManager::Remove(Glyph *pNode)
{
    assert(pNode != nullptr);

    GlyphManager *pMan = GlyphManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseRemove(pNode);
}

void GlyphManager::Dump()
{
    GlyphManager *pMan = GlyphManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseDump();
}

GlyphManager *GlyphManager::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

void GlyphManager::privLoadGlyph(Glyph *pGlyph, protoData &pD)
{
    unsigned int GlyphCount = pD.fontCount;
    pGlyph->fontSize = GlyphCount;

    // Fill sizes
    pGlyph->x = new unsigned int[GlyphCount];
    pGlyph->y = new unsigned int[GlyphCount];
    pGlyph->width = new unsigned int[GlyphCount];
    pGlyph->height = new unsigned int[GlyphCount];

    for (int i = 0; i < (int)pD.fontCount; i++)
    {
        fontData *fD = &pD.pFontData[i];

        pGlyph->x[i] = fD->x;
        pGlyph->y[i] = fD->y;
        pGlyph->width[i] = fD->width;
        pGlyph->height[i] = fD->height;
    }
}

DLink *GlyphManager::derivedCreateNode()
{
    DLink *pNodeBase = new Glyph();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}