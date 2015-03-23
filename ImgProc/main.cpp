#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d/features2d.hpp>
#include "opencv2/highgui/highgui.hpp"


#include <iostream>
#include "preprocess.h"
#include "eventhandler.h"
#include "line.h"
#include "autoruler.h"
extern "C" {
#include "Yin.h"
}

using namespace std;

cv::Mat findBiggestBlob(cv::Mat & matImage);

int main(){

    Mat box = imread("img/pfm20det.png");
    Mat gray;
    Mat detected_edges, blurred;
    cvtColor(box, gray, COLOR_RGB2GRAY);
    

    
    Ptr<MSER> ms  = MSER::create(9,100,1000);

    vector<vector <Point> > regions;
    vector<Rect> boxes;
    ms->detectRegions(gray, regions, boxes);
    int allContours = -1;
    int thickness=2;
    Mat binary(box.size(), CV_8U);
    
    drawContours(binary, regions, allContours, Scalar::all(255), thickness);
    
    imshow("binary",binary);
    
    
    Mat hsvImage, hsvTresh;
    cvtColor(box, hsvImage, COLOR_BGR2HSV);
    imshow("hsvimage", hsvImage);
    inRange(hsvImage,Scalar(18,100,120),Scalar(30,255,230),hsvTresh);
    imshow("hsvtresh", hsvTresh);
    
    Mat biggestYellowBlob = findBiggestBlob(hsvTresh);
    
    imshow("biggestyellow", biggestYellowBlob);
    
    Mat filtered;
    binary.copyTo(filtered, biggestYellowBlob);
    imshow("filtered", filtered);
    filtered.copyTo(binary);
    blur( gray, blurred, Size(3,3) );
    
    Canny( gray, detected_edges, 20, 20*3 );
    
    Mat result;
    binary.copyTo(result, detected_edges);
    
    
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Vec3f> vecCircles;
    vector<Vec3f>::iterator itrCircles;
    
    bitwise_not(binary, binary);
    
    int morph_size = 1;
    Mat element = getStructuringElement( MORPH_ELLIPSE, Size( 2*morph_size + 1, 2*morph_size+1 ) );
    morphologyEx( binary, binary, MORPH_OPEN, element, Point(-1,-1), 1 );
    morphologyEx( binary, binary, MORPH_CLOSE, element, Point(-1,-1), 1 );
    
    bitwise_not(binary, binary);
        
    findContours(binary, contours, hierarchy, RETR_EXTERNAL,  CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );
    
    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        
        minEnclosingCircle( Mat(contours_poly[i]),
                           center[i], radius[i] );
        
        RotatedRect rect = minAreaRect(Mat(contours_poly[i]));
        cout << rect.angle <<endl;
        
        Point2f vertices[4];
        rect.points(vertices);
        for (int i = 0; i < 4; i++)
            line(box, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));
        
        Mat res = box(boundRect[i]);
        
        
    }
    

    /// Draw polygonal contour + bonding rects
    Mat drawing = Mat::zeros( binary.size(), CV_8UC3 );
    char s[3] = "ss";
    
    int deltax = 0;
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar(255,0,255);
        drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        rectangle( box, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
        
    }
    Size newSize(binary.size().width/2, binary.size().height/2);
    /*    for (int i = 0; i < regions.size(); i++)
    {
        ellipse(box, fitEllipse(regions[i]), Scalar(255));
    }*/
    
    resize(binary,             // input image
           binary ,           // result image
           newSize,    // new dimensions
           0,
           0,
           INTER_CUBIC       // interpolation method
           );
    imshow("bin", binary);
    resize(box,             // input image
           box ,           // result image
           newSize,    // new dimensions
           0,
           0,
           INTER_CUBIC       // interpolation method
           );
    imshow("ooo", box);
    imshow("mser", result);
    waitKey(0);
    return 0;


}


Mat findBiggestBlob(cv::Mat &src){
    int largest_area=0;
    int largest_contour_index=0;
    Mat temp(src.rows,src.cols,CV_8UC1);
    Mat dst(src.rows,src.cols,CV_8UC1,Scalar::all(0));
    Rect boundRect;
    src.copyTo(temp);
    
    vector<vector<Point>> contours; // storing contour
    vector<Vec4i> hierarchy;
    
    findContours( temp, contours, hierarchy,RETR_CCOMP, CHAIN_APPROX_SIMPLE );
    
    for( int i = 0; i< contours.size(); i++ ) // iterate
    {
        double a=contourArea( contours[i],false);  //Find the largest area of contour
        if(a>largest_area)
        {
            largest_area=a;
            largest_contour_index=i;
        }
        
    }

    boundRect = boundingRect(Mat(contours[largest_contour_index]));
    drawContours( dst, contours,largest_contour_index, Scalar(255), FILLED, 8, hierarchy );
    rectangle( dst, boundRect.tl(), boundRect.br(), Scalar(255), FILLED, 8, 0 );
    // Draw the largest contour
    return dst;
}