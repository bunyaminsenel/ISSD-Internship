#include "opencv2/opencv.hpp"
#include <vector>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;


int main(int, char**)
{
    VideoCapture cap("/home/bunyamin/Downloads/rekor denemesi.mp4");
    double fps =cap.get(CAP_PROP_FPS);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920 );
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
    cap.set(CV_CAP_PROP_FPS, 3);
    int tresh=10*120; 
    float sonuc;
    Size size(1920,1080);

    for(int i=0; i<tresh;i++)
    {
        Mat frame;
        cap >> frame;
        /*
        video kayıt etme.
        int satir=cap.get(CV_CAP_PROP_FRAME_HEIGHT);
        int sutun=cap.get(CV_CAP_PROP_FRAME_WIDTH);
        Size boyut(satir,sutun);
        resize(frame,frame,size);
        VideoWriter yaz("/home/bunyamin/opencv/veri/"+ss.str()+".jpg",CV_FOURCC_DEFAULT,20,boyut,true);
        */
        /*
        vector<int> sparam;
        sparam.push_back(CV_IMWRITE_JPEG_QUALITY);
        sparam.push_back(9); //0-100 arasında değer alır
        imwrite("/home/bunyamin/opencv/veri1/"+ss.str()+".jpg",frame,sparam);

        */ 
        string out_string;
        stringstream ss;
        ss << i;
        imwrite("/home/bunyamin/opencv/veri1/"+ss.str()+".jpg",frame);
        if(waitKey(30) >= 0) break;   
    }
    return 0;
}