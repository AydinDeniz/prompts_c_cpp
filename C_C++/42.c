
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

using namespace cv;
using namespace cv::face;

void detectFaces(Mat frame, CascadeClassifier face_cascade) {
    std::vector<Rect> faces;
    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    face_cascade.detectMultiScale(gray, faces, 1.3, 5);

    for (size_t i = 0; i < faces.size(); i++) {
        rectangle(frame, faces[i], Scalar(255, 0, 0), 2);
    }
    imshow("Face Detection", frame);
}

int main() {
    CascadeClassifier face_cascade;
    if (!face_cascade.load("haarcascade_frontalface_default.xml")) {
        printf("Error loading face cascade\n");
        return -1;
    }

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        printf("Error opening video stream\n");
        return -1;
    }

    while (true) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        detectFaces(frame, face_cascade);

        if (waitKey(30) == 27) break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
