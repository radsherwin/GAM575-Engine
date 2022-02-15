//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "sb7.h"
#include "GameManager.h"

#include "MathEngine.h"
#include "VectApp.h"
#include "QuatApp.h"

#include "GameObject.h"
#include "ShaderObject.h"
#include "CubeMesh.h"
#include "ProtoBuffMesh.h"

#include "TextureManager.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "MeshNodeManager.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "AnimationManager.h"
#include "ImageManager.h"
#include "FontManager.h"
#include "GlyphManager.h"
#include "SkeletonManager.h"

#include "Camera.h"
#include "CameraInput.h"
#include "ProtoMeshFactory.h"
#include "SphereMesh.h"
#include "GameObjectBoundingSphere.h"
#include "CameraMesh.h"

#include "GraphicsObjectHdr_Sprite.h"
#include "GraphicsObject_Sprite.h"
#include "SpriteMesh.h"
#include "GameObject2D.h"
#include "GameObjectFont.h"

//Animation
#include "TimerController.h"
#include "AnimController.h"
#include "Clip.h"
#include "NullMesh.h"
#include "Skeleton.h"

using namespace Azul;

namespace Azul
{
//AnimController* pAnimController;
//Clip* pWalkClip = nullptr;
//-----------------------------------------------------------------------------
//  Game::Game()
//		Game Engine Constructor
//-----------------------------------------------------------------------------
Game::Game(const char *const pWindowName, const int Width, const int Height)
    :Engine(pWindowName, Width, Height)
{
    assert(pWindowName);
    this->globalTimer.Tic();
    this->intervalTimer.Tic();
}

//-----------------------------------------------------------------------------
//  Game::~Game()
//		Game Engine destructor
//-----------------------------------------------------------------------------
Game::~Game()
{
}

//-----------------------------------------------------------------------------
// Game::Initialize()
//		Allows the engine to perform any initialization it needs to before
//      starting to run.  This is where it can query for any required services
//      and load any non-graphic related content.
//-----------------------------------------------------------------------------
void Game::Initialize()
{
}

//-----------------------------------------------------------------------------
// Game::LoadContent()
//		Allows you to load all content needed for your engine,
//	    such as objects, graphics, etc.
//-----------------------------------------------------------------------------
void Game::LoadContent()
{
    InputManager::Create();
    TextureManager::Create();
    MeshNodeManager::Create();
    ShaderManager::Create();
    CameraManager::Create();
    CameraInput::Create(this->window);
    AnimationManager::Create();
    ImageManager::Create();
    FontManager::Create();
    GlyphManager::Create();
    SkeletonManager::Create();

    //-----------------------------------------------------------------------------
    //	    Load Cameras
    //-----------------------------------------------------------------------------

    //Perspective 3D
    Camera *pCam0 = new Camera(Camera::Type::PERSPECTIVE_3D);
    pCam0->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
    pCam0->setPerspective(35.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.f, 10000.f);
    pCam0->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, 0.0f), Vect(0.0f, 0.0f, 14.5f));
    pCam0->updateCamera();
    CameraManager::Add(Camera::ID::CAMERA_0, pCam0);
    CameraManager::SetCurrent(Camera::ID::CAMERA_0, Camera::Type::PERSPECTIVE_3D);

    //Orthographic 2D
    Camera *pCam2D = new Camera(Camera::Type::ORTHOGRAPHIC_2D);
    pCam2D->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
    pCam2D->setOrthographic((float)-pCam2D->getScreenWidth() / 2.f, (float)pCam2D->getScreenWidth() / 2.f,
                            (float)-pCam2D->getScreenHeight() / 2.f, (float)pCam2D->getScreenHeight() / 2.f,
                            1.0f, 1000.f);
    pCam2D->setOrientAndPosition(Vect(0.f, 1.f, 0.f), Vect(0.f, 0.f, -1.f), Vect(0.f, 0.f, 2.f));
    CameraManager::Add(Camera::ID::CAMERA_1, pCam2D);
    CameraManager::SetCurrent(Camera::ID::CAMERA_1, Camera::Type::ORTHOGRAPHIC_2D);

    //-----------------------------------------------------------------------------
    //	    Load Textures
    //-----------------------------------------------------------------------------

    TextureManager::Add("../src/Textures/HotPink.t.proto.azul", Texture::Name::PINK_ERROR);
    TextureManager::Add("../src/Textures/Rocks.t.proto.azul", Texture::Name::ROCKS);
    TextureManager::Add("../src/Textures/Stone.t.proto.azul", Texture::Name::STONES);
    TextureManager::Add("../src/Textures/RedBrick.t.proto.azul", Texture::Name::RED_BRICK);
    TextureManager::Add("../src/Textures/Duckweed.t.proto.azul", Texture::Name::DUCKWEED);
    TextureManager::Add("../src/Textures/Aliens.t.proto.azul", Texture::Name::INVADERS);
    TextureManager::Add("../src/Textures/Font30pt.t.proto.azul", Texture::Name::FONT_30PT);

    //-----------------------------------------------------------------------------
    //	    Load Glyphs and Fonts
    //-----------------------------------------------------------------------------

    GlyphManager::Add("../src/Fonts/FontMetrics.f.proto.azul", Glyph::Name::FONT_30PT, Texture::Name::FONT_30PT);

    FontManager::Add("Testing", Glyph::Name::FONT_30PT, Font::Name::FONT_30PT, 600, 650);

    //-----------------------------------------------------------------------------
    //	    Load Basic Mesh
    //-----------------------------------------------------------------------------

    //Sphere
    Mesh *pSphereMesh = new SphereMesh("sphereModel.azul");
    MeshNodeManager::Add(Mesh::Name::SPHERE, pSphereMesh);
    //Cube
    Mesh *pCubeMesh = new CubeMesh("./Models/cubeMEsh.azul");
    MeshNodeManager::Add(Mesh::Name::CUBE, pCubeMesh);

    Mesh *pNullMesh = new NullMesh(nullptr);
    MeshNodeManager::Add(Mesh::Name::NULL_MESH, pNullMesh);

    // SPRITE
    SpriteMesh *pSpriteMesh = new SpriteMesh("SpriteMesh.m.proto.azul");
    assert(pSpriteMesh);
    MeshNodeManager::Add(Mesh::Name::SPRITE, pSpriteMesh);

    // Corset
    Mesh *pCorsetMesh = nullptr;
    ProtoMeshFactory::CreateMeshSingle("Corset.mt.proto.azul", pCorsetMesh, Texture::Name::CORSET);
    MeshNodeManager::Add(Mesh::Name::CORSET, pCorsetMesh);

    // Antique Camera
    Mesh *pAntigueCamera[2];
    ProtoMeshFactory::CreateMeshArray("AntiqueCamera.mt.proto.azul", pAntigueCamera, Texture::Name::ANTIQUE_CAMERA);
    MeshNodeManager::Add(Mesh::Name::ANTIQUE_CAMERA, pAntigueCamera, 2);

    // DogHouse
    Mesh *pDogHouseMesh = nullptr;
    ProtoMeshFactory::CreateMeshSingle("DogHouse.mt.proto.azul", pDogHouseMesh, Texture::Name::DOG_HOUSE);
    MeshNodeManager::Add(Mesh::Name::DOG_HOUSE, pDogHouseMesh);

    Mesh *chickenBotMesh;
    ProtoMeshFactory::CreateMeshSingle("walk_mesh.mat.proto.azul", chickenBotMesh, Texture::Name::CHICKEN_BOT);
    MeshNodeManager::Add(Mesh::Name::CHICKEN_BOT, chickenBotMesh);

    Animation *Anim_Die_Left = nullptr;
    ProtoMeshFactory::GetAnimation("die_left_mesh.a.proto.azul", Anim_Die_Left);

    Animation *Anim_Walk = nullptr;
    ProtoMeshFactory::GetAnimation("walk_mesh.mat.proto.azul", Anim_Walk);

    Animation *Anim_Shot_Down = nullptr;
    ProtoMeshFactory::GetAnimation("shot_down_mesh.a.proto.azul", Anim_Shot_Down);

    Animation *Anim_Hit_Right = nullptr;
    ProtoMeshFactory::GetAnimation("hit_right_mesh.a.proto.azul", Anim_Hit_Right);

    Animation *Anim_Run = nullptr;
    ProtoMeshFactory::GetAnimation("run_RM_mesh.a.proto.azul", Anim_Run);

    //-----------------------------------------------------------------------------
    //	    Create/Load Shaders
    //-----------------------------------------------------------------------------

    ShaderObject *pShaderObject_sprite = ShaderManager::Add(ShaderObject::Name::SPRITE, "../src/GLSL/spriteRender");
    assert(pShaderObject_sprite);

    ShaderObject *pShaderObject_color = ShaderManager::Add(ShaderObject::Name::COLOR_POSITION, "../src/GLSL/colorRender");
    assert(pShaderObject_color);

    ShaderObject *pShaderObject_Null = ShaderManager::Add(ShaderObject::Name::NULL_SHADER, "../src/GLSL/nullRender");

    ShaderObject *pShaderObject_texture = ShaderManager::Add(ShaderObject::Name::TEXTURE_SIMPLE, "../src/GLSL/textureRender");
    assert(pShaderObject_texture);

    ShaderObject *pShaderObject_textureLight = ShaderManager::Add(ShaderObject::Name::TEXTURE_POINT_LIGHT, "../src/GLSL/texturePointLightDiff");
    assert(pShaderObject_textureLight);

    ShaderObject *pShaderObject_constColor = ShaderManager::Add(ShaderObject::Name::CONST_COLOR, "../src/GLSL/constcolorRender");
    assert(pShaderObject_constColor);

    ShaderObject *pShaderObject_specLighting = ShaderManager::Add(ShaderObject::Name::SPEC_LIGHTING, "../src/GLSL/specLighting");
    assert(pShaderObject_specLighting);

    ShaderObject *pShaderObject_colorSingle = ShaderManager::Add(ShaderObject::Name::COLOR_SINGLE, "../src/GLSL/colorSingleRender");
    assert(pShaderObject_colorSingle);

    ////-----------------------------------------------------------------------------
    ////	    Create Image
    ////-----------------------------------------------------------------------------

    ImageManager::Add(Image::Name::Alien_Red, Texture::Name::INVADERS, Rect(554.f, 63.f, 98.f, 64.f));

    ImageManager::Add(Image::Name::Alien_Green, Texture::Name::INVADERS, Rect(136.0f, 64.0f, 85.0f, 63.0f));

    ////-----------------------------------------------------------------------------
    ////	    Sprite
    ////-----------------------------------------------------------------------------
    GraphicsObjectHdr_Sprite *pGraphicsHdr_Sprite = new GraphicsObjectHdr_Sprite();
    pGraphicsHdr_Sprite->Set_Sprite(pSpriteMesh,
                                    pShaderObject_sprite,
                                    ImageManager::Find(Image::Name::Alien_Red),
                                    Rect(455, 155, 150, 150));

    GameObject2D *pA1 = new GameObject2D(pGraphicsHdr_Sprite);
    GameObjectManager::Add(pA1, GameObjectManager::GetRoot());
    pA1->posX = 1000.f;
    pA1->posY = 300.f;
    pA1->scaleX = 2.f;
    pA1->SetName("ALIEN_RED");

    pGraphicsHdr_Sprite = new GraphicsObjectHdr_Sprite();
    pGraphicsHdr_Sprite->Set_Sprite(pSpriteMesh,
                                    pShaderObject_sprite,
                                    ImageManager::Find(Image::Name::Alien_Green),
                                    Rect(455, 155, 150, 150));

    pA1 = new GameObject2D(pGraphicsHdr_Sprite);
    GameObjectManager::Add(pA1, GameObjectManager::GetRoot());
    pA1->posX = 600;
    pA1->posY = 350.f;
    pA1->angle = 20.f;
    pA1->SetName("ALIEN_GREEN");

    // Font
    pGraphicsHdr_Sprite = new GraphicsObjectHdr_Sprite();
    pGraphicsHdr_Sprite->Set_Font(pSpriteMesh, pShaderObject_sprite);
    GameObjectFont *pFont = new GameObjectFont(pGraphicsHdr_Sprite, Font::Name::FONT_30PT);
    GameObjectManager::Add(pFont, GameObjectManager::GetRoot());
    pFont->SetName("FONT");

    ////-----------------------------------------------------------------------------
    ////	    Basic GameObjects
    ////-----------------------------------------------------------------------------

    // CUBE
    GraphicsObjectHdr *pGraphicsHdr = new GraphicsObjectHdr();
    pGraphicsHdr->Set_FlatTexture(pCubeMesh, pShaderObject_texture, Texture::Name::STONES);
    GameObjectBasic *pGameObject = new GameObjectBasic(pGraphicsHdr);
    pGameObject->SetTrans(-2.f, -1.f, 0.f);
    pGameObject->SetScale(Vect(1.5f, 1.5f, 1.5f));
    pGameObject->deltaZ = 0.02f;
    pGameObject->deltaY = 0.02f;
    GameObjectManager::Add(pGameObject, GameObjectManager::GetRoot());

    // CORSET
    pGraphicsHdr = new GraphicsObjectHdr();
    pGraphicsHdr->Set_FlatTexture(pCorsetMesh, pShaderObject_texture, Texture::Name::CORSET);
    pGameObject = new GameObjectBasic(pGraphicsHdr);
    pGameObject->SetTrans(4.f, 1.f, 0.f);
    pGameObject->SetScale(Vect(17.f, 17.f, 17.f));
    pGameObject->deltaY = 0.01f;
    GameObjectManager::Add(pGameObject, GameObjectManager::GetRoot());

    // DOG HOUSE
    pGraphicsHdr = new GraphicsObjectHdr();
    pGraphicsHdr->Set_FlatTexture(pDogHouseMesh, pShaderObject_texture, Texture::Name::DOG_HOUSE);
    pGameObject = new GameObjectBasic(pGraphicsHdr);
    pGameObject->SetTrans(-3.f, 3.f, 0.f);
    pGameObject->SetScale(Vect(100.f, 100.f, 100.f));
    pGameObject->curRotZ = 0.5f;
    pGameObject->deltaY = 0.01f;
    GameObjectManager::Add(pGameObject, GameObjectManager::GetRoot());

    // CAMERA
    pGraphicsHdr = new GraphicsObjectHdr();
    pGraphicsHdr->Set_FlatTexture(pAntigueCamera, 2, pShaderObject_texture, Texture::Name::ANTIQUE_CAMERA);
    pGameObject = new GameObjectBasic(pGraphicsHdr);
    pGameObject->SetTrans(2.f, 2.f, 0.f);
    pGameObject->SetScale(Vect(.2f, .2f, .2f));
    pGameObject->SetName("Camera");
    GameObjectManager::Add(pGameObject, GameObjectManager::GetRoot());

    ////-----------------------------------------------------------------------------
    ////	    Create Animation
    ////-----------------------------------------------------------------------------

    SkeletonManager::Add(Skeleton::Name::CHICKEN_BOT_1, chickenBotMesh, Vect(-1.3f, 0,0));
    SkeletonManager::Add(Skeleton::Name::CHICKEN_BOT_2, chickenBotMesh, Vect(1.0f, 0,0));

    AnimationManager::Add(Anim_Die_Left, Clip::Name::DIE_LEFT);
    AnimationManager::Add(Anim_Walk, Clip::Name::WALK);
    AnimationManager::Add(Anim_Shot_Down, Clip::Name::SHOT_DOWN);
    AnimationManager::Add(Anim_Hit_Right, Clip::Name::HIT_RIGHT);
    AnimationManager::Add(Anim_Run, Clip::Name::RUN);

    AnimationManager::AddController(Skeleton::Name::CHICKEN_BOT_1, Clip::Name::WALK);

    //pSkel->AddController(AnimController::AnimName::MESH1, Clip::Name::WALK);

    //AnimationManager::AddController(AnimController::AnimName::MESH1, pSkel, Clip::Name::WALK);
    //AnimationManager::AddController(AnimController::AnimName::MESH2, pSkel2, Clip::Name::DIE_LEFT);

    //AnimationManager::Demo();

    CameraManager::Update(Camera::Type::PERSPECTIVE_3D);
    CameraManager::Update(Camera::Type::ORTHOGRAPHIC_2D);

    Trace::out("Z - Reset ,  I,O - Zoom In/Out\n");
    Trace::out("ASDF - Switch Cameras\n");
    Trace::out("H: Hide B Spheres\n");

    Trace::out("-: Slow animation speed, =: Increase animation speed\n ");
    Trace::out("Q: Play animation reverse, W: Player animation forward\n ");
    Trace::out("E: Play Demo 2\n");
}

