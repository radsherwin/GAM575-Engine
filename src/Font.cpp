#include "Font.h"
#include "StringThis.h"

Font::Font()
    : fontName(Name::NOT_INITIALIZED)
    
{
}

Font::~Font()
{
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