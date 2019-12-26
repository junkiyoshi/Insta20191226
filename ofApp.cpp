#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);

	for (int i = 0; i < 5; i++) {

		this->noise_seed_list.push_back(glm::vec2(ofRandom(1000), ofRandom(1000)));
	}

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-Bold.ttf", 500, true, true, true);

	string word = "A";
	auto word_size = glm::vec2(font.stringWidth(word), font.stringHeight(word));

	vector<ofPath> word_path = font.getStringAsPoints(word, true, false);
	for (int word_index = 0; word_index < word.size(); word_index++) {

		vector<ofPolyline> outline = word_path[word_index].getOutline();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledBySpacing(10);
		    auto vertices = outline[outline_index].getVertices();
			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				glm::vec2 vertex = glm::vec2(vertices[vertices_index]);
				vertex += glm::vec2(ofGetWidth() * 0.5 - word_size.x * 0.5, ofGetHeight() * 0.5 + word_size.y * 0.5);

				this->location_list.push_back(vertex);
				this->velocity_list.push_back(glm::vec2());
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (auto& noise_seed : this->noise_seed_list) {

		auto location = glm::vec2(ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.003), 0, 1, 0, ofGetWidth()), ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.003), 0, 1, 0, ofGetHeight()));
		auto next = glm::vec2(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + 1) * 0.003), 0, 1, 0, ofGetWidth()), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + 1) * 0.003), 0, 1, 0, ofGetHeight()));
		auto distance = location - next;
		distance *= 2;

		auto future = location + distance * 30;
		auto random_deg = ofRandom(360);
		future += glm::vec2(30 * cos(random_deg * DEG_TO_RAD), 30 * sin(random_deg * DEG_TO_RAD));
		auto future_distance = future - location;

		this->location_list.push_back(location);
		this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance) * 0.5);
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->location_list[i] += this->velocity_list[i];
		this->velocity_list[i] *= 1.01;

		if (glm::distance(glm::vec2(ofGetWidth() * 0.5, ofGetHeight() * 0.5), this->location_list[i]) > 720) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->location_list.size(); i++) {

		for (int k = 0; k < this->location_list.size(); k++) {

			if (i == k) continue;
			if (this->velocity_list[i] == glm::vec2() && this->velocity_list[k] == glm::vec2()) { continue; }

			if (glm::distance(this->location_list[i], this->location_list[k]) < 50) {

				ofDrawLine(this->location_list[i], this->location_list[k]);
			}
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}