#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/Arcball.h"
#include "cinder/CameraUi.h"
#include "cinder/params/Params.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"

#include "Controller.h"
#include "Glow.h"
#include "Star.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class StarApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void keyDown( KeyEvent event) override;
	void update() override;
	void draw() override;
    
    //CAMERA
    CameraPersp					mCamera;
    CameraUi					mCameraCtrl;
    void                        setupCamera();
    
    //STAR
    Star				mStar;
    float               mStartColor;
    
    //Sphere
    void createSphere();
    gl::BatchRef	mSphereBatch;
    float           mRadius;
    
    
    bool            mLights;
    
    //PASS
    void drawRenderPass();
    int  mRenderPass;
    
    void drawCorona();
    void drawSphere();
    void drawGlows();
    
    
    
    // SHADERS  and   TEXTURES
    void loadTextures();
    void loadShaders();

    gl::GlslProgRef		mGradientShader;
    gl::GlslProgRef		mRoomShader;
    gl::GlslProgRef		mStarShader;
    gl::GlslProgRef		mGlowShader;
    gl::GlslProgRef		mNebulaShader;
    gl::GlslProgRef		mDustShader;
    gl::GlslProgRef		mCoronaShader;
    gl::GlslProgRef		mPlanetShader;
    
    gl::Texture2dRef	mSpectrumTex;
    gl::Texture2dRef	mGlowTex;
    gl::Texture2dRef	mNebulaTex;
    gl::Texture2dRef	mCoronaTex;
    gl::Texture2dRef	mGridTex;
    gl::Texture2dRef	mSmallGridTex;
    gl::Texture2dRef	mBigGlow0Tex;
    gl::Texture2dRef	mBigGlow1Tex;
    gl::Texture2dRef	mIconTex;
    
    // CONTROLLER
    Controller			mController;
    void                updateParticles();
    
    //Params
    params::InterfaceGlRef		mParams;
};

void StarApp::setup()
{
    setWindowSize(1024, 768);
    
    mRenderPass = 1;
    
    loadShaders();
    loadTextures();
    
    createSphere();
    
    setupCamera();
    
}

void StarApp::createSphere()
{
    
    mStar				= Star( vec3(0), 4000000.0f );
    
    auto lambert = gl::ShaderDef().lambert().color();
    gl::GlslProgRef shader = gl::getStockShader( lambert );
    
    
    float solarRadius = mStar.mMaxRadius * 0.25f;
    mStartColor = 0.6f;
    
    console()<<"solar radius: "<<solarRadius<<std::endl;
    
    //mRadius = 0.1f;
    int sphereResolution = 32;
    mSphereBatch = gl::Batch::create(geom::Sphere().radius(solarRadius).subdivisions(sphereResolution), shader );
    
    
    mLights = false;
}

void StarApp::setupCamera()
{
    float verticalFOV = 45.0f;
    float aspectRatio = getWindowAspectRatio();
    float nearClip = 0.1f;
    float farClip = 50.0f;
    vec3 cameraPosition = vec3(0, 0.1, -1);
    vec3 cameraTarget = vec3(0);
    vec3 cameraUpAxis = vec3(0, 1, 0);
    
    mCamera.setPerspective(verticalFOV, aspectRatio, nearClip, farClip);
    mCamera.lookAt(cameraPosition, cameraTarget, cameraUpAxis);
    mCameraCtrl = CameraUi(&mCamera, getWindow());
}

void StarApp::loadShaders()
{
    // LOAD SHADERS
    try {
        //mGradientShader = gl::GlslProg::create( loadAsset( "passThru.vert" ), loadAsset( "gradient.frag" ) );
        //mRoomShader		= gl::GlslProg::create( loadAsset( "room.vert" ), loadAsset( "room.frag" ) );
        //mStarShader		= gl::GlslProg::create( loadAsset( "star.vert" ), loadAsset( "star.frag" ) );
        mGlowShader		= gl::GlslProg::create( loadAsset( "passThru.vert" ), loadAsset( "glow.frag" ) );
        //mNebulaShader	= gl::GlslProg::create( loadAsset( "passThru.vert" ), loadAsset( "nebula.frag" ) );
        mCoronaShader	= gl::GlslProg::create( loadAsset( "passThru.vert" ), loadAsset( "corona.frag" ) );
        //mDustShader		= gl::GlslProg::create( loadAsset( "passThruColor.vert" ), loadAsset( "dust.frag" ) );
        //mPlanetShader	= gl::GlslProg::create( loadAsset( "passThruNormals.vert" ), loadAsset( "planet.frag" ) );
    } catch( gl::GlslProgCompileExc e ) {
        std::cout << e.what() << std::endl;
        //quit();
    }
}


