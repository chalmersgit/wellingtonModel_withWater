//
//  ParticleController.cpp
//  VectorFlowField
//
//  Created by Oliver Ellmers on 5/05/13.
//
//

#include "ParticleController.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "cinder/Perlin.h"
#include <vector>
#include <stdlib.h>  

using namespace ci;
using namespace ci::app;
using std::list;

ParticleController::ParticleController()
{

}

ParticleController::~ParticleController()
{
	/*
	if(mFlowLookUpTable != NULL){
		for (int i = 0; i < mXRes; i++){  
			//console() << i << endl;
			free(mFlowLookUpTable[i]);  
		}  
		free(mFlowLookUpTable);
	}*/
}

ParticleController::ParticleController(int res)
{
    mXRes = app::getWindowWidth()/res;
    mYRes = app::getWindowHeight()/res;

	mFlowLookUpTable = createFlowField(mXRes, mYRes);



	Perlin perlin = Perlin(32, clock() * 0.1f);
	Vec2f v = Vec2f(0.05f, 0.05f);
	float noise = perlin.fBm(v);
	float angle = noise * 15.0f;
	float xoff = 0.0f;
    for(int y=0; y<mYRes; y++){
		float yoff = 0.0f;
        for(int x=0; x<mXRes; x++){
			float thetaX = cos(angle) * Rand::randFloat(0, xoff);
			float thetaY = sin(angle) * Rand::randFloat(0, yoff);

			Vec2f rotationVector(thetaX, thetaY);

            addParticle(x, y, res, rotationVector);
			yoff += 0.1f;
        }
		xoff += 0.1f;
    }
}

void ParticleController::update( const Channel32f &channel, const Vec2f &windDirection)
{
    for(list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p){
        p->update(channel, windDirection);
    }
}

void ParticleController::draw()
{
    for (list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p){
        p->draw();
    }
}

void ParticleController::addParticle(int xi, int yi, int res, Vec2f rotationVector)
{
    float x = (xi + 0.5f) * (float)res;
    float y = (yi + 0.5f) * (float)res;
	Particle p = Particle(Vec2f(x, y), rotationVector);
    mParticles.push_back(p);

	mFlowLookUpTable[xi][yi] = p;

}

Particle** ParticleController::createFlowField(int arraySizeX, int arraySizeY) {  
	Particle** theArray;  
	theArray = (Particle**) malloc(arraySizeX*sizeof(Particle*));  
	for (int i = 0; i < arraySizeX; i++){
		theArray[i] = (Particle*) malloc(arraySizeY*sizeof(Particle));  
	}
	return theArray;  
} 

void ParticleController::addParticles(int amt)
{
    for(int i=0; i<amt; i++){
        for(int i=0; i<amt; i++){
            float x = Rand::randFloat(app::getWindowWidth());
            float y = Rand::randFloat(app::getWindowHeight());
            mParticles.push_back(Particle(Vec2f(x, y), Vec2f(1.0f, 0.0f)));
        }
    }
}

void ParticleController::removeParticles(int amt)
{
    for(int i=0; i<amt; i++){
        mParticles.pop_back();
    }
                    
}

void ParticleController::printFlow(){
	if(mFlowLookUpTable != NULL){
		for (int x = 0; x < mXRes; ++x){ 
			for(int y = 0; y < mYRes; ++y){
				console() << mFlowLookUpTable[x][y].getLocation() << endl;
			}
		}  
	}
}

/*
Vec2f ParticleController::flowLookUp(Vec2f lookup){

	int column = constrain(lookup.x/mXRes, 0, mXRes-1);
	int row = constrain(lookup.y/mYRes, 0, mYRes-1);

	return Vec2f(0, 0);
}
*/



