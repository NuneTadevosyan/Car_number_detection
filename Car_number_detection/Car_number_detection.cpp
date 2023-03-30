#include <opencv2/core.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/highgui.hpp>s
#include <iostream>
#include <vector>

using namespace cv;
using namespace cv::cuda;
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <file_path>" << endl;
        return 1;
    }
    string filename = argv[1];
    // Read the input image
    Mat car_img = imread(filename);

    // Create a GPU mat and copy the input image to it
    GpuMat gpu_img;
    gpu_img.upload(car_img);

    // Convert RGB image to grayscale
    GpuMat gpu_gray;
    cuda::cvtColor(gpu_img, gpu_gray, COLOR_BGR2GRAY);

    // Apply bilateral filter to reduce noise
    GpuMat gpu_blurred_gray;
    cuda::bilateralFilter(gpu_gray, gpu_blurred_gray, 10, 50, 50);

    // Apply Canny edge detection
    GpuMat gpu_edged;
    auto canny_detector = cuda::createCannyEdgeDetector(50, 250);
    canny_detector->detect(gpu_blurred_gray, gpu_edged);
    //cv::Mat edgedImage(edgedImage_gpu);
    auto gaussianFilter = cuda::createGaussianFilter(CV_8UC1, CV_8UC1, { 3,3 }, 0);
    gaussianFilter->apply(gpu_edged, gpu_edged);

    // Copy the edged image from GPU to CPU
    Mat edged;
    gpu_edged.download(edged);

    //// Find contours in the edged image
    vector<vector<Point>> contours;
    findContours(edged, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

    //// Sort the contours by area in descending order
    sort(contours.begin(), contours.end(), [](const vector<Point>& a, const vector<Point>& b) {
        return contourArea(a) > contourArea(b);
        });

    //// Find the contour with 4 points, which is the screen
    vector<Point> screenCnt;
    for (auto& c : contours) {
        double peri = arcLength(c, true);
        vector<Point> approx;
        approxPolyDP(c, approx, 0.018 * peri, true);
        if (approx.size() == 4) {
            screenCnt = approx;
            break;
        }
    }

    //// If no screen contour is found, print a message
    if (screenCnt.empty()) {
        cout << "No contour detected" << endl;
    }
    else {
        //    // Draw the screen contour on the input image
        vector<vector<Point>> cnts = { screenCnt };
        drawContours(car_img, cnts, -1, Scalar(0, 0, 255), 3);
        imshow("Output", car_img);
        waitKey(0);
    }

    return 0;
}

