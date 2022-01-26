#include "Glyph.h"
#include "StringThis.h"

Glyph::Glyph()
    : glyphName(Name::NOT_INITIALIZED),
    fontSize(0),
    pImage{ nullptr }
{
}

Glyph::~Glyph()
{
    delete[] pImage;
}

char *Glyph::GetName()
{
    static char pTmp[128];
    strcpy_s(pTmp, 128, StringMe(this->glyphName));
    return pTmp;
}

bool Glyph::Compare(DLink *pTarget)
{
    assert(pTarget != nullptr);

    Glyph *pDataB = (Glyph *)pTarget;

    bool status = false;

    if (this->glyphName == pDataB->glyphName)
    {
        status = true;
    }

    return status;
}

void Glyph::Dump()
{
    Trace::out("      Texture(%p)\n", this);

    // Data:
    Trace::out("      Name: %s \n", StringMe(this->glyphName));

    DLink::Dump();
}

void Glyph::Wash()
{
}