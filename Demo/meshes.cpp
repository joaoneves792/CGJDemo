//
// Created by joao on 11/21/17.
//

#include "CGJengine.h"
#include "meshes.h"
#include "Constants.h"

void loadMeshes(){
    ResourceManager::Factory::createMesh(CAR, "res/F430GTC/F430GTC.h3d");
    ResourceManager::Factory::createMesh(ROAD, "res/Road/road.h3d");
    ResourceManager::Factory::createMesh(SKY, "res/skybox/skybox.h3d");
    ResourceManager::Factory::createMesh(WHEEL, "res/F430GTC/F430GTC_wheel.h3d");
}