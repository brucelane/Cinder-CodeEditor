#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"

#include "CodeEditor.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicEditorApp : public App {
  public:
	void setup();
	void draw();
    
	// default vertex shader
	std::string		vert;
	gl::GlslProgRef mShader;
    CodeEditorRef   mCodeEditor;
};

void BasicEditorApp::setup()
{
	vert = loadString(loadAsset("shaders/simple.vert"));

    // Create CodeEditor
    mCodeEditor = CodeEditor::create( "shaders/simple.frag" );
    
    mCodeEditor->registerCodeChanged( "shaders/simple.frag", [this](const string& frag) {
        try {
			mShader = gl::GlslProg::create( vert.c_str(), frag.c_str());
            mCodeEditor->clearErrors();
        }
        catch( gl::GlslProgCompileExc exc ) {
            mCodeEditor->setError( "Simple: " + string( exc.what() ) );
        }
    } );
    
}


void BasicEditorApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    if( mShader ){
        gl::enableAlphaBlending();
        mShader->bind();
        gl::drawSolidRect( getWindowBounds() );
		gl::disableAlphaBlending();
    }
}

CINDER_APP( BasicEditorApp, RendererGl )
