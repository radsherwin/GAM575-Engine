#include "fontData.h"

fontData::fontData()
    : enabled(false),
    key(0),
    x(0),
    y(0),
    width(0),
    height(0)
{
}

fontData::fontData(const fontData &f)
    : enabled(f.enabled),
    key(f.key),
    x(f.x),
    y(f.y),
    width(f.width),
    height(f.height)
{
}

fontData &fontData::operator=(const fontData &f)
{
    if (this != &f)
    {
        this->enabled = f.enabled;
        this->key = f.key;
        this->x = f.x;
        this->y = f.y;
        this->width = f.width;
        this->height = f.height;
    }

    return *this;
}

void fontData::Serialize(fontData_proto &out) const
{
    out.set_enabled(this->enabled);
    out.set_key(this->key);
    out.set_x(this->x);
    out.set_y(this->y);
    out.set_width(this->width);
    out.set_height(this->height);
}

void fontData::Deserialize(const fontData_proto &in)
{
    this->enabled = in.enabled();
    this->key = in.key();
    this->x = in.x();
    this->y = in.y();
    this->width = in.width();
    this->height = in.height();
}

void fontData::Print(const char *const pName) const
{
}