#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "Resources.h"
#include "cinder/Timeline.h"
#include "cinder/CinderMath.h"
#include "cinder/Rand.h"
#include <vector>

#include "VectorFlowFieldApp.h"
#include "ParticleController.h"
#include "CloudController.h"

#define RESOLUTION 50
using namespace ci;
using namespace ci::app;
using namespace std;

void VectorFlowFieldApp::setup()
{
	mPrevTime = 0.0f;
	mTheta = 0.1f * (M_PI/180); 
    mWindDirection = Vec2f(1.0f, 0.0f);
    mChannel = Channel32f(loadImage(loadResource(RES_WELLINGTON_IMG)));
    mTexture = mChannel;
    
    mParticleController = new ParticleController(RESOLUTION);

	//mCloudController = new CloudController(Vec2f(0, 0), 0.0f, 0.0f, mParticleController);
   


	for(int i = 0; i < 10; ++i){
		Vec2f loc = Vec2f(Rand::randFloat(0.0f, app::getWindowWidth()), Rand::randFloat(0.0f, app::getWindowHeight()));

		CloudController* cloudController = new CloudController(loc, Rand::randFloat(2.0, 5.0), Rand::randFloat(0.1f, 0.5f), mParticleController);
		mCloudControllers.push_back(cloudController);
	}
		
    mDrawParticles = true;
    mDrawImage = false;
    
}


void VectorFlowFieldApp::update()
{
    if(! mChannel) return;
    
	if((timeline().getCurrentTime() - mPrevTime) > 0.01){

		float newX = mWindDirection.x*cos(mTheta) - mWindDirection.y*sin(mTheta);
		float newY = mWindDirection.x*sin(mTheta) + mWindDirection.y*cos(mTheta);

		mWindDirection.x = newX;
		mWindDirection.y = newY;

		mWindDirection.normalize();


		mPrevTime = timeline().getCurrentTime();
	}

    mParticleController->update(mChannel, mWindDirection);
	
	if(!mCloudControllers.empty()){
	for(int i = 0; i < mCloudControllers.size(); i++){
		mCloudControllers[i]->update();
		}
	}
}

void VectorFlowFieldApp::draw()
{
    
    gl::setViewport(getWindowBounds());
    gl::setMatricesWindow(getWindowSize());  //NOTE: remeber this big dog error thrower
//    gl::setMatricesWindowPersp(getWindowSize()); //NOTE: doesnt seem to work as well
    gl::color(1, 1, 1);
    
	/*
    if(mDrawImage){
        mTexture.enableAndBind();
        gl::draw(mTexture, getWindowBounds()); //NOTE: this is the image, dont really need, keeping for debugging
    }
     */
    
    /*
    if(mDrawParticles){                 //NOTE: this is the flow field
        glDisable(GL_TEXTURE_2D);
        mParticleController->draw();
    }
     */

	if(!mCloudControllers.empty()){
		for(int i = 0; i < mCloudControllers.size(); i++){
			mCloudControllers[i]->draw();
		}
	}
    
    
}

//CINDER_APP_BASIC( VectorFlowFieldApp, RendererGl )
