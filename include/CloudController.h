/*
	2013
*/


#pragma once
#include "cinder/Vector.h"
#include "ParticleController.h"

#include <vector>


class CloudController {
public:
    CloudController();
    CloudController(ci::Vec2f, float, float, ParticleController*);
    void update();
    void draw();

	//void follow();

	ParticleController* mParticleController;

    ci::Vec2f mLoc;
    ci::Vec2f mVel;
	ci::Vec2f mAcc;

	float mMaxForce;
	float mMaxSpeed;

	float mRadius;
};



