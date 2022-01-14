//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "meshData.h"

meshData::meshData()
    : pName{ 0 },
    mode{ RENDER_MODE::DEFAULT },
    triCount(0),
    vertCount(0),
    materialIndex(0),
    jointIndex(0),
    parentJointIndex(0)

{
}

meshData::meshData(const meshData &r)
    : mode(r.mode),
      vbo_vert(r.vbo_vert),
      vbo_norm(r.vbo_norm),
      vbo_uv(r.vbo_uv),
      vbo_index(r.vbo_index),
      vbo_color(r.vbo_color),
      triCount(r.triCount),
      vertCount(r.vertCount),
      materialIndex(r.materialIndex),
      jointIndex(r.jointIndex),
      parentJointIndex(r.parentJointIndex)
{
    memcpy_s(this->pName, meshData::FILE_NAME_SIZE, r.pName, meshData::FILE_NAME_SIZE);
}

meshData & meshData::operator=(const meshData &r)
{
    if(this != &r)
    {
        this->mode = r.mode;
        this->vbo_vert = r.vbo_vert;
        this->vbo_norm = r.vbo_norm;
        this->vbo_uv = r.vbo_uv;
        this->vbo_index = r.vbo_index;
        this->vbo_color = r.vbo_color;
        this->triCount = r.triCount;
        this->vertCount = r.vertCount;
        this->materialIndex = r.materialIndex;
        this->jointIndex = r.jointIndex;
        this->parentJointIndex = r.parentJointIndex;
        memcpy_s(this->pName, meshData::FILE_NAME_SIZE, r.pName, meshData::FILE_NAME_SIZE);
    }

    return *this;
}

meshData::~meshData()
{
}

void meshData::Serialize(meshData_proto &out) const
{
    vboData_proto *pVBO_proto;

    std::string sName((const char *)this->pName, meshData::FILE_NAME_SIZE);
    out.set_pname(sName);

    out.set_tricount(this->triCount);
    out.set_vertcount(this->vertCount);
    out.set_materialindex(this->materialIndex);
    out.set_jointindex(this->jointIndex);
    out.set_parentjointindex(this->parentJointIndex);

    out.set_mode((meshData_proto_RENDER_MODE)(this->mode));

    pVBO_proto = new vboData_proto();
    this->vbo_vert.Serialize(*pVBO_proto);
    out.set_allocated_vbo_vert(pVBO_proto);

    pVBO_proto = new vboData_proto();
    this->vbo_norm.Serialize(*pVBO_proto);
    out.set_allocated_vbo_norm(pVBO_proto);

    pVBO_proto = new vboData_proto();
    this->vbo_uv.Serialize(*pVBO_proto);
    out.set_allocated_vbo_uv(pVBO_proto);

    pVBO_proto = new vboData_proto();
    this->vbo_index.Serialize(*pVBO_proto);
    out.set_allocated_vbo_index(pVBO_proto);

    pVBO_proto = new vboData_proto();
    this->vbo_color.Serialize(*pVBO_proto);
    out.set_allocated_vbo_color(pVBO_proto);
}

void meshData::Deserialize(const meshData_proto &in)
{
    memcpy_s(this->pName, meshData::FILE_NAME_SIZE, in.pname().data(), meshData::FILE_NAME_SIZE);

    this->triCount = in.tricount();
    this->vertCount = in.vertcount();
    this->materialIndex = in.materialindex();
    this->jointIndex = in.jointindex();
    this->parentJointIndex = in.parentjointindex();

    this->mode = (RENDER_MODE)in.mode();

    this->vbo_vert.Deserialize(in.vbo_vert());
    this->vbo_norm.Deserialize(in.vbo_norm());
    this->vbo_uv.Deserialize(in.vbo_uv());
    this->vbo_index.Deserialize(in.vbo_index());
    this->vbo_color.Deserialize(in.vbo_color());
}

void meshData::Print(const char *const _pName) const
{
    /*Trace::out("%s: \n", _pName);

    for (int i = 0; i < (int)this->meshCount; i++)
    {
        Trace::out("version: %s \n", this->pVersion);
        Trace::out("\n");
        if (i < (int)this->nameCount) Trace::out("pName: %s \n", this->pName[i]);
        this->vbo_vert[i].Print("vbo_vert", i);
        Trace::out("\n");
        this->vbo_norm[i].Print("vbo_norm", i);
        Trace::out("\n");
        this->vbo_uv[i].Print("vbo_uv", i);
        Trace::out("\n");
        this->vbo_index[i].Print("vbo_index", i);
        Trace::out("\n");
        this->vbo_color[i].Print("vbo_color", i);
        Trace::out("\n");
    }
    for (int i = 0; i < (int)this->texCount; i++)
    {
        this->text_color[i].Print("text_color", i);
        Trace::out("\n");
    }

    for(int i = 0; i < (int)this->animCount; i++)
    {
        this->anim_data[i].Print("anim_data", i);
        Trace::out("\n");
    }*/
}

// --- End of File ---