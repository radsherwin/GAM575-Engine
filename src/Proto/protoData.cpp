#include "protoData.h"

protoData::protoData()
    : pVersion{ 0 }, pName{0}, meshCount(0), textureCount(0),
    animCount(0), pMeshData(nullptr), pAnimData(nullptr),
    pTextureData(nullptr)
{
    strcpy_s(this->pVersion, protoData::VERSION_NUM_BYTES, PROTO_VERSION);
    assert(strlen(this->pVersion) < VERSION_NUM_BYTES);
}

protoData::~protoData()
{
    delete[] this->pMeshData;
    delete[] this->pAnimData;
    delete[] this->pTextureData;
}

void protoData::Serialize(protoData_proto &out) const
{
    std::string sVersion((const char *)this->pVersion, VERSION_NUM_BYTES);
    out.set_pversion(sVersion);

    std::string sName((const char *)this->pName, FILE_NAME_SIZE);
    out.set_pname(sName);

    out.set_meshcount(this->meshCount);
    out.set_animcount(this->animCount);
    out.set_texturecount(this->textureCount);

    for (int i = 0; i < this->meshCount; i++)
    {
        meshData_proto *pMesh = new meshData_proto();
        this->pMeshData[i].Serialize(*pMesh);
        out.mutable_pmeshdata()->AddAllocated(pMesh);
    }

    for (int i = 0; i < this->animCount; i++)
    {
        animData_proto *pAnim = new animData_proto();
        this->pAnimData[i].Serialize(*pAnim);
        out.mutable_panimdata()->AddAllocated(pAnim);
    }

    for (int i = 0; i < this->textureCount; i++)
    {
        textureData_proto *pTexture = new textureData_proto();
        this->pTextureData[i].Serialize(*pTexture);
        out.mutable_ptexturedata()->AddAllocated(pTexture);
    }
}

void protoData::Deserialize(const protoData_proto &in)
{
    // CHECK the Version
    memcpy_s(this->pVersion, VERSION_NUM_BYTES, in.pversion().data(), VERSION_NUM_BYTES);
    assert(strcmp(this->pVersion, PROTO_VERSION) == 0);

    memcpy_s(this->pName, FILE_NAME_SIZE, in.pname().data(), FILE_NAME_SIZE);

    this->meshCount = in.meshcount();
    this->animCount = in.animcount();
    this->textureCount = in.texturecount();

    if (this->meshCount == 0)
    {
        this->pMeshData = nullptr;
    }
    else
    {
        this->pMeshData = new meshData[this->meshCount];
        for(int i = 0; i < this->meshCount; i++)
        {
            this->pMeshData[i].Deserialize(in.pmeshdata(i));
        }
    }

    if (this->animCount == 0)
    {
        this->pAnimData = nullptr;
    }
    else
    {
        this->pAnimData = new animData[this->animCount];
        for (int i = 0; i < this->animCount; i++)
        {
            this->pAnimData[i].Deserialize(in.panimdata(i));
        }
    }

    if (this->textureCount == 0)
    {
        this->pTextureData = nullptr;
    }
    else
    {
        this->pTextureData = new textureData[this->textureCount];
        for (int i = 0; i < this->textureCount; i++)
        {
            this->pTextureData[i].Deserialize(in.ptexturedata(i));
        }
    }
}

void protoData::Print(const char *const _pName) const
{
    Trace::out("protoData Print-- %s: \n", _pName);
    Trace::out("meshCount: %d \n", this->meshCount);
    Trace::out("animCount: %d \n", this->animCount);
    Trace::out("textureCount: %d \n", this->textureCount);
}