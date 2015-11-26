#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class StarApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void StarApp::setup()
{
}

void StarApp::mouseDown( MouseEvent event )
{
}

void StarApp::update()
{
}

void StarApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( StarApp, RendererGl )
