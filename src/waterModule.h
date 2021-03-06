
#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"


class waterModule{
public:
    waterModule();
    void draw();
    void mouseDown(ci::app::MouseEvent event);
    void mouseDrag(ci::app::MouseEvent event);
    void mouseUp(ci::app::MouseEvent event);
    void setup();
private:
    //Convenience method for drawing fullscreen rect
    //with tex coord
    void drawFullScreenRect();
    
    //FBOs to ping pong
    ci::gl::Fbo mFbo[2];
    size_t mFboIndex;
    
    //Shaders
    ci::gl::GlslProg mShaderGpGpu;
    ci::gl::GlslProg mShaderRefraction;
    
    //Refraction texture
    ci::gl::Texture mTexture;
    
    //Mouse
    ci::Vec2i mMouse;
    bool mMouseDown;
    
    //True renders input to screen
    bool mShowInput;                  //TODO: bring to main app level
};
