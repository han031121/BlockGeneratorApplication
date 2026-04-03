#include "drawObject.h"

void drawObject::setup() {
	setFbo();

	light.setup();
	light.setDirectional();
	light.setDiffuseColor(ofFloatColor(1.0));
	light.setSpecularColor(ofFloatColor(0.3));

	cam.setFov(cam_fov);

	float size_r = data->getSizeRow() * block_size;
	float size_c = data->getSizeCol() * block_size;
	float size_h = data->getSizeHeight() * block_size;
	float block_radius = 0.5f * sqrt(size_r * size_r + size_c * size_c + size_h * size_h);
	float margin = 1.1;

	cam_dist = margin * block_radius / std::tan(std::numbers::pi * cam.getFov() / 180 / 2);
	cam_dist = std::max(cam_min_dist, cam_dist);
	default_cam_dist = cam_dist;

	std::tuple<float, float, float> block_center = data->getCenter();
	cam_center = {
		get<0>(block_center) * block_size,
		get<2>(block_center) * block_size,
		-get<1>(block_center) * block_size
	};
}

void drawObject::drawBlocks() {
	int max_r = data->getMaxRow();
	int max_c = data->getMaxCol();

	for (int r = 0; r < max_r; r++) {
		for (int c = 0; c < max_c; c++) {
			int height = data->getHeightData(r, c);

			for (int h = 0; h < height; h++)
				ofDrawBox(r * block_size, h * block_size, -c * block_size, block_size);
		}
	}
}

void drawObject::drawOutline() {
	int max_r = data->getMaxRow();
	int max_c = data->getMaxCol();

	ofSetColor(0, 0, 0, 255);

	for (int r = 0; r < max_r; r++) {
		for (int c = 0; c < max_c; c++) {
			int height = data->getHeightData(r, c);

			for (int h = 0; h < height; h++)
				drawSingleOutline(r, c, h);
		}
	}
}

void drawObject::drawSingleOutline(int r, int c, int h) {
	int mul_a[4] = { 1, 1, -1, -1 };
	int mul_b[4] = { 1, -1, 1, -1 };
	glm::vec3 block_center = { r * block_size, h * block_size, -c * block_size };

	for (int i = 0; i < 4; i++) {
		glm::vec3 mul = { mul_a[i], mul_b[i], 0 }; //z fixed
		glm::vec3 edge_center = block_center + (float)block_size / 2 * mul;
		ofDrawBox(edge_center, thickness, thickness, block_size);
	}
	for (int i = 0; i < 4; i++) {
		glm::vec3 mul = { 0, mul_b[i], mul_a[i] }; //x fixed
		glm::vec3 edge_center = block_center + (float)block_size / 2 * mul;
		ofDrawBox(edge_center, block_size, thickness, thickness);
	}
	for (int i = 0; i < 4; i++) {
		glm::vec3 mul = { mul_a[i], 0, mul_b[i] }; //y fixed
		glm::vec3 edge_center = block_center + (float)block_size / 2 * mul;
		ofDrawBox(edge_center, thickness, block_size, thickness);
	}
}

void drawObject::setFbo() {
	ofFbo::Settings s;
	s.width = width;
	s.height = height;
	s.internalformat = GL_RGBA;
	s.useDepth = true;
	s.useStencil = true;
	s.depthStencilAsTexture = false;
	s.numSamples = 4;
	fbo.allocate(s);
}

void drawObject::setCamera() {
	cam.orbitDeg(90 + degree_xz, -degree_h, cam_dist, cam_center);
	cam.lookAt(cam_center, {0, 1, 0});

	light.setOrientation(glm::vec3(light_degree_h, 270 + light_degree_xz, 0));
}

void drawObject::render() {
	if (!data || !need_to_refresh)
		return;

	fbo.begin();
	ofClear(255, 255, 255, 255);
	setCamera();
	ofEnableDepthTest();

	ofEnableLighting();
	light.enable();

	cam.begin();
	ofSetColor(draw_color, 255);
	drawBlocks();
	cam.end();

	light.disable();
	ofDisableLighting();

	cam.begin();
	ofSetColor(0, 0, 0, 255);
	drawOutline();
	cam.end();

	ofDisableDepthTest();
	fbo.end();

	need_to_refresh = false;
	status.setStatus(statusLevel::Info, "[ drawObject ] Image rendered successfully");
}

