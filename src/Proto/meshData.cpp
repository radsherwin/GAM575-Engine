//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "meshData.h"

meshData::meshData()
    : enabled{ false },
    pName{ 0 },
    mode{ RENDER_MODE::DEFAULT },
    triCount(0),
    vertCount(0),
    materialIndex(0),
    jointIndex(0),
    parentJointIndex(0)

{
}

meshData::meshData(const meshData &r)
    : enabled{ r.enabled },
    mode(r.mode),
    vbo_vert(r.vbo_vert),
    vbo_norm(r.vbo_norm),
    vbo_uv(r.vbo_uv),
    vbo_index(r.vbo_index),
    vbo_color(r.vbo_color),
    vbo_weights(r.vbo_weights),
    vbo_joints(r.vbo_joints),
    vbo_invBind(r.vbo_invBind),
    triCount(r.triCount),
    vertCount(r.vertCount),
    materialIndex(r.materialIndex),
    jointIndex(r.jointIndex),
    parentJointIndex(r.parentJointIndex)
{
    memcpy_s(this->pName, meshData::FILE_NAME_SIZE, r.pName, meshData::FILE_NAME_SIZE);
}

meshData &meshData::operator=(const meshData &r)
{
    if (this != &r)
    {
        this->enabled = r.enabled;
        this->mode = r.mode;
        this->vbo_vert = r.vbo_vert;
        this->vbo_norm = r.vbo_norm;
        this->vbo_uv = r.vbo_uv;
        this->vbo_index = r.vbo_index;
        this->vbo_color = r.vbo_color;
        this->vbo_weights = r.vbo_weights;
        this->vbo_joints = r.vbo_joints;
        this->vbo_invBind = r.vbo_invBind;
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
    out.set_enabled(this->enabled);
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

    // Skinning
    pVBO_proto = new vboData_proto();
    this->vbo_weights.Serialize(*pVBO_proto);
    out.set_allocated_vbo_weights(pVBO_proto);

    pVBO_proto = new vboData_proto();
    this->vbo_joints.Serialize(*pVBO_proto);
    out.set_allocated_vbo_joints(pVBO_proto);

    pVBO_proto = new vboData_proto();
    this->vbo_invBind.Serialize(*pVBO_proto);
    out.set_allocated_vbo_invbind(pVBO_proto);
}

void meshData::Deserialize(const meshData_proto &in)
{
    this->enabled = in.enabled();
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

    // Skinning
    if (in.vbo_weights().enabled()) this->vbo_weights.Deserialize(in.vbo_weights());
    if (in.vbo_joints().enabled()) this->vbo_joints.Deserialize(in.vbo_joints());
    if (in.vbo_invbind().enabled()) this->vbo_invBind.Deserialize(in.vbo_invbind());
}

void meshData::Print(const char *const _pName, const int i) const
{
    Trace::out("\n\n--------Mesh Data: %s, %d-------- \n", this->pName, i);
    Trace::out("Parent Name: %s\n", _pName);

    Trace::out("\n");
    this->vbo_vert.Print("vbo_vert");
    Trace::out("\n");
    this->vbo_norm.Print("vbo_norm");
    Trace::out("\n");
    this->vbo_uv.Print("vbo_uv");
    Trace::out("\n");
    this->vbo_index.Print("vbo_index");
    Trace::out("\n");
    this->vbo_color.Print("vbo_color");
    Trace::out("\n");
}

// --- End of File ---