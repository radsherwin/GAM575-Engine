//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CameraManager.h"
#include "StringThis.h"

//Testing displaying text

namespace Azul
{
//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
CameraManager::CameraManager()
	: poActive(nullptr), pCamPerspective(nullptr), pCamOrthographic(nullptr), pCameraMesh(nullptr)
{
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void CameraManager::Create()
{
	// Get the instance to the manager
	CameraManager *pCameraMan = CameraManager::privGetInstance();
	assert(pCameraMan);
}

void CameraManager::Destroy()
{
	// Get the instance to the manager
	CameraManager *pCameraMan = CameraManager::privGetInstance();
	assert(pCameraMan);

	CameraLink *pTmp = pCameraMan->poActive;
	while (pTmp != nullptr)
	{
		CameraLink *pDeleteMe = pTmp;
		pTmp = pTmp->next;
		delete pDeleteMe;
	}
}

void CameraManager::Add(Camera::ID _camID, Camera *pCamera)
{
	CameraManager *pCamMan = CameraManager::privGetInstance();
	assert(pCamMan);

	CameraNode *pNode = new CameraNode();
	assert(pNode);

	assert(pCamera);
	pCamera->SetCameraID(_camID);
	pCamera->SetOriginal();
	pNode->set(pCamera);

	pCamMan->privAddToFront(pNode, pCamMan->poActive);
}

//void CameraManager::Add(Camera::ID camID, const float fovDeg, const float nearDist, const float farDist,
//						const int width, const int height)
//{
//	CameraManager *pMan = CameraManager::privGetInstance();
//
//	Camera *pCam = new Camera();
//
//	// Initialize the date
//	switch (camID)
//	{
//		case Camera::ID::MAIN:
//			pCam->bStatic = false;
//			pCam->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, 0.0f), Vect(0.0f, 0.0f, 4.5f));
//			break;
//		case Camera::ID::SIDE:
//			pCam->bStatic = true;
//			pCam->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 1.0f, -7.5f), Vect(40.0f, 1.0f, -7.5f));
//			break;
//		case Camera::ID::TOP:
//			pCam->bStatic = true;
//			pCam->setOrientAndPosition(Vect(0.0f, 0.0f, -1.0f), Vect(0.0f, 1.0f, -5.0f), Vect(0.0f, 40.0f, -5.0f));
//			break;
//		case Camera::ID::FORWARD:
//			pCam->bStatic = true;
//			pCam->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, 0.0f), Vect(0.0f, 0.0f, 30.0f));
//			break;
//	}
//
//	pCam->Set(camID, fovDeg, nearDist, farDist, width, height);
//	pCam->updateCamera();
//
//	// Create a TextureNode
//	CameraNode *pNode = new CameraNode();
//	assert(pNode);
//
//	pNode->set(pCam);
//
//	// Now add it to the manager
//	pMan->privAddToFront(pNode, pMan->poActive);
//
//	//return pCam;
//}

void CameraManager::AddMesh(CameraMesh *_mesh)
{
	CameraManager *pCameraMan = CameraManager::privGetInstance();
	assert(pCameraMan);
	pCameraMan->pCameraMesh = _mesh;
}

Camera *CameraManager::Find(Camera::ID _name)
{
	// Get the instance to the manager
	CameraManager *pCameraMan = CameraManager::privGetInstance();
	assert(pCameraMan);

	CameraNode *pNode = (CameraNode *)pCameraMan->poActive;
	while (pNode != 0)
	{
		if (pNode->poCamera->GetCameraID() == _name)
		{
			// found it
			break;
		}

		pNode = (CameraNode *)pNode->next;
	}
	assert(pNode);
	return pNode->poCamera;
}

void CameraManager::SetCurrent(Camera::ID _cameraID, Camera::Type _type)
{
	CameraManager *pCameraMan = CameraManager::privGetInstance();
	assert(pCameraMan);

	Camera *pCam = CameraManager::Find(_cameraID);
	assert(pCam);

	switch (_type)
	{
		case Camera::Type::PERSPECTIVE_3D:
			assert(pCam->GetType() == Camera::Type::PERSPECTIVE_3D);
			pCameraMan->pCamPerspective = pCam;
			break;

		case Camera::Type::ORTHOGRAPHIC_2D:
			assert(pCam->GetType() == Camera::Type::ORTHOGRAPHIC_2D);
			pCameraMan->pCamOrthographic = pCam;
			break;
		default:
			assert(false);
			break;
	}
}

Camera *CameraManager::GetCurrent(Camera::Type _type)
{
	Camera *pCam = nullptr;
	switch(_type)
	{
		case Camera::Type::PERSPECTIVE_3D:
			pCam = CameraManager::privGetInstance()->pCamPerspective;
			break;
		case Camera::Type::ORTHOGRAPHIC_2D:
			pCam = CameraManager::privGetInstance()->pCamOrthographic;
			break;
		default:
			assert(false);
			break;
	}

	assert(pCam != nullptr);
	return pCam;
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
CameraManager *CameraManager::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static CameraManager cameraMan;
	return &cameraMan;
}

void CameraManager::privAddToFront(CameraLink *node, CameraLink *&head)
{
	assert(node != nullptr);

	if (head == nullptr)
	{
		head = node;
		node->next = nullptr;
		node->prev = nullptr;
	}
	else
	{
		node->next = head;
		head->prev = node;
		head = node;
	}
}

void CameraManager::privRemove(CameraLink *pNode, CameraLink *&poHead)
{
	assert(pNode);

	if (pNode->prev != nullptr)
	{	// middle or last node
		pNode->prev->next = pNode->next;
	}
	else
	{  // first
		poHead = (CameraLink *)pNode->next;
	}

	if (pNode->next != nullptr)
	{	// middle node
		pNode->next->prev = pNode->prev;
	}
}

void CameraManager::Update(Camera::Type _type)
{
	CameraManager *pMan = CameraManager::privGetInstance();

	switch(_type)
	{
		case Camera::Type::PERSPECTIVE_3D:
			pMan->pCamPerspective->updateCamera();
			break;
		case Camera::Type::ORTHOGRAPHIC_2D:
			pMan->pCamOrthographic->updateCamera();
			break;
		default:
			assert(false);
			break;
	}

	if (pMan->pCameraMesh != nullptr) pMan->pCameraMesh->update();
}
}