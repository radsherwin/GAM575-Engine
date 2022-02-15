#include "jointData.h"

jointData::jointData()
    : enabled(false),
    pName{ 0 },
    jointIndex(-1),
    parentIndex(-1)
{
}

jointData::jointData(const jointData &j)
    : enabled(j.enabled),
    jointIndex(j.jointIndex),
    parentIndex(j.parentIndex)
{
    memcpy_s(this->pName, jointData::FILE_NAME_SIZE, j.pName, jointData::FILE_NAME_SIZE);
}

jointData &jointData::operator=(const jointData &j)
{
    if (this != &j)
    {
        this->enabled = j.enabled;
        this->jointIndex = j.jointIndex;
        this->parentIndex = j.parentIndex;
        memcpy_s(this->pName, jointData::FILE_NAME_SIZE, j.pName, jointData::FILE_NAME_SIZE);
    }

    return *this;
}

jointData::~jointData()
{
}

void jointData::Serialize(jointData_proto &out) const
{
    std::string sName((const char *)this->pName, jointData::FILE_NAME_SIZE);
    out.set_pname(sName);

    out.set_enabled(enabled);
    out.set_jointindex(jointIndex);
    out.set_parentjointindex(parentIndex);
}

void jointData::Deserialize(const jointData_proto &in)
{
    this->enabled = in.enabled();
    memcpy_s(this->pName, jointData::FILE_NAME_SIZE, in.pname().data(), jointData::FILE_NAME_SIZE);

    this->jointIndex = in.jointindex();
    this->parentIndex = in.parentjointindex();
}

void jointData::Print(const char *const pName, int index) const
{
    Trace::out("\n\n--------Joint Data: %s, %d-------- \n", this->pName, index);
    Trace::out("Joint Index: %d\n", jointIndex);
    Trace::out("Parent Index: %d\n", parentIndex);
}