#include "testApp.h"

ofImage tester;

void testApp::setup() 
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetWindowPosition(0,0);
    ofSetFrameRate(60);
    ofBackground(50);
    
    ofxOpenNI2GrabberSettings settings;
//    settings.doRegisterDepthToColor = false;
    openNI.setup(settings);
    openNI.startThread();
    
    string config = ofToDataPath("camara_lucida/config.xml");

    _depthmap = new cml::Depthmap_openni2();
    _cml = new cml::CamaraLucida( config, _depthmap );
    _cml->renderer->next_view();

    ofAddListener(_cml->render_texture, this, &testApp::render_texture);
    ofAddListener(_cml->render_3d, this, &testApp::render_3d);
    ofAddListener(_cml->render_2d, this, &testApp::render_2d);

}

void testApp::update()
{
    openNI.update();
    _depthmap->update( openNI.getDepthRawPixels().getPixels(), openNI.depthSource.videoStream );
}

void testApp::draw() 
{    
    _cml->render();
//    ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
//    ofScale(100,100);
//    _cml->mesh->render();
//
//    ofEventArgs someShit;
//    render_2d(someShit);
}

void testApp::render_texture(ofEventArgs &args)
{
    glClearColor(0.5, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float w = _cml->tex_width();
    float h = _cml->tex_height();

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);

    ofPushMatrix();
    
//    ofScale(-1,1);
//    ofTranslate(-ofGetWidth(), 0);
    
    ofPixels pix = *openNI.depthSource.currentPixels;
    int ind = 0;
    for (int i=0; i<pix.getWidth() * pix.getHeight() * pix.getNumChannels(); i++){
        pix[i] = pix[i] == 0 ? 0 : 255 - pix[i];
    }
    
    tester.setFromPixels(pix);
    tester.mirror(false, true);
    
    tester.draw(0,0,w,h);
    ofPopMatrix();
    
    glColor3f(1, 1, 0);
    ofCircle(800, 200, 60);
}

void testApp::render_3d(ofEventArgs &args)
{
    glTranslatef(-0.3, 0.3, 1);
    glColor3f(1, 1, 1);
    glutWireTeapot(0.1);
}

void testApp::render_2d(ofEventArgs &args)
{
    ofSetColor(255, 255, 255);
    
    ofPushMatrix();
    ofTranslate(15, 200);
    ofScale(200.0f/openNI.settings.width, 150.0f/openNI.settings.height);
    openNI.depthSource.draw();
    ofPopMatrix();
    
    ofDrawBitmapString("press h for help",10,10);
}

void testApp::exit() 
{
    ofLog(OF_LOG_VERBOSE, "exit!");

    ofRemoveListener(_cml->render_texture, this, &testApp::render_texture);
    ofRemoveListener(_cml->render_3d, this, &testApp::render_3d);
    ofRemoveListener(_cml->render_2d, this, &testApp::render_2d);

    _cml->dispose();
    _depthmap->dispose();
    
    openNI.close();
}

void testApp::keyPressed (int key) {
    switch (key) {
        case 'w':
            _cml->wireframe( ! _cml->wireframe() );
            break;
            
        case 'f':
            ofToggleFullscreen();
            break;
            
        case 'p':
            ofSetWindowPosition( ofGetWindowPositionX() == 0 ? 1440 : 0, 0 );
            break;
    }
}

void testApp::mouseDragged(int x, int y, int button)
{}

void testApp::mousePressed(int x, int y, int button)
{}

void testApp::mouseReleased(int x, int y, int button)
{}

void testApp::windowResized(int w, int h)
{}
