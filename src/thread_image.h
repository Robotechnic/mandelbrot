#ifndef THREAD_IMAGE_H
#define THREAD_IMAGE_H
#include "ofMain.h"

class thread_image : public ofThread
{
public:
    thread_image(vector <glm::tvec3<double>>, int height, int width, int debut,int fin, int zero, string cheminf);
    void dessin();
    void zoomer(double x_y, double y_z, double facteur);
    void update();
    void threadedFunction()
    {
        while (this->isThreadRunning())
        {
            update();
        }
    }
    bool getfinis();
private:
    ofImage img;
    vector <glm::tvec3<double>>click;
    double zoom;
    double widthImg, heightImg;
    double centerX, centerY;
    double c_x,z_x,c_y,z_y,newZX;
    int iter, iterMax;
    ofColor color;
    double escapeRadius;
    int debut, fin;
    bool finis;
    int zeros;
    string cheminfichier;
};

#endif // THREAD_IMAGE_H
