#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"

#include "CodeEditor.h"

// VisualStudio does'nt seem to support initializer_list
// yet so let's use boost::assign::list_of instead
#if defined( CINDER_MSW )
#include "boost/assign/list_of.hpp"
using namespace boost::assign;
#endif


using namespace ci;
using namespace ci::app;
using namespace std;

class MultipleFilesApp : public App {
public:
	void setup();
	void draw();
    
    gl::GlslProgRef mShader;
    CodeEditorRef   mCodeEditor;
};

void MultipleFilesApp::setup()
{
    // Create CodeEditor
#if defined( CINDER_MSW )
	mCodeEditor = CodeEditor::create( list_of<string>( "SphereShader.vert" )( "SphereShader.frag").convert_to_container<vector<fs::path>>(), CodeEditor::Settings().lineNumbers().autoSave() );
#else
    mCodeEditor = CodeEditor::create( { "SphereShader.vert", "SphereShader.frag" }, CodeEditor::Settings().lineNumbers().autoSave() );
#endif
    
    mCodeEditor->registerCodeChanged( "SphereShader.vert", "SphereShader.frag", [this](const string& vert,const string& frag) {
        try {
            mShader = gl::GlslProg::create( vert.c_str(), frag.c_str() );
            mCodeEditor->clearErrors();
        }
        catch( gl::GlslProgCompileExc exc ) {
            mCodeEditor->setError( "Sphere: " + string( exc.what() ) );
        }
    } );
    
}

void MultipleFilesApp::draw()
{
	gl::clear( Color( 1, 1, 1 ) );
    
    if( mShader ){
        gl::enableAlphaBlending();
        gl::enableWireframe();
        
        mShader->bind();
        gl::drawSphere( vec3( getWindowCenter().x, getWindowCenter().y, 0.0f ), 150.0f );
        
        gl::disableWireframe();
		gl::disableAlphaBlending();
    }
}

CINDER_APP( MultipleFilesApp, RendererGl )
