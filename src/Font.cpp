#include "Font.h"
#include "StringThis.h"
#include "GlyphManager.h"

Font::Font()
    : fontName(Name::NOT_INITIALIZED),
    pText{nullptr}

{
}

Font::~Font()
{
    delete[] pText;
}

void Font::Set(Name _name,
               const char *const pMessage,
               const unsigned int &_textLength,
               Glyph::Name _glyphName,
               const float &xStart,
               const float &yStart)
{
    // Create images for each letter in the Glyph;

    // Loop through pMessage to get each char
    //      For each char, convert to ascii, ascess _glyph[ascii-startingOffset] to get the letter
    this->fontName = _name;
    this->textLength = _textLength;
    this->startX = startX;
    this->startY = startY;
    this->glyphName = _glyphName;
    this->pText = new char[_textLength+1];
    memcpy(pText, pMessage, _textLength+1);

    this->pTexture = GlyphManager::Find(_glyphName)->pTexture;
}

void Font::Render()
{
    float xTmp = this->startX;
    float yTmp = this->startY;

    float xEnd = this->startX;

    for (int i = 0; i < (int)textLength; i++)
    {
        int key(pText[i]); // gets ascii

        Glyph *pGlyph = GlyphManager::Find(this->glyphName); // Hack

        key -= pGlyph->startingOffset; // set the array offset

        Rect *pRect = &pGlyph->pImage[key].imageRect;

        xTmp = xEnd + ((float)pRect->width / 2.0f);
        this->rect = Rect(xTmp, yTmp, pRect->width, pRect->height);

        // Call some render function

        // Move the start to the next character
        xEnd = ((float)pRect->width / 2.0f) + xTmp;
    }
}

char *Font::GetName()
{
    static char pTmp[128];
    strcpy_s(pTmp, 128, this->pText);
    return pTmp;
}

bool Font::Compare(DLink *pTarget)
{
    assert(pTarget != nullptr);

    Font *pDataB = (Font *)pTarget;

    bool status = false;

    if (this->fontName == pDataB->fontName)
    {
        status = true;
    }

    return status;
}

void Font::Dump()
{
    Trace::out("      Texture(%p)\n", this);

    // Data:
    Trace::out("      Name: %s \n", this->pText);

    DLink::Dump();
}

void Font::Wash()
{
}