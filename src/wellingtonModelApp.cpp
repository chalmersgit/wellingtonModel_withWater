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
    
    
    gl::Texture myImage;
    gl::VboMesh mVbo;
    
    Channel32f mChannel;
    TriMesh mMesh;
    Arcball mArcball;
    MayaCamUI mMayaCam;
    
    waterModule *mWaterModule;
    
    //Mouse
    ci::Vec2i mMouse;
    bool mMouseDown;
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
    
    //Set flags                     <-water module
    mMouse = Vec2i::zero();
    mMouseDown = false;
    
    
    //setFullScreen(true);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
//        myImage = gl::Texture(loadImage(loadResource(RES_WELLINGTON_IMG)));
    myImage = gl::Texture(loadImage(loadFile("/Users/oliverellmers/Desktop/Cinder Projects/Projects/wellingtonModel_withWater/resources/wellington.jpg")));
    
    
    //    mChannel = Channel32f(loadImage(loadResource("wellington.jpg")));
    //    mTexture = myImage;
    
    
    ObjLoader loader(loadFile("/Users/oliverellmers/Desktop/Cinder Projects/Projects/wellingtonModel_withWater/resources/wellington01.obj"));
    loader.load(&mMesh);
    mVbo = gl::VboMesh(mMesh);
    
    CameraPersp initialCam;
    initialCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.1, 50000 ); //TODO: get correct camera persp from C4D
    mMayaCam.setCurrentCam( initialCam );
    
    
    mWaterModule = new waterModule();
    mWaterModule->setup();
}

void wellingtonModelApp::mouseDown( MouseEvent event )
{
//    /*
    if( event.isAltDown() )
		mMayaCam.mouseDown( event.getPos() );
	else
		mArcball.mouseDown( event.getPos() );
//     */
}

void wellingtonModelApp::mouseDrag(MouseEvent event)
{
//    /*
    mouseMove(event);
    
    if( event.isAltDown() )
		mMayaCam.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
	else
		mArcball.mouseDrag( event.getPos() );
//     */
}

//TODO: bring mouse events in from waterModule to here

/*
 void banTheRewindApp::mouseDown(MouseEvent event)
 {
 mMouseDown = true;
 mouseDrag(event);
 }
 
 void banTheRewindApp::mouseDrag(MouseEvent event)
 {
 mMouse = event.getPos();
 }
 
 void banTheRewindApp::mouseUp(MouseEvent event)
 {
 mMouseDown = false;
 }
 
 */


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
}

void wellingtonModelApp::update()
{
}

void wellingtonModelApp::draw()
{
    
    gl::enableDepthRead();
    gl::enableDepthWrite();
    gl::enableAlphaBlending();
    
    // clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    /*
     if(mWaterModule != NULL){
     gl::pushMatrices();
     mWaterModule->draw();
     gl::popMatrices();
     }
     */
    
    
//    /*
    gl::setMatrices( mMayaCam.getCamera());
    
    gl::pushMatrices();
    myImage.enableAndBind();
    gl::rotate( mArcball.getQuat() );
    gl::scale(Vec3f(0.035,0.035,0.035));
    glLineWidth(0.3f);
    gl::enableWireframe();
    gl::rotate(Vec3f(50.0, -20.0, 0.0));
    gl::draw(mVbo);
    myImage.unbind();
    gl::popMatrices();
     
//     */
    
 
    
}

//CINDER_APP_BASIC( wellingtonModelApp, RendererGl )
CINDER_APP_BASIC( wellingtonModelApp, RendererGl(RendererGl::AA_MSAA_32) )
