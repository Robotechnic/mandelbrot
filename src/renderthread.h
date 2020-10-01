#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H


#include <iostream>
using namespace std;
#include <ofThread.h>
#include <ofColor.h>
#include <ofImage.h>
#include <ofThreadChannel.h>
#include <ofEvents.h>

class renderThread : public ofThread
{
public:
    renderThread();
    ~renderThread();
    void startRender(ofImage& render, double iterMax, double zoom, double centerX, double centerY, double escapeRadius, double xMin, double yMin, double xMax, double yMax, double widthImg, double heightImg);
    bool getDoRender(){
        return !(this->renderChannel.empty() && this->treatedChannel.empty());
    }

private:
    virtual void threadedFunction();
    void update(ofEventArgs & a);
    ofColor renderPoint(double x, double y, double zoom, double centerX, double centerY, double escapeRadius, double iterMax, double widthImg, double heightImg);
    int width, height;
    double c_x;
    double c_y;
    double z_x;
    double z_y;
    int iter;
    double newZX;
    bool doRender;

    struct ofImageEntry {
        ofImageEntry() {
            image = NULL;
        }

        ofImageEntry(ofImage & pImage) {
            image = &pImage;
        }
        ofImage* image;
        double zoom,  centerX, centerY, yMin, yMax, xMin, xMax,iterMax;
        double widthImg,heightImg;
        double escapeRadius;
    };

    ofThreadChannel<ofImageEntry> renderChannel;
    ofThreadChannel<ofImageEntry> treatedChannel;
};

#endif // RENDERTHREAD_H
