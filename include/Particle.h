//
//  Particle.h
//  VectorFlowField
//
//  Created by Oliver Ellmers on 5/05/13.
//
//

#pragma once
#include "cinder/Channel.h"
#include "cinder/Vector.h"

#include <vector>


class Particle {
public:
    Particle();
    Particle(ci::Vec2f, ci::Vec2f);
    void update(const ci::Channel32f &channel, const ci::Vec2f &windDirectionr);
    void draw();

    ci::Vec2f mLoc;
    ci::Vec2f mDir;
	ci::Vec2f mRotationVector;

    float mRadius;

	//Getters
	ci::Vec2f getLocation(){
		return mLoc;
   }

	ci::Vec2f getDirection(){
		return mDir;
   }

};

//#endif /* defined(__VectorFlowField__Particle__) */
