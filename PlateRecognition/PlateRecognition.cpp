//
// Created by bunyamin on 23.08.2019.
//
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <vector>
#include <string>


using namespace cv;
using namespace std;
using namespace tesseract;
Size size(620,480);


int main(int argc, char* argv[])
{
    Mat img,gray,bilateral,median,cannyout,thresh,crop;
    int largest_area=0;
    int largest_contour_index=0;
    Rect bounding_rect;

    img = imread("/home/bunyamin/opencv/ocr/image11.png");
    resize(img,img,size);
    cvtColor(img,gray,CV_BGR2GRAY);

    bilateralFilter ( gray, bilateral, 9, 23, 23 );
    medianBlur ( bilateral,median, (3,3));

    double CannyAccThresh = threshold(median,thresh,0,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C|CV_THRESH_OTSU);
    double CannyThresh = 3 * CannyAccThresh;
    Canny(median,cannyout,CannyThresh,CannyAccThresh);

    //find contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( cannyout, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    for( int i = 0; i < contours.size(); i++ )
    {
        double a = contourArea(contours[i], false); 
        if (a > largest_area)
        {
            largest_area = a;
            largest_contour_index = i;           
            bounding_rect = boundingRect(contours[i]); 
        }
            //drawContours( img, contours, largest_contour_index, Scalar(255,0,0),3);
    }
    rectangle(img, bounding_rect, Scalar(0, 255, 0), 1, 8, 0);
    crop=gray(bounding_rect);

    imshow("finale",img);
    imshow("plate",crop);

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    imwrite("output.png",crop,compression_params);
    waitKey(0);
    return 0;
}
