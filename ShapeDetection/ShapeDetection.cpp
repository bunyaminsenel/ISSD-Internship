#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Size size(620, 480);

float calculateDistance(Point p1, Point p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double angle(Point p1, Point p2)
{
    int a=atan2(p1.y - p2.y, p1.x - p2.x);
    return a * 180 / 3.14;
}

int main(int argc, char* argv[])
{
    Mat input,gray,dst1,bilateral,median,thres1,thres2,cannyout,dst;
    double otsu;

    input = imread("TrainingImages.jpg", 1);
    cvtColor(input, gray, CV_BGR2GRAY);

    resize(gray, gray, size);
    resize(input, input, size);

    bilateralFilter ( gray, bilateral, 13, 75, 75 );
    medianBlur(bilateral, median, 1);

    otsu = threshold(median, thres1, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    Canny(thres1, cannyout, otsu, otsu * 1 / 2, 3, 1);

    cornerHarris(thres1, dst, 7, 5, 0.038, BORDER_DEFAULT);

    dilate(dst, dst, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
    dst.convertTo(dst1, CV_8UC1);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(cannyout, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<Moments> mu(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        mu[i] = moments(contours[i], false);
    }
    vector<Point2f> mc(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
    }

    Mat drawing = Mat::zeros(cannyout.size(), CV_8UC1);
    string sObjectNumber;
    ostringstream sContourNumber;
    vector<Point> centerPoints;

    for (int i = 0; i< contours.size(); i++)
    {
        sContourNumber << i;
        sObjectNumber = sContourNumber.str();
        Point pCoordinates(mc[i].x, mc[i].y);
        centerPoints.push_back(pCoordinates);
        Scalar color = Scalar(255, 255, 255);
        drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
        circle(drawing, mc[i], 4, color, -1, 8, 0);
        //putText(drawing, sObjectNumber, pCoordinates, CV_FONT_HERSHEY_COMPLEX, 1, color, 2, 8);
        sContourNumber.str("");
        sContourNumber.clear();
    }

    vector<vector<Point> > contours1;
    vector<Vec4i> hierarchy1;

    findContours(dst1, contours1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<Moments> mu1(contours1.size());
    for (int i = 0; i < contours1.size(); i++)
    {
        mu1[i] = moments(contours1[i], false);
    }
    vector<Point2f> mc1(contours1.size());
    for (int i = 0; i < contours1.size(); i++)
    {
        mc1[i] = Point2f(mu1[i].m10 / mu1[i].m00, mu1[i].m01 / mu1[i].m00);
    }

    Mat drawing1 = Mat::zeros(dst1.size(), CV_8UC1);
    string sObjectNumber1;
    ostringstream sContourNumber1;
    vector<Point> cornerPoints;

    for (int i = 0; i<contours1.size(); i++)
    {
        sContourNumber1 << i;
        sObjectNumber1 = sContourNumber1.str();
        Point pCoordinates1(mc1[i].x, mc1[i].y);
        cornerPoints.push_back(pCoordinates1);
        Scalar color = Scalar(255, 255, 255);
        drawContours(drawing1, contours1, i, color, 1, 8, hierarchy1, 0, Point());
        circle(drawing1, mc1[i], 4, color, -1, 8, 0);
        putText(drawing1, sObjectNumber1, pCoordinates1, CV_FONT_HERSHEY_COMPLEX, 1, color, 2, 8);
        sContourNumber1.str("");
        sContourNumber1.clear();
    }

    //cout << calculateDistance(Point(10, 10), Point(20, 20)) << endl;
    vector<int> edgeCounterVec;

    for (int i = 0; i < centerPoints.size(); i++)
    {
        float Distance= calculateDistance(centerPoints.at(0), cornerPoints.at(1));
        Distance=Distance*(1.3);
        int edgeCounter = 0;
        stringstream ss;
        //cout<<s<<endl;
        for (int j = 0; j < cornerPoints.size(); j++)
        {
            float mDistance = calculateDistance(centerPoints.at(i), cornerPoints.at(j));

            if (mDistance < Distance)
            {
                edgeCounter++;
            }
            //cout << i << " " << j << " " <<  mDistance << endl;

        }
        edgeCounterVec.push_back(edgeCounter);

    if(edgeCounter == 0 || edgeCounter == 1 )
    {
    ss<<"Daire";
    }
    else if(edgeCounter == 3)
    {
    ss << "Ucgen";
    }
    else if(edgeCounter == 4)
    {
    ss << "Dortgen";
    }
    else if(edgeCounter == 5)
    {
        ss<< "besgen";
    }
    else if (edgeCounter==6)
    {
        double anle=angle(cornerPoints.at(0),cornerPoints.at(2));
        cout<<"Angle:"<<anle<<endl;
        if(anle<75)
        {
            ss<< "yildiz";
        }
        else if(anle>75)
        {
            ss<< "altigen";
        }
    }
   else{
      //cout<<edgeCounter<<endl;
    }
        sObjectNumber = ss.str();
        putText(input, sObjectNumber, centerPoints.at(i), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255), 2, 8);
    }
    imshow("debug1", input);
    imshow("debug2", drawing1);
    waitKey(0);
    return 0;
}
