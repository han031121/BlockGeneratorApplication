#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::updateLayout() {
	float w = ofGetWidth();
	float h = ofGetHeight();

	if (gui_on) {
		rect_block_gui.set(margin, margin, gui_width, h - 3 * margin - status_height);
		rect_draw_gui.set(gui_width + 2 * margin, margin, gui_width, h - 3 * margin - status_height);
		rect_image.set(gui_width * 2 + 3 * margin, margin, w - 2 * gui_width - 4 * margin, h - 2 * margin);
		rect_status_block.set(rect_block_gui.x, h - margin - status_height, gui_width, status_height);
		rect_status_draw.set(rect_draw_gui.x, h - margin - status_height, gui_width, status_height);
	} else {
		rect_image.set(margin, margin, w - 2 * margin, h - 2 * margin);
	}
}

//--------------------------------------------------------------
void ofApp::initializeBlockValue() {
	block_count_1.set("Min block count", 4, 1, 1000);
	block_count_2.set("Max block count", 8, 1, 1000);
	max_r.set("Max row", 3, 1, 10);
	max_c.set("Max colunm", 3, 1, 10);
	max_h.set("Max height", 3, 1, 10);
	density.set("Density", 20, 0, 100);
	allow_duplication.set("Allow duplication", false);

	set_block.set("Set block (N)");
	generate_block.set("Generate block (G)");

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
void ofApp::initializeDrawValue() {
	cam_degree_xz.set("Horizontal", 25, 0, 359.99);
	cam_degree_y.set("Vertical", 20, -89.99, 89.99);
	light_degree_xz.set("Horizontal", 25, 0, 359.99);
	light_degree_y.set("Vertical", 38, -89.99, 89.99);

	draw_color.set("Draw color", ofColor(220, 185, 154));
	thickness.set("Line thickness", 5, 0, 15);
	magnification.set("Magnification", 1, 0.2, 5);
	fix_light.set("Fix light degree", false);

	save_image.set("Save image (S)");
	reset.set("Reset settings (R)");
}

//--------------------------------------------------------------
void ofApp::guiBlockSetup() {
	block_settings.setName("Block Settings");
	block_current_settings.setName("Current Settings");
	block_current_info.setName("Current block info");
	block_generation.setName("Block Generation");

	block_current_settings.setDefaultHeight(gui_height);
	block_current_info.setDefaultHeight(gui_height);

	block_settings.add(block_count_1);
	block_settings.add(block_count_2);
	block_settings.add(max_r);
	block_settings.add(max_c);
	block_settings.add(max_h);
	block_settings.add(density);
	block_settings.add(allow_duplication);
	block_current_settings.add(block_count_setting.setup("Block count", ""));
	block_current_settings.add(max_size_setting.setup("Max size", ""));
	block_current_settings.add(density_setting.setup("Density", ""));
	block_current_settings.add(allow_duplication_setting.setup("Duplication", ""));
	block_current_info.add(block_count_current.setup("Block count", ""));
	block_current_info.add(size_current.setup("Size", ""));
	block_generation.add(set_block);
	block_generation.add(generate_block);

	gui_block.setup("BLOCK MENU", "block_settings.xml", rect_block_gui.x, rect_block_gui.y);
	gui_block.setWidthElements(gui_width);
	gui_block.add(block_settings);
	gui_block.add(&block_current_settings);
	gui_block.add(&block_current_info);
	gui_block.add(block_generation);
}

//--------------------------------------------------------------
void ofApp::guiDrawSetup() {
	draw_settings.setName("Draw Settings");
	draw_functions.setName("Draw Functions");
	cam_degree.setName("Cam degree");
	light_degree.setName("Light degree");

	cam_degree.add(cam_degree_xz);
	cam_degree.add(cam_degree_y);
	light_degree.add(light_degree_xz);
	light_degree.add(light_degree_y);

	draw_settings.add(cam_degree);
	draw_settings.add(light_degree);
	draw_settings.add(thickness);
	draw_settings.add(magnification);
	draw_settings.add(fix_light);
	draw_functions.add(save_image);
	draw_functions.add(reset);

	gui_draw.setup("DRAW MENU", "draw_settings.xml", rect_draw_gui.x, rect_draw_gui.y);
	gui_draw.setWidthElements(gui_width);
	gui_draw.add(draw_settings);
	gui_draw.add(draw_color);
	gui_draw.add(draw_functions);
}

//--------------------------------------------------------------
void ofApp::guiSetListener() {
	set_block.addListener(this, &ofApp::setBlockClicked);
	generate_block.addListener(this, &ofApp::generateBlockClicked);
	max_r.addListener(this, &ofApp::maxSizeChanged);
	max_c.addListener(this, &ofApp::maxSizeChanged);
	max_h.addListener(this, &ofApp::maxSizeChanged);
	block_count_1.addListener(this, &ofApp::minBlockCountChanged);
	block_count_2.addListener(this, &ofApp::maxBlockCountChanged);

	reset.addListener(this, &ofApp::initializeDrawValue);
	save_image.addListener(this, &ofApp::saveImageClicked);
}

//--------------------------------------------------------------
void ofApp::guiSetScale() {
	gui_width = WIDTH_UNIT * gui_scale;
	gui_height = HEIGHT_UNIT * gui_scale;
	status_height = STATUS_HEIGHT_UNIT * gui_scale;
	margin = MARGIN_UNIT * gui_scale;

	updateLayout();

	ofxGuiSetDefaultHeight(gui_height);
	font_status.load("Roboto-Regular.ttf", STATUS_FONT_UNIT * gui_scale);
	font_gui.load("Roboto-Regular.ttf", FONT_UNIT * gui_scale);
	ofxBaseGui::loadFont("Roboto-Regular.ttf", FONT_UNIT * gui_scale);
	guiRebuild();

	help_height = font_gui.getStringBoundingBox(help_text, 0, 0).getHeight();
}

//--------------------------------------------------------------
void ofApp::guiRebuild() {
	gui_block.clear();
	gui_draw.clear();

	block_settings.clear();
	block_current_info.clear();
	block_current_settings.clear();
	block_generation.clear();

	cam_degree.clear();
	light_degree.clear();
	draw_settings.clear();
	draw_functions.clear();

	blockSettingUpdate();
	if (block_data)
		blockCurrentInfoUpdate();

	guiBlockSetup();
	guiDrawSetup();
	blockSettingUpdate();
	blockCurrentInfoUpdate();
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
void ofApp::statusUpdate() {
	if (block_data && block_data->status.isUpdated()) {
		statusMessage s = block_data->status.getStatus();
		if (s.level == statusLevel::Error)
			block_status = "[ ERROR ] " + s.msg;
		if (s.level == statusLevel::Warning)
			block_status = "[ WARNING ] " + s.msg;
		if (s.level == statusLevel::Info)
			block_status = "[ INFO ] " + s.msg;
	}
	if (draw_object && draw_object->status.isUpdated()) {
		statusMessage s = draw_object->status.getStatus();
		if (s.level == statusLevel::Error)
			draw_status = "[ ERROR ] " + s.msg;
		if (s.level == statusLevel::Warning)
			draw_status = "[ WARNING ] " + s.msg;
		if (s.level == statusLevel::Info)
			draw_status = "[ INFO ] " + s.msg;
	}
}

//--------------------------------------------------------------
void ofApp::drawStatus() {
	float max_width = gui_width - 30;

	std::string s = "";
	std::string tmp = "";
	for (char c : block_status.get()) {
		s += c;
		tmp += c;
		if (font_status.stringWidth(tmp) > max_width) {
			s += '\n';
			tmp = "";
		}
	}
	font_status.drawString(s, rect_status_block.x + margin, rect_status_block.y + margin + font_status.getSize());

	s = "";
	tmp = "";
	for (char c : draw_status.get()) {
		s += c;
		tmp += c;
		if (font_status.stringWidth(tmp) > max_width) {
			s += '\n';
			tmp = "";
		}
	}
	font_status.drawString(s, rect_status_draw.x + margin, rect_status_draw.y + margin + font_status.getSize());

	generate_status_changed = true;
}

//--------------------------------------------------------------
void ofApp::drawEtc() {
	font_gui.drawString(help_text, rect_block_gui.x + margin, rect_block_gui.y + rect_block_gui.height - help_height - margin);
}

//--------------------------------------------------------------
void ofApp::maxSizeChanged(int & v) {
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

	generate_request = true;
	generate_status_changed = false;
	block_data->status.setStatus(statusLevel::Info, "[ blockData ] Generating block...");
}

//--------------------------------------------------------------
void ofApp::drawResetClicked() {
	initializeDrawValue();
}

//--------------------------------------------------------------
void ofApp::saveImageClicked() {
	if (!draw_object)
		return;
	std::string folder = ofFilePath::getAbsolutePath("saved_images/", false);
	std::string name = draw_object->getIdentify() + ".jpg";
	ofDirectory::createDirectory(folder, true, true);
	draw_object->saveImage(name, folder);
}

//--------------------------------------------------------------
void ofApp::saveGuiSettings() {
	//block, draw settings?
	ofJson json;

	json["window"]["width"] = ofGetWidth();
	json["window"]["height"] = ofGetHeight();

	json["gui"]["scale"] = gui_scale;

	ofSavePrettyJson("gui_settings.json", json);
}

//--------------------------------------------------------------
void ofApp::loadGuiSettings() {
	std::string path = "gui_settings.json";

	if (!ofFile::doesFileExist(path))
		return;

	ofJson json = ofLoadJson(path);

	if (json.contains("window")) {
		int w = json["window"].value("width", ofGetWidth());
		int h = json["window"].value("height", ofGetHeight());
		ofSetWindowShape(w, h);
	}
	if (json.contains("gui")) {
		int scale = json["gui"].value("scale", gui_scale);
		gui_scale = scale;
	}
}
