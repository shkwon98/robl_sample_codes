#include <opencv2/opencv.hpp>

#include <iostream>
#include <algorithm>

using namespace cv;
using namespace std;

bool isMousePressed = false;
int start_x, start_y, end_x, end_y;
int step = 0;
Mat img_color;
Rect roi;

void mouse_callback(int event, int x, int y, int flags, void* userdata)
{
    Mat img_result = img_color.clone();

    if (event == EVENT_LBUTTONDOWN)
    {
        step = 1;
        isMousePressed = true;
        start_x = x;
        start_y = y;
    }
    else if (event == EVENT_MOUSEMOVE)
    {
        if (isMousePressed)
        {
            end_x = x;
            end_y = y;
            step = 2;
        }
    }
    else if (event == EVENT_LBUTTONUP)
    {
        isMousePressed = false;
        end_x = x;
        end_y = y;
        step = 3;
    }
}

int main()
{
    Mat img_hsv, img_mask, img_ROI, mask_ROI;
    Mat objectHistogram;

    int channels[] = { 0 };
    int hsize[] = { 64 };
    float range1[] = { 0, 180 };
    const float* histRange[] = { range1 };

    VideoCapture cap("../data/yunna.mp4");
    if (!cap.isOpened())
    {
        cerr << "video 에러 - 영상을 열 수 없습니다.\n";
        return -1;
    }

    namedWindow("Color", 1);
    setMouseCallback("Color", mouse_callback);

    Mat img_sceen;
    cap.read(img_sceen);

    while (1)
    {
        if (step == 4)
            cap.read(img_color);
        else
            img_sceen.copyTo(img_color);

        if (img_color.empty())
        {
            cerr << "빈 영상이 캡쳐되었습니다.\n";
            break;
        }

        switch (step)
        {

            case 1:
                circle(img_color, Point(start_x, start_y), 10, Scalar(0, 255, 0), -1);
                break;

            case 2:
                rectangle(img_color, Point(start_x, start_y), Point(end_x, end_y), Scalar(0, 255, 0), 3);
                break;

            case 3:
                if (start_x > end_x)
                {
                    swap(start_x, end_x);
                    swap(start_y, end_y);
                }

                roi = Rect(start_x, start_y, end_x - start_x, end_y - start_y);
                cvtColor(img_color, img_hsv, COLOR_BGR2HSV);
                inRange(img_hsv, Scalar(0., 30., 30.), Scalar(180., 255., 255.), img_mask);

                img_ROI = Mat(img_hsv, roi);
                mask_ROI = Mat(img_mask, roi);

                imshow("ROI", img_ROI);

                calcHist(&img_ROI, 1, channels, mask_ROI, objectHistogram, 1, hsize, histRange);
                normalize(objectHistogram, objectHistogram, 0, 255, NORM_MINMAX);

                step++;
                break;

            case 4:
                Mat bp;
                cvtColor(img_color, img_hsv, COLOR_BGR2HSV);

                calcBackProject(&img_hsv, 1, channels, objectHistogram, bp, histRange);
                bp &= img_mask;

                // Tracking
                RotatedRect rot_rect = CamShift(bp, roi, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));

                if (roi.area() <= 1)
                {
                    int cols = bp.cols, rows = bp.rows, r = (MIN(cols, rows) + 5) / 6;
                    roi = Rect(roi.x - r, roi.y - r, roi.x + r, roi.y + r) & Rect(0, 0, cols, rows);
                }

                ellipse(img_color, rot_rect, Scalar(0, 0, 255), 3, LINE_AA);
                Point2f vertices[4];
                rot_rect.points(vertices);
                for (int i = 0; i < 4; i++)
                    line(img_color, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0), 2);

                break;
        }

        //if (step < 4)
        cout << step << endl;

        imshow("Color", img_color);

        if (waitKey(25) >= 0)
            break;
    }

    return 0;
}