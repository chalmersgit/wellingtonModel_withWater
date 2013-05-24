#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ObjLoader.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Arcball.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Sphere.h"
#include "Resources.h"
#include "cinder/Sphere.h"
#include "waterModule.h"
#include "cinder/gl/GlslProg.h"
#include "VectorFlowFieldApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class wellingtonModelApp : public AppBasic {
  public:
    void prepareSettings( Settings *settings );
	void setup();
    void resize(ResizeEvent event);
    void mouseDown(MouseEvent event);
    void mouseDrag( MouseEvent event );
//    void mouseUp( MouseEvent event );
    void keyDown( KeyEvent event );
    void frameCurrentObject();
	void update();
	void draw();
    
    float userIncr;
    
    gl::Texture myImage;
    gl::VboMesh mVbo;
    
    Channel32f mChannel;
    TriMesh mMesh;
    Arcball mArcball;
    MayaCamUI mMayaCam;
    
    WaterModule *mWaterModule;
    VectorFlowFieldApp *mFlowField;
    
    //Mouse
    ci::Vec2i mMouse;
    bool mMouseDown;
    
    //show stuff
    bool drawWater;
    bool drawMesh;
    
    //shader
    gl::GlslProg wellingtonShader;
};

void wellingtonModelApp::prepareSettings(Settings *settings )
{
    settings->setWindowSize(1280, 720); //NOTE: DEBUG SIZE
//    settings->setWindowSize(1920, 1080); //NOTE: OUTPUT SIZE
    settings->setFrameRate(60.0);
}

void wellingtonModelApp::resize(ResizeEvent event)
{
    App::resize( event );
	mArcball.setWindowSize( getWindowSize() );
	mArcball.setCenter( Vec2f( getWindowWidth() / 2.0f, getWindowHeight() / 2.0f ) );
	mArcball.setRadius( 150 );
}

void wellingtonModelApp::setup()
{
    
    //Load shader
    try{
        wellingtonShader = gl::GlslProg(loadResource(RES_MODEL_VERT ), loadResource(RES_MODEL_FRAG ));
    }catch(gl::GlslProgCompileExc ex){
        console() << "Unable to compile model shader:\n" << ex.what() << "\n";
        return;
    }
    
    
    drawWater = true;
    drawMesh = true;
    
    userIncr = 0.0f; 
    
    //setFullScreen(true);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
//        myImage = gl::Texture(loadImage(loadResource(RES_WELLINGTON_IMG)));
    myImage = gl::Texture(loadImage(loadFile("/Users/oliverellmers/Desktop/Cinder Projects/Projects/wellingtonModel_withWater/resources/wellington_ALPHA.png")));
    
    
    
    //    mChannel = Channel32f(loadImage(loadResource("wellington.jpg")));
    //    mTexture = myImage;
    
    
    ObjLoader loader(loadFile("/Users/oliverellmers/Desktop/Cinder Projects/Projects/wellingtonModel_withWater/resources/wellington01.obj"));
    loader.load(&mMesh);
    mVbo = gl::VboMesh(mMesh);
    
    CameraPersp initialCam;
    initialCam.setPerspective( 77.5f, getWindowAspectRatio(), 5.0f, 3000.0f ); //TODO: get correct camera persp from C4D
    
    Vec3f mEye = Vec3f(0.0f, 30.0f, 0.0f);
    Vec3f mCenter = Vec3f::zero();
    Vec3f mUp = Vec3f::yAxis();
    
    initialCam.lookAt( mEye, mCenter, mUp ); //NOTE: new camera
//    initialCam.lookAt(Vec3f(0, 43, 0), Vec3f(0, 0, 0), Vec3f(0, -1, 0)); //NOTE: orginal camera
    
    mMayaCam.setCurrentCam( initialCam );
    
    
    
    
    mWaterModule = new WaterModule();
    mWaterModule->setup();
    
    mFlowField = new VectorFlowFieldApp();
    mFlowField->setup();
}

void wellingtonModelApp::mouseDown( MouseEvent event )
{
    /*
    if( event.isAltDown() )
		mMayaCam.mouseDown( event.getPos() );
	else
		mArcball.mouseDown( event.getPos() );
     */
    mWaterModule->mouseDown( event );
}

void wellingtonModelApp::mouseDrag(MouseEvent event)
{
    /*
    mouseMove(event);
    
    if( event.isAltDown() )
		mMayaCam.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
	else
		mArcball.mouseDrag( event.getPos() );
     */
    mWaterModule->mouseDrag(event);
}


void wellingtonModelApp::frameCurrentObject()
{
    Sphere boundingSphere = Sphere::calculateBoundingSphere( mMesh.getVertices() );
	
	mMayaCam.setCurrentCam( mMayaCam.getCamera().getFrameSphere( boundingSphere, 100 ) );
    
}

void wellingtonModelApp::keyDown(KeyEvent event)
{
    if(event.getCode() == 27){
        if(mWaterModule != NULL){
            delete mWaterModule;
        }
        exit(0);
    }
    if(event.getChar() == 'p'){
        userIncr+= 1.0f;
    }
    if(event.getChar() == 'o'){
        userIncr -= 1.0f;
    }
    if(event.getChar() == 'w'){
        drawWater = !drawWater;
    }
    if(event.getChar() == 'm'){
        drawMesh = !drawMesh;
    }
}

void wellingtonModelApp::update()
{
    mFlowField->update();
    
}

void wellingtonModelApp::draw()
{
    
    gl::enableDepthRead();
    gl::enableDepthWrite();
    gl::enableAlphaBlending();
    
    // clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices( mMayaCam.getCamera());


    //            /*
    if(drawWater == true){
        if(mWaterModule != NULL){
            gl::pushMatrices();
            mWaterModule->draw(userIncr);
            gl::popMatrices();
        }
    }
    //         */

//        /*
    if(drawMesh == true){
        gl::pushMatrices();
//        wellingtonShader.bind();
        myImage.enableAndBind();
//      gl::rotate( mArcball.getQuat() ); //NOTE: for debugging
        gl::scale(Vec3f(0.035,0.035,0.035));
        glLineWidth(0.2f);
        gl::enableWireframe();
        gl::translate(Vec3f(280.0, 0.0, -180.0));
        gl::rotate(Vec3f(-10.0, -10.0, 0.0));
        gl::draw(mVbo);
        gl::disableWireframe();
        myImage.unbind();
//        wellingtonShader.unbind();
        gl::popMatrices();
        }
//    */
    
    gl::pushMatrices();
    mFlowField->draw();
    gl::popMatrices();

}

//CINDER_APP_BASIC( wellingtonModelApp, RendererGl )
CINDER_APP_BASIC( wellingtonModelApp, RendererGl(RendererGl::AA_MSAA_32) )
