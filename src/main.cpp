#include "ofMain.h"
#include "ofApp.h"

#define IMAGE_SIZE 1000
#define HEIGHT_ADDITION 250
//========================================================================
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(IMAGE_SIZE + 840, IMAGE_SIZE + HEIGHT_ADDITION + 20);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();

}
