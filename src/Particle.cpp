//
//  Particle.cpp
//  VectorFlowField
//
//  Created by Oliver Ellmers on 5/05/13.
//
//


#include "cinder/Rand.h"
#include "Particle.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;
using namespace ci::app;
using namespace std;

Particle::Particle()
{
}


Particle::Particle(Vec2f loc, Vec2f rotationVector){//, Vec2f initialDirection){
    mLoc = loc;
    
	mRotationVector = rotationVector;
    mDir = Vec2f(1.0f, 0.0f);
    mRadius = 4.0f;
}

void Particle::update(const Channel32f &channel, const Vec2f &windDirection){
    
    mDir = mRotationVector; //mouseLoc - mLoc;
    mDir.safeNormalize();
    
    Vec2f newLoc = mLoc + mDir * 100.f;
    newLoc.x = constrain(newLoc.x, 0.f, channel.getWidth() - 1.f);
    newLoc.x = constrain(newLoc.x, 0.f, channel.getHeight() - 1.f);
    
    mRadius = channel.getValue(newLoc) * 7.0f;
	
}

void Particle::draw(){
    //Rectf rect(mLoc.x, mLoc.y, mLoc.x + mRadius, mLoc.y + mRadius);
    //gl::drawSolidRect(rect);

	gl::color(Color(1.f, 1.f, 1.f));
    float arrowLength = 15.f;
	
	//console() << mLoc << endl;
	
    Vec3f p1(mLoc, 0.f);
    Vec3f p2(mLoc + mDir * arrowLength, 0.f);
    float headLength = 6.f;
    float headWidth = 3.f;
    gl::drawVector(p1, p2, headLength, headWidth);
}
