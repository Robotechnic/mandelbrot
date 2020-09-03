#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H


#include <iostream>
using namespace std;
#include <ofThread.h>
#include <ofColor.h>
#include <ofImage.h>

class renderThread : public ofThread
{
public:
    renderThread();
    void threadedFunction();
    void startRender(int zoom, int centerX, int centerY, double escapeRadius, int xMin, int xMax, int yMin, int yMax);
    ofImage getRender();

private:
    int iterMax, width, height;
    double c_x;
    double c_y;
    double z_x;
    double z_y;
    int iter;
    double newZX;
    double widthg;
    double widthImg,heightImg;
    double escapeRadius;
    int zoom,  centerX, centerY, yMin, yMax, xMin, xMax;
    ofColor renderPoint(int x, int y);
    ofImage render;
};

#endif // RENDERTHREAD_H
