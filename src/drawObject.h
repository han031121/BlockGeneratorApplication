#pragma once

#include "blockData.h"
#include "statusManager.h"
#include "ofMain.h"

class drawObject {

private:
	int block_size = 250;
	int width;
	int height;

	float degree_xz;
	float degree_h;
	float light_degree_xz;
	float light_degree_h;
	float thickness;

	int cam_fov = 15;
	float cam_min_dist = 7000;
	float default_cam_dist = 7000;
	float cam_dist = 0;
	glm::vec3 cam_center;

	bool need_to_refresh = true;

	blockData * data = nullptr;

	ofColor draw_color = ofColor(220, 185, 154);
	ofFbo fbo;
	ofEasyCam cam;
	ofLight light;

	void setup();

	void drawBlocks(); //draw blocks on fbo
	void drawOutline();
	void drawSingleOutline(int r, int c, int h);

	void setFbo();
	void setCamera();

public:
	statusManager status;

	drawObject(blockData* _d, int _w, int _h, int _dxz = 25, int _dh = 20, int _ldxz = 25, int _ldh = 38, int _t = 5)
		: data(_d), width(_w), height(_h),
		degree_xz(_dxz), degree_h(_dh), light_degree_xz(_ldxz), light_degree_h(_ldh), thickness(_t) {
			if (!data) {
				status.setStatus(statusLevel::Error, "[drawObject] invalid blockData.");
				return;
			}	

			setup();
		}

	void render(); //draw and render blocks on fbo
	void saveImage(std::string filename); //save PNG

	void getPixels(ofPixels & pixels);
	void getImage(ofImage & image);
	std::string getIdentify();

	void camDegreeUpdate(float deg_xz, float deg_h, bool isRelative = false);
	void lightDegreeUpdate(float deg_xz, float deg_h, bool isRelative = false);
	void blockColorUpdate(int r, int g, int b, bool isRelative = false);
	void camDistUpdate(int d, bool isRelative = false);
	void thicknessUpdate(int t, bool isRelative = false);

	float getCamDegreeXZ() const { return degree_xz; }
	float getCamDegreeH() const { return degree_h; }
	float getLightDegreeXZ() const { return light_degree_xz; }
	float getLightDegreeH() const { return light_degree_h; }
	glm::vec3 getblockColor() const { return { draw_color.r, draw_color.g, draw_color.b }; }
	float getCamDist() const { return cam_dist; }
	float getDefaultCamDist() const { return default_cam_dist; }
	float getThickness() const { return thickness; }

	void drawFbo(float x, float y, int w, int h) {
		fbo.draw(x, y, w, h);
	}
};