//-----------------------------------------------------------------------------
// Game::Update()
//      Called once per frame, update data, transformations, etc
//      Use this function to control process order
//      Input, AI, Physics, Animation, and Graphics
//-----------------------------------------------------------------------------
int count = 0;
void Game::Update(float currentTime)
{
    this->intervalTimer.Toc();
    this->intervalTimer.Tic();

    // ------------------------------------
    // Camera update
    // ------------------------------------
    CameraManager::Update(Camera::Type::PERSPECTIVE_3D);
    CameraManager::Update(Camera::Type::ORTHOGRAPHIC_2D);
    CameraInput::Update();

    // ------------------------------------
    // Font update
    // ------------------------------------
    std::string s = "Count: ";
    s += std::to_string(count++);
    FontManager::UpdateMessage(Font::Name::FONT_30PT, s);

    // ------------------------------------
    // Animate Me
    // ------------------------------------
    SkeletonManager::Update();

    // ------------------------------------
    // GameObject update
    // ------------------------------------
    GameObjectManager::Update(this->globalTimer.Toc());
}

//-----------------------------------------------------------------------------
// Game::Draw()
//		This function is called once per frame
//	    Use this for draw graphics to the screen.
//      Only do rendering here
//-----------------------------------------------------------------------------
void Game::Draw()
{
    GameObjectManager::Draw();
    //FontManager::Draw();
}

//------------------- ----------------------------------------------------------
// Game::UnLoadContent()
//       unload content (resources loaded above)
//       unload all content that was loaded before the Engine Loop started
//-----------------------------------------------------------------------------
void Game::UnLoadContent()
{
    TextureManager::Destroy();
    InputManager::Destroy();
    GameObjectManager::Destroy();
    MeshNodeManager::Destroy();
    ShaderManager::Destroy();
    CameraManager::Destroy();
    ImageManager::Destroy();
    CameraInput::Destroy();
    AnimationManager::Destroy();
    GlyphManager::Destroy();
    FontManager::Destroy();
    SkeletonManager::Destroy();
}

//------------------------------------------------------------------
// Game::ClearBufferFunc()
// Allows user to change the way the clear buffer function works
//------------------------------------------------------------------
void Game::ClearBufferFunc()
{
    const GLfloat grey[] = { 0.7f, 0.7f, 0.8f, 1.0f };
    const GLfloat one = 1.0f;

    glViewport(0, 0, info.windowWidth, info.windowHeight);
    glClearBufferfv(GL_COLOR, 0, grey);
    glClearBufferfv(GL_DEPTH, 0, &one);
}
}

// --- End of File ---