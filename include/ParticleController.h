//
//  ParticleController.h
//  VectorFlowField
//
//  Created by Oliver Ellmers on 5/05/13.
//
//


#pragma once
#include "Particle.h"
#include <list>

#include <vector>

using namespace std;

class ParticleController{
public:
    ParticleController();
	~ParticleController();
    ParticleController(int res);
    void update(const ci::Channel32f &channel, const ci::Vec2f &mouseLoc);
    void draw();
    void addParticle(int xi, int yi, int res, ci::Vec2f);
    void addParticles(int amt);
    void removeParticles(int amt);
	void printFlow();
    Particle** createFlowField(int arraySizeX, int arraySizeY);

    //Vec2f flowLookUp(ci::Vec2f);

    list<Particle> mParticles;

	//Vec2f mParticleLookUp[][];
    
    int mXRes, mYRes;
    
	Particle** mFlowLookUpTable;

};

