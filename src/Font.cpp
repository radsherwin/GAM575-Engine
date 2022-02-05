#include "Font.h"
#include "StringThis.h"
#include "GlyphManager.h"

#include "MathEngine.h"

Font::Font()
    : fontName(Name::NOT_INITIALIZED),
    pText{ nullptr }

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
    this->startX = xStart;
    this->startY = yStart;
    this->glyphName = _glyphName;
    this->pText = new char[_textLength + 1];
    memcpy(pText, pMessage, _textLength + 1);

    this->pTexture = GlyphManager::Find(_glyphName)->pTexture;
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