void StarApp::loadTextures()
{
    mSpectrumTex		= gl::Texture2d::create( loadImage( loadAsset( "spectrum.png" ) ) );
    mGlowTex			= gl::Texture2d::create( loadImage( loadAsset( "glow.png" ) ) );
    mNebulaTex			= gl::Texture2d::create( loadImage( loadAsset( "nebula.png" ) ) );
    mCoronaTex			= gl::Texture2d::create( loadImage( loadAsset( "corona.png" ) ) );
    mGridTex			= gl::Texture2d::create( loadImage( loadAsset( "grid.png" ) ) );
    mSmallGridTex		= gl::Texture2d::create( loadImage( loadAsset( "smallGrid.png" ) ) );
    mBigGlow0Tex		= gl::Texture2d::create( loadImage( loadAsset( "bigGlow0.png" ) ) );
    mBigGlow1Tex		= gl::Texture2d::create( loadImage( loadAsset( "bigGlow1.png" ) ) );
    mIconTex			= gl::Texture2d::create( loadImage( loadAsset( "iconStar.png" ) ) );
}

void StarApp::mouseDown( MouseEvent event )
{
    
}

void StarApp::keyDown(KeyEvent event)
{
    switch (event.getChar()) {
        case '1':
            mRenderPass = 1;
             console()<<mRenderPass<<std::endl;
            break;
        case '2':
            mRenderPass = 2;
            console()<<mRenderPass<<std::endl;
            break;
        case '3':
            mRenderPass = 3;
             console()<<mRenderPass<<std::endl;
            break;
        case '4':
            mRenderPass = 4;
            break;
        case '5':
            mRenderPass = 5;
            break;
        case 'a':
            mLights = !mLights;
            break;
        default:
            break;
    }
}

void StarApp::update()
{
    updateParticles();
}

void StarApp::updateParticles()
{


}

void StarApp::drawCorona()
{
    gl::color( ColorA( 1, 1, 1, 1 ) );
    float radius = mRadius * 1.8f;
    
    gl::ScopedGlslProg scpGlsl(mCoronaShader);
    gl::ScopedTextureBind scpCorina(mCoronaTex, 0);
    gl::ScopedTextureBind scpSpectrum(mCoronaTex, 1);
    
    mCoronaShader->uniform( "coronaTex", 0 );
    mCoronaShader->uniform( "spectrumTex", 1 );
    mCoronaShader->uniform( "starColor", mStartColor );
    mCoronaShader->uniform( "power",  mLights );
    gl::drawSolidRect( Rectf( -radius, -radius, radius, radius ) );
}

void StarApp::drawGlows()
{
    
    gl::ScopedGlslProg scpGlsl(mGlowShader);
    gl::ScopedTextureBind scpGlow(mGlowTex, 0);
    gl::ScopedTextureBind scpSmallGrid(mSmallGridTex, 1);
    gl::ScopedTextureBind scpSpectrumTex(mSpectrumTex, 2);
    
    
    mGlowShader->uniform( "glowTex", 0 );
    mGlowShader->uniform( "gridTex", 1 );
    mGlowShader->uniform( "spectrumTex", 2 );
    mGlowShader->uniform( "color", mStartColor );
    mGlowShader->uniform( "power", mLights );
    mGlowShader->uniform( "starRadius", mStar.mRadiusDrawn );
    vec3 right = vec3(1, 0, 0);
    vec3 up	= vec3(0, 1, 0);
    
    mController.drawGlows( mGlowShader, right, up );
}

void StarApp::drawSphere()
{
    gl::ScopedMatrices mat;
    gl::setMatrices(mCamera);
    
   // gl::ScopedDepth depth(true, true);
    
    mSphereBatch->draw();
}

void StarApp::drawRenderPass()
{
    switch (mRenderPass) {
        case 1:
            drawSphere();
            break;
        case 2:
            drawCorona();
            break;
        case 3:
            drawGlows();
            break;
        default:
            break;
    }
}

void StarApp::draw()
{
	gl::clear( Color( 0.2, 0.2, 0.2 ) );

    
    gl::enableDepthWrite();
    gl::enableDepthRead();
    
    {
        gl::ScopedMatrices mat;
        gl::setMatrices(mCamera);
    
        drawRenderPass();
    }
    
}

CINDER_APP( StarApp, RendererGl )
