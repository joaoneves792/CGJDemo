//
// Created by joao on 11/21/17.
//

#include "CGJengine.h"
#include "meshes.h"
#include "Constants.h"

void loadMeshes(){
    ResourceManager::Factory::createMesh(CAR, "res/charger/charger.h3d");
    ResourceManager::Factory::createMesh(ROAD, "res/Road/road.h3d");
    ResourceManager::Factory::createMesh(SKY, "res/Road/sky.h3d");
}