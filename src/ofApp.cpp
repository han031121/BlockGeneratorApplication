#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);

	updateLayout();

	ofxBaseGui::loadFont("Roboto-Regular.ttf", 20);

	guiBlockSetup();
	guiDrawSetup();

	block_data = std::make_unique<blockData>(block_count_1, block_count_2, max_r, max_c, max_h);
	block_data->generateBlock();
	draw_object = std::make_unique<drawObject>(block_data.get(), 1000, 1000);
}

//--------------------------------------------------------------
void ofApp::updateLayout() {
	float w = ofGetWidth();
	float h = ofGetHeight();

	rect_block_gui.set(margin, margin, gui_width, h - 2 * margin);
	rect_draw_gui.set(gui_width + 2 * margin, margin, gui_width, h - 2 * margin);
	rect_image.set(gui_width * 2 + 3 * margin, margin, w - 2 * gui_width - 4 * margin, h - 2 * margin);


}

//--------------------------------------------------------------
void ofApp::initalizeUiValue() {
	cam_degree.set({ 25, 20 });
	light_degree.set({ 25, 38 });
	thickness.set(2);
	draw_color.set(ofColor(220, 185, 154));
}

//--------------------------------------------------------------
void ofApp::guiBlockSetup() {
	block_settings.setName("Block Settings");
	block_generation.setName("Block Generation");

	//set_block.addListener(this,);
	//generate_block.addListener(this,);
	max_r.addListener(this, &ofApp::maxSizeChanged);
	max_c.addListener(this, &ofApp::maxSizeChanged);
	max_h.addListener(this, &ofApp::maxSizeChanged);
	block_count_1.addListener(this, &ofApp::minBlockCountChanged);
	block_count_2.addListener(this, &ofApp::maxBlockCountChanged);

	block_settings.add(block_count_1.set("Min block count", 4, 1, 1000));
	block_settings.add(block_count_2.set("Max block count", 8, 1, 1000));
	block_settings.add(max_r.set("Max row", 3, 1, 10));
	block_settings.add(max_c.set("Max colunm", 3, 1, 10));
	block_settings.add(max_h.set("Max height", 3, 1, 10));
	block_settings.add(density.set("Density", 20, 0, 100));
	block_settings.add(allow_duplication.set("Allow duplication", false));
	block_generation.add(set_block.set("Set block"));
	block_generation.add(generate_block.set("Generate block"));

	gui_block.setup("BLOCK MENU", "block_settings.xml", rect_block_gui.x, rect_block_gui.y);
	gui_block.setWidthElements(gui_width);
	gui_block.setDefaultHeight(40);
	gui_block.add(block_settings);
	gui_block.add(block_generation);
}

//--------------------------------------------------------------
void ofApp::guiDrawSetup() {
	draw_settings.setName("Draw Settings");
	draw_functions.setName("Draw Functions");

	draw_settings.add(cam_degree.set("Cam degree", { 25, 20 }, { 0, 0 }, { 359, 359 }));
	draw_settings.add(light_degree.set("Light degree", { 25, 38 }, { 0, 0 }, { 359, 359 }));
	draw_settings.add(thickness.set("Line thickness", 2, 0, 10));
	draw_settings.add(cam_dist.set("Cam distance", 2000, 1000, 4000));
	draw_functions.add(save_image.set("Save image"));
	draw_functions.add(reset.set("Reset settings"));
	draw_functions.add(calculate_cam_dist.set("Set cam dist"));

	gui_draw.setup("DRAW MENU", "draw_settings.xml", rect_draw_gui.x, rect_draw_gui.y);
	gui_draw.setWidthElements(gui_width);
	gui_draw.setDefaultHeight(40);
	gui_draw.add(draw_settings);
	gui_draw.add(draw_color.set("Draw color", ofColor(220, 185, 154)));
	gui_draw.add(draw_functions);
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
	ofDrawRectangle(rect_block_gui);
	ofDrawRectangle(rect_draw_gui);
	ofSetColor(220);
	ofDrawRectangle(rect_image);

	if (draw_object) {
		float size = std::min(rect_image.getWidth(), rect_image.getHeight());
		float x = (2 * rect_image.x + rect_image.getWidth() - size) / 2;
		float y = (2 * rect_image.y + rect_image.getHeight() - size) / 2;
		ofSetColor(255);
		draw_object->drawFbo(x, y, size, size);
	}

	gui_block.draw();
	gui_draw.draw();
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
