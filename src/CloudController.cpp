//
//  Particle.cpp
//  VectorFlowField
//
//  Created by Oliver Ellmers on 5/05/13.
//
//


#include "cinder/Rand.h"
#include "CloudController.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;
using namespace ci::app;
using namespace std;

CloudController::CloudController(){


}


CloudController::CloudController(Vec2f loc, float ms, float mf, ParticleController* pc){
    mLoc = loc;
	mMaxForce = mf;
	mMaxSpeed = ms;
	mParticleController = pc;

	mAcc = Vec2f(1.0f, 0.0f);
	mVel = Vec2f(0.0f, 0.0f);
    mRadius = 10.0f;
}

void CloudController::update(){

	mVel += mAcc;
	mVel.limit(mMaxSpeed);

	mLoc += mVel;

	//?
	//mAcc *= 0.0f;
	
}

void CloudController::draw(){
	//console() << mLoc << endl;
	gl::color(1.0, 0.0, 0.0);
	Rectf rect(mLoc.x, mLoc.y, mLoc.x + mRadius, mLoc.y + mRadius);
    gl::drawSolidRect(rect);



}

/*
void CloudController::follow(){
	

}
*/


