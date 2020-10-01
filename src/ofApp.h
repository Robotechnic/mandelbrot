#pragma once

#include "ofMain.h"
#include "ofxPanel.h"
#include "ofxButton.h"
#include "renderthread.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
    void draw();
    void dessin();
    void zoomer(double x_y, double y_z, double facteur);
    void zero();
    void screen_shoot();
	void keyPressed  (int key);
    void windowResized(int w, int h);
    void keyReleased(int key);
    void startcapture();
    void stopcapture();
    void nbZero();
    void caractere();
    /*void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);*/
	void mousePressed(int x, int y, int button);
    /*void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
	void gotMessage(ofMessage msg);
    void dragEvent(ofDragInfo dragInfo);*/
private:
    ofColor color;
    bool doUpdate;
    float zoom;
    double centerX;
    double centerY;
    double escapeRadius;
    int iterMax, width, height;
    double c_x;
    double c_y;
    double z_x;
    double z_y;
    int iter;
    double newZX;
    double widthg;
    double widthImg,heightImg;
    int x;
    int y;
    bool capture;
    double pourcentage_capture;
    ofKeyEventArgs keyEvent;
    vector <glm::tvec3<double>>click;
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
    bool creerVideo,waitForRender;

    ofImage img1,img2,img3,img4;
    renderThread thread1, thread2,thread3,thread4;
};
