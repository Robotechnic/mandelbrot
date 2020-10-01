#include "renderthread.h"

renderThread::renderThread()
{
    ofAddListener(ofEvents().update, this, &renderThread::update);
    this->startThread();
    this->doRender = true;
}

renderThread::~renderThread(){
    this->cancelRender();
    this->doRender = false;
    this->renderChannel.close();
    this->treatedChannel.close();
    ofRemoveListener(ofEvents().update, this, &renderThread::update);
}

void renderThread::startRender(ofImage &render,double iterMax,double zoom, double centerX, double centerY, double escapeRadius, double xMin, double yMin, double xMax, double yMax,double widthImg, double heightImg){
    ofImageEntry entry(render);
    entry.zoom = zoom;
    entry.centerX = centerX;
    entry.centerY = centerY;
    entry.xMin = xMin;
    entry.xMax = xMax;
    entry.yMin = yMin;
    entry.yMax = yMax;
    entry.escapeRadius = escapeRadius;
    entry.iterMax = iterMax;
    entry.widthImg = widthImg;
    entry.heightImg = heightImg;

    entry.image->setUseTexture(false);
    entry.image->allocate(xMax-xMin, yMax-yMin, OF_IMAGE_COLOR);

    this->renderChannel.send(entry);
}

void renderThread::threadedFunction(){
    //the render of the image
    ofImageEntry current;
    while (this->doRender){
        if (this->renderChannel.receive(current)){
          //  cout<<"Start render"<<endl;
            for(int x=current.xMin; x<current.xMax; x++)
            {
                for(int y=current.yMin; y<current.yMax; y++)
                {
                    current.image->setColor(x-current.xMin,y-current.yMin,this->renderPoint(x,y,current.zoom,current.centerX,current.centerY,current.escapeRadius,current.iterMax,current.widthImg,current.heightImg));
                }
            }
           // cout<<"Stop render"<<endl;
            this->treatedChannel.send(current);
        }
    }
   // cout<<"Stop thread"<<endl;

}

ofColor renderThread::renderPoint(double x, double y,double zoom,double centerX,double centerY,double escapeRadius,double iterMax,double widthImg,double heightImg){
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

void renderThread::update(ofEventArgs &a){
    ofImageEntry render;
    if (this->treatedChannel.tryReceive(render)){
       // cout<<"New render"<<endl;
        render.image->setUseTexture(true);
        render.image->update();
    }
}
