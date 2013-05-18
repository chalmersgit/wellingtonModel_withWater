
#include "waterModule.h"
#include "wellingtonModelApp.cpp"

#include "cinder/ImageIo.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const Vec2i kWindowSize = Vec2i(1280, 720);
const Vec2f kPixel = Vec2f::one() / Vec2f(kWindowSize);

waterModule::waterModule(){} //TODO: change all to upperclass W for constructors

void waterModule::mouseDown(MouseEvent event)
{
    mMouseDown = true;
    mouseDrag(event);
}

void waterModule::mouseDrag(MouseEvent event)
{
    mMouse = event.getPos();
}

void waterModule::mouseUp(MouseEvent event)
{
    mMouseDown = false;
}

void waterModule::setup()
{
    //Set flags                     TODO:bring to main app level
    mMouse = Vec2i::zero();
    mMouseDown = false;
    mShowInput = false;
    
    //Load shaders
    try{
        mShaderGpGpu = gl::GlslProg(loadResource(RES_PASS_THRU_VERT), loadResource(RES_GPGPU_FRAG));
    }catch(gl::GlslProgCompileExc ex){
        console() << "Unable to compile GPGPU shader:\n" << ex.what() << "\n";
        return;
    }
    try{
        mShaderRefraction = gl::GlslProg(loadResource(RES_PASS_THRU_VERT), loadResource(RES_REFRACTION_FRAG));
    }catch(gl::GlslProgCompileExc ex){
        console() << "Unable to compile refraction shader:\n" << ex.what() << "\n";
        return;
    }
    
    //Load refraction texture
    {
        gl::Texture::Format format;
        format.setInternalFormat(GL_RGBA32F_ARB);
        mTexture = gl::Texture(loadImage(loadResource(RES_TEXTURE)));
        mTexture.setWrap(GL_REPEAT, GL_REPEAT);
    }
    
    {
        //Set up format with 32-bit color for high resolution data
        gl::Fbo::Format format;
        format.enableColorBuffer(true);
        format.enableDepthBuffer(false);
        format.setColorInternalFormat(GL_RGBA32F_ARB);
        
        //Create two frame buffer objects to ping ping
        mFboIndex = 0;
        for(size_t n = 0; n < 2; ++n){
            mFbo[n] = gl::Fbo(kWindowSize.x, kWindowSize.y, format);
            mFbo[n].bindFramebuffer();
            gl::setViewport(mFbo[n].getBounds());
            gl::clear();
            mFbo[n].unbindFramebuffer();
            mFbo[n].getTexture().setWrap(GL_REPEAT,GL_REPEAT);
        }
    }
}

void waterModule::drawFullScreenRect() //TODO: make the mesh 2158 × 1540 pixels to match mesh
{
    //Begin drawing
    gl::begin(GL_TRIANGLES);
    
    //Define quad vertices
    Area bounds = getWindowBounds();
    Vec2f vert0((float)bounds.x1, (float)bounds.y1 );
    Vec2f vert1((float)bounds.x2, (float)bounds.y1 );
    Vec2f vert2((float)bounds.x1, (float)bounds.y2 );
    Vec2f vert3((float)bounds.x2, (float)bounds.y2 );
    
    //Define quad texture coords
    Vec2f uv0(0.0f, 0.0f);
    Vec2f uv1(1.0f, 0.0f);
    Vec2f uv2(0.0f, 1.0f);
    Vec2f uv3(1.0f, 1.0f);
    
    //Draw quad (two triangles)
    gl::texCoord(uv0);
    gl::vertex(vert0);
    gl::texCoord(uv2);
    gl::vertex(vert2);
    gl::texCoord(uv1);
    gl::vertex(vert1);
    
    gl::texCoord(uv1);
    gl::vertex(vert1);
    gl::texCoord(uv2);
    gl::vertex(vert2);
    gl::texCoord(uv3);
    gl::vertex(vert3);
    
    //end drawing
    gl::end();
}

void waterModule::draw()
{
    
    //GPGPU pass
    
    //Enable textures
    gl::enable(GL_TEXTURE_2D);
    gl::color(Colorf::white());
    
    //Bind the other FBO to draw onto it
    size_t pong = (mFboIndex+1) % 2;
    mFbo[pong].bindFramebuffer();
    
    //set up the window to match the FBO
    gl::setViewport(mFbo[mFboIndex].getBounds());
    gl::setMatricesWindow(mFbo[mFboIndex].getSize(), false);
    gl::clear();
    
    
    //Bind the texture from the FBO on which we last wrote data
    mFbo[mFboIndex].bindTexture();
    
    //Bind and configure GPU shader
    mShaderGpGpu.bind();
    mShaderGpGpu.uniform("buffer", 0);
    mShaderGpGpu.uniform("pixel", kPixel);
    
    //Draw a fullscreen rectangle to process data
    drawFullScreenRect();
    
    //End shader output
    mShaderGpGpu.unbind();
    
    //Unbind and disable textures
    mFbo[mFboIndex].unbindTexture();
    gl::disable(GL_TEXTURE_2D);
    
    //Draw mouse input into red channel
    if(mMouseDown){
        gl::color(ColorAf(1.0f, 0.0f, 0.0f, 1.0f));
        gl::drawSolidCircle(Vec2f(mMouse), 4.0f, 32);
        gl::color(Color::white());
    }
    
    //Stop drawing to FBO
    mFbo[pong].unbindFramebuffer();
    
    //Swap FBO's
    mFboIndex = pong;
    
    ////////////////////////////////////////////////////
    
    //Refraction pass
    
    //clear screen and set to viewport
    gl::clear(Color::black());
    gl::setViewport(getWindowBounds());
    gl::setMatricesWindow(getWindowSize());
    
    //this flag draws the raw data without refraction
    if(mShowInput){
        gl::draw(mFbo[mFboIndex].getTexture());
    }else{
        //bind the FBO we last rendered as a texture
        mFbo[mFboIndex].bindTexture(0, 0);
        
        //bind and enable the refraction texture
        gl::enable(GL_TEXTURE_2D);
        mTexture.bind(1);
        
        //bind and configure the refraction shader
        mShaderRefraction.bind();
        mShaderRefraction.uniform("buffer", 0);
        mShaderRefraction.uniform("pixel", kPixel);
        mShaderRefraction.uniform("tex", 1);
        
        //fill the screen with the shader output
        drawFullScreenRect();
        
        //unbind and disable the texture
        mTexture.unbind();
        gl::disable(GL_TEXTURE_2D);
        
        //end shader output
        mShaderRefraction.unbind();
        
    }
    
}


//CINDER_APP_BASIC( waterModule, RendererGl(RendererGl::AA_MSAA_32) )
