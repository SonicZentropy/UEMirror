/*
* Copyright (c) 2008-2017, NVIDIA CORPORATION.  All rights reserved.
*
* NVIDIA CORPORATION and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#ifndef FRICTION_SCENE_H
#define FRICTION_SCENE_H

#include "scene/Scene.h"
#include <foundation/PxVec3.h>

class FrictionScene : public Scene
{
public:

	FrictionScene(SceneController* sceneController): Scene(sceneController) {}

	void initializeCloth(int index, physx::PxVec3 offset, float frictionCoef);
	virtual void onInitialize() override;

private:
	nv::cloth::Fabric* mFabric[5];
	nv::cloth::Solver* mSolver;
	ClothActor* mClothActor[5];
};


#endif