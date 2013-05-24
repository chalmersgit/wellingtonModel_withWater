//
//  VectorDlowFieldApp.h
//  wellingtonModel
//
//  Created by Oliver Ellmers on 25/05/13.
//
//

//#ifndef wellingtonModel_VectorDlowFieldApp_h
//#define wellingtonModel_VectorDlowFieldApp_h
//
//
//
//#endif

#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

#include <vector>
#include "ParticleController.h"
#include "CloudController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VectorFlowFieldApp{
public:
	void setup();
	void update();
	void draw();
    
    Channel32f mChannel;
    gl::Texture mTexture;
    
    
    ParticleController* mParticleController;
    
	vector<CloudController*> mCloudControllers;
	//CloudController* mCloudController;
    
    bool mDrawParticles;
    bool mDrawImage;
    
    Vec2i mMouseLoc;
    Vec2f mWindDirection;
    
	float mTheta;
	float mPrevTime;
};