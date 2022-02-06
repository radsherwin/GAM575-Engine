#include "FontManager.h"
#include "File.h"
#include "protoData.h"
#include "GlyphManager.h"

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

Font *FontManager::Add(const char *const pText, Glyph::Name glyphName,
                       Font::Name fontName, const float &xStart, const float &yStart)
{
    FontManager *pMan = FontManager::privGetInstance();

    // Create text from glyph

    Font *pNode = (Font *)pMan->baseAddToFront();
    pNode->fontName = fontName;
    assert(pNode != nullptr);

    pNode->Set(
        fontName,
        pText,
        glyphName,
        xStart,
        yStart
    );

    return pNode;
}

void FontManager::Draw()
{
    FontManager *pMan = FontManager::privGetInstance();
    assert(pMan != nullptr);
    // Loop through fonts and call Update on each of them
    Iterator *pIt = pMan->baseGetActiveIterator();

    DLink *pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        Font *pDrawMe = (Font *)pIt->Curr();
        pNode = pIt->Next();
    }
}

void FontManager::UpdateMessage(Font::Name fontName, std::string pText)
{
    FontManager *pMan = FontManager::privGetInstance();
    assert(pMan != nullptr);

    Font *pFont = Find(fontName);

    pFont->pText = pText;
}

Font *FontManager::Find(Font::Name fontName)
{
    FontManager *pMan = FontManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->poNodeCompare->fontName = fontName;

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

DLink *FontManager::derivedCreateNode()
{
    DLink *pNodeBase = new Font();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}