void drawObject::saveImage(std::string filename) {
	if (need_to_refresh) {
		status.setStatus(statusLevel::Error, "[ drawObject ] Need to refresh the image.");
		return;
	}
	ofPixels pixels;
	fbo.readToPixels(pixels);
	ofSaveImage(pixels, filename);

	status.setStatus(statusLevel::Info, "[ drawObject ] Image " + filename + " saved.");
};

void drawObject::getPixels(ofPixels & pixels) {
	if (need_to_refresh) {
		status.setStatus(statusLevel::Error, "[ drawObject ] Need to refresh the image.");
		return;
	}
	fbo.readToPixels(pixels);
}

void drawObject::getImage(ofImage & image) {
	if (need_to_refresh) {
		status.setStatus(statusLevel::Error, "[ drawObject ] Need to refresh the image.");
		return;
	}
	ofPixels pix;
	getPixels(pix);
	image.setFromPixels(pix);
}

std::string drawObject::getIdentify() {
	char hex[] = "0123456789ABCDEF";
	std::string s = "";
	std::stringstream ss;

	s += data->getIdentify();
	s += '_' + std::to_string((int)degree_xz) + std::to_string((int)degree_h);
	s += '_' + std::to_string((int)light_degree_xz) + std::to_string((int)light_degree_h);
	ss << '_'
	   << std::uppercase << std::hex
	   << std::setw(2) << std::setfill('0') << (int)draw_color.r
	   << std::setw(2) << std::setfill('0') << (int)draw_color.g
	   << std::setw(2) << std::setfill('0') << (int)draw_color.b;
	s += ss.str();
	return s;
}

//---------------------------- Update -------------------------------
void drawObject::camDegreeUpdate(float deg_xz, float deg_h, bool isRelative) {
	if (isRelative) {
		if (deg_xz == 0 && deg_h == 0)
			return;
		degree_xz += deg_xz;
		degree_h += deg_h;
	}
	else {
		if (deg_xz == degree_xz && deg_h == degree_h)
			return;
		degree_xz = deg_xz;
		degree_h = deg_h;
	}
	need_to_refresh = true;
}

void drawObject::lightDegreeUpdate(float deg_xz, float deg_h, bool isRelative) {
	if (isRelative) {
		if (deg_xz == 0 && deg_h == 0)
			return;
		light_degree_xz += deg_xz;
		light_degree_h += deg_h;
	}
	else {
		if (deg_xz == light_degree_xz && deg_h == light_degree_h)
			return;
		light_degree_xz = deg_xz;
		light_degree_h = deg_h;
	}
	need_to_refresh = true;
}

void drawObject::blockColorUpdate(int r, int g, int b, bool isRelative) {
	if (isRelative) {
		if (r == 0 && g == 0 && b == 0)
			return;
		glm::vec3 new_color(draw_color.r, draw_color.g, draw_color.b);
		new_color += glm::vec3(r, g, b);
		draw_color = ofColor(new_color.x, new_color.y, new_color.z);
	}
	else {
		if (r == draw_color.r && g == draw_color.g && b == draw_color.b)
			return;
		draw_color = ofColor(r, g, b);
	}
	need_to_refresh = true;
}

void drawObject::camDistUpdate(int d, bool isRelative) {
	if (isRelative) {
		if (d == 0)
			return;
		cam_dist += d;
	}
	else {
		if (d == cam_dist)
			return;
		cam_dist = d;
	}
	need_to_refresh = true;
}

void drawObject::thicknessUpdate(int t, bool isRelative) {
	if (isRelative) {
		if (t == 0)
			return;
		thickness += t;
	}
	else {
		if (t == thickness)
			return;
		thickness = t;
	}
	need_to_refresh = true;
}
