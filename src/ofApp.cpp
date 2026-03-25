#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);

	updateLayout();

	//set_block.addListener(this,);
	//generate_block.addListener(this,);
	max_r.addListener(this, &ofApp::maxSizeChanged);
	max_c.addListener(this, &ofApp::maxSizeChanged);
	max_h.addListener(this, &ofApp::maxSizeChanged);
	block_count_1.addListener(this, &ofApp::minBlockCountChanged);
	block_count_2.addListener(this, &ofApp::maxBlockCountChanged);

	gui_block.setup("Block", ofxPanelDefaultFilename, margin, margin);
	gui_block.setWidthElements(gui_width);
	gui_block.setDefaultHeight(40);
	gui_block.add(block_count_1.set("Min block count", 4, 1, 1000));
	gui_block.add(block_count_2.set("Max block count", 8, 1, 1000));
	gui_block.add(max_r.set("Max row", 3, 1, 10));
	gui_block.add(max_c.set("Max colunm", 3, 1, 10));
	gui_block.add(max_h.set("Max height", 3, 1, 10));
	gui_block.add(density.set("Density", 20, 0, 100));
	gui_block.add(allow_duplication.set("Allow duplication", false));
	gui_block.add(set_block.setup("Set block"));
	gui_block.add(generate_block.setup("Generate block"));

	block_data = std::make_unique<blockData>(block_count_1, block_count_2, max_r, max_c, max_h);
	block_data->generateBlock();
	draw_object = std::make_unique<drawObject>(block_data.get(), 1000, 1000);
}

//--------------------------------------------------------------
void ofApp::updateLayout() {
	float w = ofGetWidth();
	float h = ofGetHeight();

	rect_gui.set(margin, margin, gui_width, h - 2 * margin);
	rect_draw.set(gui_width + 2 * margin, margin, w - gui_width - 3 * margin, h - 2 * margin);


}

//--------------------------------------------------------------
void ofApp::initalizeUiValue() {
	cam_degree.set({ 25, 20 });
	light_degree.set({ 25, 38 });
	thickness.set(2);
	draw_color.set(ofColor(220, 185, 154));
}

//--------------------------------------------------------------
void ofApp::maxSizeChanged(int& v) {
	int max_count = max_r * max_c * max_h;
	block_count_1.setMax(max_count);
	block_count_1 = block_count_1.get();
	block_count_2.setMax(max_count);
	block_count_2 = block_count_2.get();

	if (block_count_1 > max_count)
		block_count_1 = max_count;
	if (block_count_2 > max_count)
		block_count_2 = max_count;
}

//--------------------------------------------------------------
void ofApp::maxBlockCountChanged(int & v) {
	if (v < block_count_1)
		block_count_1 = v;
}

//--------------------------------------------------------------
void ofApp::minBlockCountChanged(int & v) {
	if (v > block_count_2)
		block_count_2 = v;
}

//--------------------------------------------------------------
void ofApp::update(){
	if (draw_object)
		draw_object->render();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(50);
	ofDrawRectangle(rect_gui);
	ofSetColor(220);
	ofDrawRectangle(rect_draw);

	if (draw_object) {
		float size = std::min(rect_draw.getWidth(), rect_draw.getHeight());
		float x = (2 * rect_draw.x + rect_draw.getWidth() - size) / 2;
		float y = (2 * rect_draw.y + rect_draw.getHeight() - size) / 2;
		ofSetColor(255);
		draw_object->drawFbo(x, y, size, size);
	}

	gui_block.draw();
	//gui_draw.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
