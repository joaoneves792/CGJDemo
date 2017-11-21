//
// Created by joao on 11/21/17.
//

#include "CGJengine.h"
#include "meshes.h"
#include "ResourceNames.h"

void loadMeshes(){
    ResourceFactory::createMesh(CAR, "res/charger/charger.h3d");
    ResourceFactory::createMesh(ROAD, "res/Road/road.h3d");
    ResourceFactory::createMesh(SKY, "res/Road/sky.h3d");
}