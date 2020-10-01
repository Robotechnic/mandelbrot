#pragma once

#include "ofMain.h"
#include "ofxPanel.h"
#include "ofxButton.h"
#include <boost/multiprecision/float128.hpp>

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
    void draw();
    void dessin();
    void zoomer(float128_type x_y, float128_type y_z, float128_type facteur);
    void zero();
    void screen_shoot();
    void keyPressed  (int key);
    void windowResized(int w, int h);
    //void keyReleased(int key);
    void startcapture();
    void stopcapture();
    void nbZero();
    void caractere();
    /*void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
	void gotMessage(ofMessage msg);
    void dragEvent(ofDragInfo dragInfo);*/
private:
    ofImage img;
    ofColor color;
    bool doUpdate;
    float128_type zoom;
    float128_type centerX;
    float128_type centerY;
    float128_type escapeRadius;
    int iterMax, width, height;
    float128_type c_x;
    float128_type c_y;
    float128_type z_x;
    float128_type z_y;
    int iter;
    float128_type newZX;
    float128_type widthg;
    float128_type widthImg,heightImg;
    int x;
    int y;
    bool capture;
    float128_type pourcentage_capture;
    ofKeyEventArgs keyEvent;
    vector <glm::tvec3<float128_type>>click;
    ofxPanel gui;
    ofxButton reset;
    ofxButton screen;
    //groupe de capture video
    ofxGuiGroup groupe;
    ofxButton captureStart;
    ofxButton captureStop;
    ofxInputField <string>nom;
    ofxInputField <int>iteration;
    //variables de creation video
    string cheminfichier;
    int i,zeros;
    bool creerVideo;
};
