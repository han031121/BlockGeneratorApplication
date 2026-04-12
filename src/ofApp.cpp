#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetEscapeQuitsApp(false);

	ofBackground(0);

	//set initial value
	initializeDrawValue();
	initializeBlockValue();
	loadGuiSettings();

	//drawing GUI
	updateLayout();
	guiSetListener();
	guiSetScale();

	//generate objects
	block_data = std::make_unique<blockData>(block_count_1, block_count_2, max_r, max_c, max_h);
	//draw_object = std::make_unique<drawObject>(block_data.get(), image_size, image_size);

	//update label gui
	blockSettingUpdate();
	blockCurrentInfoUpdate();
}

//--------------------------------------------------------------
void ofApp::update() {
	statusUpdate();

	if (generate_request && generate_status_changed) {
		block_data->generateBlock();
		draw_object = std::make_unique<drawObject>(block_data.get(), image_size, image_size);
		blockCurrentInfoUpdate();
		generate_request = false;
		generate_status_changed = false;
	}

	if (draw_object) {
		drawObjectUpdate();
		draw_object->render();
	}

	if (fix_light) {
		light_degree_xz = cam_degree_xz;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	//draw image components
	ofSetColor(220);
	ofDrawRectangle(rect_image);

	if (draw_object) {
		float size = std::min(rect_image.getWidth(), rect_image.getHeight());
		float x = (2 * rect_image.x + rect_image.getWidth() - size) / 2;
		float y = (2 * rect_image.y + rect_image.getHeight() - size) / 2;
		ofSetColor(255);
		draw_object->drawFbo(x, y, size, size);
	}

	//draw gui components
	if (gui_on) {
		ofSetColor(50);
		ofDrawRectangle(rect_block_gui);
		ofDrawRectangle(rect_draw_gui);

		ofNoFill();
		ofSetLineWidth(3);
		ofDrawRectangle(rect_status_block);
		ofDrawRectangle(rect_status_draw);
		ofFill();

		gui_block.draw();
		gui_draw.draw();

		ofSetColor(255);
		drawStatus();
		drawEtc();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//set block
	if (key == 'n' || key == 'N') {
		setBlockClicked();
	}

	//generate block
	else if (key == 'g' || key == 'G') {
		generateBlockClicked();
	}

	//save image
	else if (key == 's' || key == 'S') {
		saveImageClicked();
	}

	//draw reset
	else if (key == 'r' || key == 'R') {
		drawResetClicked();
	}

	//gui scale up
	else if (key == '=' && gui_on) {
		if (gui_scale < 8) {
			gui_scale += 0.5;
			guiSetScale();
		}
	}

	//gui scale down
	else if (key == '-' && gui_on) {
		if (gui_scale > 1) {
			gui_scale -= 0.5;
			guiSetScale();
		}
	}

	//gui toggle
	else if (key == OF_KEY_TAB) {
		gui_on = !gui_on;
		updateLayout();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	updateLayout();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit() {
	saveGuiSettings();
}
