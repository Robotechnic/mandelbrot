#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    //initialisation des variables
    width = ofGetWidth();
    height = ofGetHeight();
    zoom = double(0.1);
    doUpdate = true;
    centerX = 0;
    centerY = 0;
    escapeRadius = 4;
    iterMax = 100;
    //creation du gui principal
    gui.setup();
    gui.add(reset.setup("Reinitialiser"));
    reset.addListener(this,&ofApp::setup);
    gui.add(screen.setup("Enregister Image"));
    screen.addListener(this,&ofApp::screen_shoot);

    //groupe de capture d'ecran
    gui.add(groupe.setup("Cature video"));
    groupe.add(nom.setup("Nom","Nom_de_la_capture"));
    //nom.addListener(this,&ofApp::caractere);
    groupe.add(captureStart.setup("Demarer la capture"));
    captureStart.addListener(this,&ofApp::startcapture);
    groupe.add(captureStop.setup("Areter la capture"));
    captureStop.addListener(this,&ofApp::stopcapture);
    gui.setPosition(0,0);
    widthg = gui.getWidth();
    img.allocate(width, height, OF_IMAGE_COLOR);
    capture = false;

}

//--------------------------------------------------------------
void ofApp::update()
{
    color.setHsb(0,255,0);
    if(doUpdate)
    {
        dessin();
        doUpdate = false;
    }
    if (ofGetMousePressed()&&ofGetMouseX()>widthg)
    {
        y = ofGetMouseY();
        x = ofGetMouseX()-widthg;
        if(capture)
        {
            click.push_back(glm::tvec3<double>(ofGetMouseX(),ofGetMouseY(),zoom));
            click.push_back(glm::tvec3<double>(ofGetMouseX(),ofGetMouseY(),zoom*1.05));
            cout<<"ajout "<<click.size()<<endl;
        }
        zoomer(x,y,1.1);
    }
    if (creerVideo&&i<(int)click.size())
    {
            i++;
            if (i >= (int)click.size())
            {
                string commande = "ffmpeg -framerate 20 -start_number 001  -i \"captures/"+nom.getParameter().toString()+"/img-%0"+std::to_string(zeros)+"d.png\" -c:v libx264 -strict -2 -preset slow -pix_fmt yuv420p -vf \"scale=trunc(iw/2)*2:trunc(ih/2)*2\" -f mp4 -y \"captures/"+nom.getParameter().toString()+"/"+nom.getParameter().toString()+".mp4\"";
                cout<<commande<<endl;
                int result = system(commande.c_str());
                cout<<"Résultat: "<<result<<endl;
                creerVideo = false;
                return;
            }
            cout <<click[i].x<<" "<<click[i].y<<endl;
            zoom = click[i].z;
            zoomer(click[i].x,click[i].y,1);
            std::stringstream stream;
            stream<<std::setfill('0')<< std::setw(zeros)<<i;
            cout<<img.save(cheminfichier+"img-"+stream.str()+".png")<<endl;

    }
}
void ofApp::zoomer(double x_z,double y_z,double facteur)
{
    cout<<ofGetMousePressed()<<endl;
    double clickX = x_z - widthImg/2;
    double clickY = y_z - heightImg/2;
    clickX /= double (zoom*1000);
    clickY /= double (zoom*1000);
    zoom *= facteur;
    centerX += clickX;
    centerY += clickY;
    centerX -= clickX/facteur;
    centerY -= clickY/facteur;
    doUpdate = true;
}
//--------------------------------------------------------------
void ofApp::draw(){
    img.draw(gui.getWidth(),0);
    gui.draw();
}
void ofApp::dessin()
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
void ofApp::zero()
{
    zoom = 0.1;
    centerX = 0;
    centerY = 0;
}
void ofApp::screen_shoot()
{
    string titr;
    titr+="../images/screen "+std::to_string(zoom)+" "+std::to_string(x)+" "+std::to_string(y)+".png";
    img.update();
    cout<<img.save(titr)<<endl;
}
void ofApp::startcapture()
{
    cout<<"capture on"<<endl;
    capture = true;
    vector<glm::tvec3<double>>t;
    click = t;
}
void ofApp::stopcapture()
{
    capture = false;
    cout<<"capture off"<<endl;
    cheminfichier = "../captures/"+nom.getParameter().toString()+"/";
    creerVideo = true;
    i=0;
    nbZero();
}
void ofApp::nbZero()
{
    int l = click.size();
    zeros = std::to_string(l).size();
}
void ofApp::caractere()
{
    string traitement = nom.getParameter().toString();
    for (int m = 0; m<(int)traitement.size();m++)
    {
        if (traitement[m] == ' ')
            traitement[m] = '_';
        nom.getParameter().fromString(traitement);
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == '+')
    {
        iterMax+=5;
    }
    else if(key == '-')
    {
        iterMax-=5;
    }

}

//--------------------------------------------------------------
//void ofApp::keyReleased(int key){
//        doUpdate = true;
//}
//--------------------------------------------------------------
/*void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}*/

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    doUpdate = true;
    width = w-widthg;
    height = h;
    img.allocate(width, height, OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
/*void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}*/
