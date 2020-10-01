#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
    //initialisation des variables
    width = ofGetWidth();
    height = ofGetHeight();
    zoom = double(0.1);
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
    capture = false;
    waitForRender = false;
    imageDraw = false;

    dessin();
}

//--------------------------------------------------------------
void ofApp::update()
{
    color.setHsb(0,255,0);
    if (creerVideo&&i<(int)click.size())
    {
        if (!waitForRender){
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
            this->waitForRender = true;
            this->imageDraw = false;
        } else if(imageDraw){
            cout<<"Render "<<thread1.getDoRender()<<" "<<thread2.getDoRender()<<" "<<thread3.getDoRender()<<" "<<thread4.getDoRender()<<endl;
            std::stringstream stream;
            stream<<std::setfill('0')<< std::setw(zeros)<<i;
            cout<<img.save(cheminfichier+"img-"+stream.str()+".png")<<endl;
            this->waitForRender = false;
        }

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
    dessin();
}
//--------------------------------------------------------------
void ofApp::draw(){
    widthImg = ofGetWidth()-widthg;
    heightImg = ofGetHeight();
    img1.draw(gui.getWidth(),0);
    img2.draw(gui.getWidth()+widthImg/2,0);
    img3.draw(gui.getWidth(),heightImg/2);
    img4.draw(gui.getWidth()+widthImg/2,heightImg/2);
    if ((!thread1.getDoRender())&&
        (!thread2.getDoRender())&&
        (!thread3.getDoRender())&&
        (!thread4.getDoRender()) && this->waitForRender){
        this->imageDraw = true;
        img.grabScreen(widthg,0,ofGetWidth(),ofGetHeight());
    }
    gui.draw();
}
void ofApp::dessin()
{
    cout<<"Dessin"<<endl;
    widthImg = ofGetWidth()-widthg;
    heightImg = ofGetHeight();
    thread1.startRender(img1,iterMax,zoom,centerX,centerY,escapeRadius,
                        0,
                        0,
                        widthImg/2,
                        heightImg/2,
                        widthImg,heightImg);
    thread2.startRender(img2,iterMax,zoom,centerX,centerY,escapeRadius,
                        widthImg/2,
                        0,
                        widthImg,
                        heightImg/2,
                        widthImg,heightImg);
    thread3.startRender(img3,iterMax,zoom,centerX,centerY,escapeRadius,
                        0,
                        heightImg/2,
                        widthImg/2,
                        heightImg,
                        widthImg,heightImg);
    thread4.startRender(img4,iterMax,zoom,centerX,centerY,escapeRadius,
                        widthImg/2,
                        heightImg/2,
                        widthImg,
                        heightImg,
                        widthImg,heightImg);
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
    ofImage img;
    img.grabScreen(widthg,0,ofGetWidth(),ofGetHeight());
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
*/
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (x>widthg)
    {
        x -= widthg;
        if(capture)
        {
            click.push_back(glm::tvec3<double>(ofGetMouseX(),ofGetMouseY(),zoom));
            click.push_back(glm::tvec3<double>(ofGetMouseX(),ofGetMouseY(),zoom*1.05));
            cout<<"ajout "<<click.size()<<endl;
        }
        zoomer(x,y,1.1);
    }
}
/*
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
    this->widthg = this->gui.getWidth();
    this->width = w-widthg;
    this->height = h;
    this->dessin();
    //img.allocate(width, height, OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
/*void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}*/
