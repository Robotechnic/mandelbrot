#include "thread_image.h"

thread_image::thread_image(vector <glm::tvec3<double>> vec, int height, int width, int debut,int fin,int zero, string cheminf)
{
    finis = false;
    img.allocate(height,width,OF_IMAGE_COLOR);
    click = vec;
    this->debut = debut;
    this->fin = fin;
    zeros = zero;
    cheminfichier = cheminf;
    centerX = 0;
    centerY = 0;
}
void thread_image::zoomer(double x_z,double y_z,double facteur)
{
    double clickX = x_z - widthImg/2;
    double clickY = y_z - heightImg/2;
    clickX /= double (zoom*1000);
    clickY /= double (zoom*1000);
    zoom *= facteur;
    centerX += clickX;
    centerY += clickY;
    centerX -= clickX/facteur;
    centerY -= clickY/facteur;
}
void thread_image::dessin()
{
    widthImg = img.getWidth();
    heightImg = img.getHeight();
    for(int x=0; x<widthImg; x++)
    {

        for(int y=0; y<heightImg; y++)
        {
            c_x = (x - (widthImg)/2)/zoom/1000+centerX;
            c_y = (y-heightImg/2)/zoom/1000+centerY;
            z_x = c_x;
            z_y = c_y;
            iter = 0;
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
            //ofLogError() << "Iteration (" << x << ","<<y<<") : " <<iter;

            img.setColor(x,y,color);//dessin de la fractale dans l'image
        }
    }
    /*ofLogWarning() << "Iteration took " << (ofGetElapsedTimef() - t0);
    ofLogWarning() << "Zoom: " << zoom;*/
    img.update();
}
void thread_image::update()
{
    debut++;
    if (debut >= fin)
    {
        finis = true;
        this->stopThread();
    }
    zoom = click[debut].z;
    zoomer(click[debut].x,click[debut].y,1);
    img.update();
    img.draw(0,0);
    std::stringstream stream;
    stream<<std::setfill('0')<< std::setw(zeros)<<debut;
    cout<<img.save(cheminfichier+"img-"+stream.str()+".png")<<endl;
}
bool thread_image::getfinis()
{
    return finis;
}
