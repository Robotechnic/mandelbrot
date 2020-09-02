#include "renderthread.h"

renderThread::renderThread()
{

}

void renderThread::startRender(int zoom, int centerX, int centerY, double escapeRadius, int xMin, int xMax, int yMin, int yMax){
    this->zoom = zoom;
    this->centerX = centerX;
    this->centerY = centerY;
    this->xMin = xMin;
    this->xMax = xMax;
    this->yMin = yMin;
    this->yMax = yMax;
    this->escapeRadius = escapeRadius;
    this->render.allocate(xMax-xMin, yMax-yMin, OF_IMAGE_COLOR);

    this->startThread();
}

void renderThread::threadedFunction(){
    //the render of the image
    for(int x=xMin; x<xMax; x++)
    {
        for(int y=yMin; y<yMax; y++)
        {
            this->render.setColor(x,y,this->renderPoint(x,y));
        }
    }
    this->stopThread();
}

ofColor renderThread::renderPoint(int x, int y){
    //render a spécific point of the image
    c_x = (x - (widthImg)/2)/zoom/1000+centerX;
    c_y = (y-heightImg/2)/zoom/1000+centerY;
    z_x = c_x;
    z_y = c_y;
    iter = 0;
    ofColor color;
    do
    {

    newZX = z_x;
    //mandelbrot *3
    //z_x = z_x*z_x*z_x*z_x - 6 * z_x * z_x * z_y * z_y + + z_y*z_y*z_y*z_y + c_x;
    //z_y = 4 * newZX * newZX * newZX * z_y - 4 * z_y * z_y * z_y * newZX + c_y;

    //mandelbrat normal
    /*z_x = z_x*z_x - z_y * z_y + c_x;
    z_y = newZX*z_y*2 + c_y;*/

    z_x = z_x*z_x - z_y * z_y  + c_x;
    z_y = newZX*z_y*2 + c_y;

    //fractale trigo hyperbolique
    //z_x =sinh(z_x)*cos(z_y)+(c_x*c_x-c_y*c_y)/(pow(c_x,10)+pow(c_y,10));
    //z_y = cos(z_x)*sinh(z_y)-(2*c_x*c_y)/(pow(c_x,10)+pow(c_y,10));
    if (iter >= iterMax) {
        //ofLogError() << "Debug1 " << iter;
        color = ofColor::fromHsb(0,0,0);
        break;
    }
    if (z_x*z_x + z_y*z_y > escapeRadius) {
        //ofLogError() << "Debug2 " << iter;
     color = ofColor::fromHsb((iter*1)%255, 255, 255);
      break;
    }
    iter += 1;

    } while (true);

    return color;
}

ofImage renderThread::getRender(){
    return this->render;
}
