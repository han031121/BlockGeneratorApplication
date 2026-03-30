#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);

	updateLayout();

	ofxBaseGui::loadFont("Roboto-Regular.ttf", 20);

	ofxGuiSetDefaultHeight(gui_height);
	guiBlockSetup();
	guiDrawSetup();

	block_data = std::make_unique<blockData>(block_count_1, block_count_2, max_r, max_c, max_h);
	block_data->generateBlock();
	draw_object = std::make_unique<drawObject>(block_data.get(), image_size, image_size);

	blockSettingUpdate();
	blockCurrentInfoUpdate();
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
void ofApp::initializeUiValue() {
	cam_degree_xz.set(25);
	cam_degree_y.set(20);
	light_degree_xz.set(25);
	light_degree_y.set(38);
	thickness.set(5);
	magnification.set(1);
	draw_color.set(ofColor(220, 185, 154));
}

//--------------------------------------------------------------
void ofApp::guiBlockSetup() {
	block_settings.setName("Block Settings");
	block_current_settings.setName("Current Settings");
	block_current_info.setName("Current block info");
	block_generation.setName("Block Generation");

	set_block.addListener(this, &ofApp::setBlockClicked);
	generate_block.addListener(this, &ofApp::generateBlockClicked);
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
	block_current_settings.add(block_count_setting.setup("Block count", ""));
	block_current_settings.add(max_size_setting.setup("Max size", ""));
	block_current_settings.add(density_setting.setup("Density", ""));
	block_current_settings.add(allow_duplication_setting.setup("Duplication", ""));
	block_current_info.add(block_count_current.setup("Block count", ""));
	block_current_info.add(size_current.setup("Size", ""));
	block_generation.add(set_block.set("Set block (N)"));
	block_generation.add(generate_block.set("Generate block (G)"));

	gui_block.setup("BLOCK MENU", "block_settings.xml", rect_block_gui.x, rect_block_gui.y);
	gui_block.setWidthElements(gui_width);
	gui_block.add(block_settings);
	gui_block.add(&block_current_settings);
	gui_block.add(&block_current_info);
	gui_block.add(block_generation);
}

//--------------------------------------------------------------
void ofApp::guiDrawSetup() {
	reset.addListener(this, &ofApp::initializeUiValue);
	save_image.addListener(this, &ofApp::saveImageClicked);

	draw_settings.setName("Draw Settings");
	draw_functions.setName("Draw Functions");
	cam_degree.setName("Cam degree");
	light_degree.setName("Light degree");

	cam_degree.add(cam_degree_xz.set("Horizontal", 25, 0, 359.99));
	cam_degree.add(cam_degree_y.set("Vertical", 20, -89.99, 89.99));
	light_degree.add(light_degree_xz.set("Horizontal", 25, 0, 359.99));
	light_degree.add(light_degree_y.set("Vertical", 38, -89.99, 89.99));

	draw_settings.add(cam_degree);
	draw_settings.add(light_degree);
	draw_settings.add(thickness.set("Line thickness", 5, 0, 10));
	draw_settings.add(magnification.set("Magnification", 1, 0.2, 5));
	draw_functions.add(save_image.set("Save image (S)"));
	draw_functions.add(reset.set("Reset settings (R)"));

	gui_draw.setup("DRAW MENU", "draw_settings.xml", rect_draw_gui.x, rect_draw_gui.y);
	gui_draw.setWidthElements(gui_width);
	gui_draw.setDefaultHeight(40);
	gui_draw.add(draw_settings);
	gui_draw.add(draw_color.set("Draw color", ofColor(220, 185, 154)));
	gui_draw.add(draw_functions);
}

//--------------------------------------------------------------
void ofApp::drawObjectUpdate() {
	draw_object->camDegreeUpdate(cam_degree_xz.get(), cam_degree_y.get());
	draw_object->lightDegreeUpdate(light_degree_xz.get(), light_degree_y.get());
	draw_object->thicknessUpdate(thickness.get());
	draw_object->camDistUpdate(1 / magnification.get() * draw_object->getDefaultCamDist());
	draw_object->blockColorUpdate(draw_color.get().r, draw_color.get().g, draw_color.get().b);
}

//--------------------------------------------------------------
void ofApp::blockSettingUpdate() {
	block_count_setting = ofToString(block_count_1) + "~" + ofToString(block_count_2);
	max_size_setting = ofToString(max_r) + "x" + ofToString(max_c) + "x" + ofToString(max_h);
	density_setting = ofToString(density);
	if (allow_duplication)
		allow_duplication_setting = "Allowed";
	else
		allow_duplication_setting = "Not allowed";
}

//--------------------------------------------------------------
void ofApp::blockCurrentInfoUpdate() {
	if (!block_data)
		return;
	block_count_current = ofToString(block_data->getBlockCount());
	size_current = ofToString(block_data->getSizeRow()) + "x" + ofToString(block_data->getSizeCol()) + "x" + ofToString(block_data->getSizeHeight());
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
void ofApp::setBlockClicked() {
	if (block_data)
		block_data.reset();
	if (draw_object)
		draw_object.reset();

	block_data = std::make_unique<blockData>(block_count_1.get(), block_count_2.get(), max_r.get(), max_c.get(), max_h.get(), density.get(), allow_duplication.get());

	if (block_data)
		blockSettingUpdate();
}

//--------------------------------------------------------------
void ofApp::generateBlockClicked() {
	if (!block_data)
		return;
	if (draw_object)
		draw_object.reset();
	block_data->generateBlock();
	draw_object = std::make_unique<drawObject>(block_data.get(), image_size, image_size);
	blockCurrentInfoUpdate();
}

//--------------------------------------------------------------
void ofApp::drawResetClicked() {
	initializeUiValue();
}

//--------------------------------------------------------------
void ofApp::saveImageClicked() {
	if (!draw_object)
		return;
	std::string name = "images/" + draw_object->getIdentify() + ".jpg";
	draw_object->saveImage(name);
}

//--------------------------------------------------------------
void ofApp::update(){
	if (draw_object) {
		drawObjectUpdate();
		draw_object->render();
	}
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
	if (key == 'n' || key == 'N') {
		setBlockClicked();
	} else if (key == 'g' || key == 'G') {
		generateBlockClicked();
	} else if (key == 's' || key == 'S') {
		saveImageClicked();
	} else if (key == 'r' || key == 'R') {
		drawResetClicked();
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
