#pragma once

#include "ofMain.h"
#include "ofxBlockGenerator.h"
#include "ofxGui.h"

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

		void updateLayout();
		void initalizeUiValue();
		void guiBlockSetup();
		void guiDrawSetup();

		void maxSizeChanged(int & v);
		void maxBlockCountChanged(int & v);
		void minBlockCountChanged(int & v);

		std::unique_ptr<blockData> block_data;
		std::unique_ptr<drawObject> draw_object;

		ofRectangle rect_draw_gui;
		ofRectangle rect_block_gui;
		ofRectangle rect_image;
		float gui_width = 400.0f;
		float margin = 10.0f;

		ofParameter<int> block_count_1 = 4, block_count_2 = 8, max_r = 3, max_c = 3, max_h = 3;
		ofParameter<float> density;
		ofParameter<bool> allow_duplication = false;
		ofParameter<void> set_block, generate_block;
		ofParameter<glm::vec2> cam_degree;
		ofParameter<glm::vec2> light_degree;
		ofParameter<float> cam_dist, thickness;
		ofParameter<ofColor> draw_color;
		ofParameter<void> save_image, reset, calculate_cam_dist;

		ofParameterGroup block_settings;
		ofParameterGroup block_generation;
		ofParameterGroup draw_settings;
		ofParameterGroup draw_functions;

		ofxPanel gui_block;
		ofxPanel gui_draw;
};
