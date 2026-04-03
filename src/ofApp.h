#pragma once

#include "ofMain.h"
#include "blockData.h"
#include "drawObject.h"
#include "ofxGui.h"

#define MARGIN_UNIT 3
#define WIDTH_UNIT 100
#define HEIGHT_UNIT 10
#define STATUS_HEIGHT_UNIT 30
#define FONT_UNIT 5
#define STATUS_FONT_UNIT 4

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();

		void updateLayout();
		void initializeBlockValue();
		void initializeDrawValue();
		void guiBlockSetup();
		void guiDrawSetup();
		void guiSetListener();
		void guiSetScale();
		void guiRebuild();

		void drawObjectUpdate();
		void blockSettingUpdate();
		void blockCurrentInfoUpdate();
		void statusUpdate();
		void drawStatus();

		void maxSizeChanged(int & v);
		void maxBlockCountChanged(int & v);
		void minBlockCountChanged(int & v);
		void setBlockClicked();
		void generateBlockClicked();
		void drawResetClicked();
		void saveImageClicked();

		void saveGuiSettings();
		void loadGuiSettings();

		ofTrueTypeFont font_status;

		std::unique_ptr<blockData> block_data;
		std::unique_ptr<drawObject> draw_object;
		float gui_width = 400.0f;
		float gui_height = 40.0f;
		float margin = 10.0f;
		float status_height = 120.0f;
		int image_size = 1000;
		int gui_scale = 4;

		ofRectangle rect_draw_gui;
		ofRectangle rect_block_gui;
		ofRectangle rect_image;
		ofRectangle rect_status_block;
		ofRectangle rect_status_draw;

		ofParameter<int> block_count_1 = 4, block_count_2 = 8, max_r = 3, max_c = 3, max_h = 3;
		ofParameter<float> density = 20;
		ofParameter<bool> allow_duplication = false;
		ofxLabel block_count_setting, max_size_setting, density_setting, allow_duplication_setting;
		ofxLabel block_count_current, size_current;
		ofParameter<void> set_block, generate_block;
		ofParameter<float> cam_degree_xz, cam_degree_y;
		ofParameter<float> light_degree_xz, light_degree_y;
		ofParameter<float> magnification;
		ofParameter<int> thickness;
		ofParameter<bool> fix_light;
		ofParameter<ofColor> draw_color;
		ofParameter<void> save_image, reset;
		ofParameter<std::string> block_status, draw_status;

		ofParameterGroup block_settings;
		ofxGuiGroup block_current_settings;
		ofxGuiGroup block_current_info;
		ofParameterGroup block_generation;
		ofParameterGroup draw_settings;
		ofParameterGroup cam_degree;
		ofParameterGroup light_degree;
		ofParameterGroup draw_functions;

		ofxPanel gui_block;
		ofxPanel gui_draw;
};
