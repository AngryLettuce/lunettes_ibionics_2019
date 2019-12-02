package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.Switch;

import org.opencv.android.JavaCameraView;
import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;

import org.opencv.core.Core;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.RotatedRect;
import org.opencv.core.Scalar;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;
import android.view.View;

import java.util.ArrayList;
import java.util.List;

public class backCameraActivity extends AppCompatActivity implements CvCameraViewListener2{
    // Used for logging success or failure messages
    private static final String TAG = "OCVSample::Activity";

    // Loads camera view of OpenCV for us to use. This lets us see using OpenCV
    private CameraBridgeViewBase mOpenCvCameraView;

    // Used in Camera selection from menu (when implemented)
    private boolean              mIsJavaCamera = true;
    private MenuItem             mItemSwitchCamera = null;

    float alpha = 10;
    float beta = 0;
    // These variables are used (at the moment) to fix camera orientation from 270degree to 0degree
    Mat mRgba;
    Mat mRgbaF;
    Mat mRgbaT;
    Mat resizeimage;
    Mat grayImage;

    Mat detectedEdges;
    Mat grad_x;
    Mat grad_y;
    Mat abs_grad_x;
    Mat abs_grad_y;
    int image_width;
    int image_height;
    Mat implant;
    Mat cropped;
    Size szImplant;
    int small_height = 16;
    int small_width = 18;
    public SeekBar seekBarZoom;
    public SeekBar seekBarThreshold;
    double zoom = 1;
    Size orig;
    int offx;
    int offy;
    int rowEnd;
    int colEnd;

    public boolean switch_HoughCircles_state = false;
    public boolean switch_EllipsFitting_state = false;
    public boolean switch_ThresholdInv_state = false;
    public double seekBarValue = 1;
    public int thresholdValue = 15;

    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");
                    mOpenCvCameraView.enableView();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };

    public backCameraActivity() {
        Log.i(TAG, "Instantiated new " + this.getClass());
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.back_camera);
        mOpenCvCameraView = (JavaCameraView) findViewById(R.id.show_camera_activity_java_surface_view2_back);
        mOpenCvCameraView.setMaxFrameSize(720, 1280);
        mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);
        mOpenCvCameraView.setCvCameraViewListener(this);


        final Switch houghCircles_Switch = findViewById(R.id.switch_HoughCircle_back);
        final Switch ellipsFitting_Switch = findViewById(R.id.switch_EllipsFitting_back);
        final Switch thresholdInv_switch = findViewById(R.id.threhold_inv_back);
        thresholdInv_switch.setVisibility(View.INVISIBLE);
        final ImageButton switch_activity = findViewById(R.id.switch_activity2_back);

        seekBarZoom=findViewById(R.id.seekBarZoomid3_back);
        seekBarZoom.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                seekBarValue = 1-((double) progress/255);
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        seekBarThreshold=findViewById(R.id.ellipseThreshold_back);
        seekBarThreshold.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                thresholdValue = progress;
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });
        houghCircles_Switch.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v1) {

                // check current state of a Switch (true or false).
                Boolean switchState = houghCircles_Switch.isChecked();
                if (switchState == true){
                    Log.d(TAG, "Switch Activated");
                    switch_HoughCircles_state = true;
                }
                else{
                    Log.d(TAG, "Switch not Activated");
                    switch_HoughCircles_state = false;
                }
            }
        });

        ellipsFitting_Switch.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v2) {

                // check current state of a Switch (true or false).
                Boolean switchState = ellipsFitting_Switch.isChecked();
                if (switchState == true){
                    Log.d(TAG, "Switch Activated");
                    switch_EllipsFitting_state = true;
                    thresholdInv_switch.setVisibility(View.VISIBLE);

                }
                else{
                    Log.d(TAG, "Switch not Activated");
                    switch_EllipsFitting_state = false;
                    thresholdInv_switch.setVisibility(View.INVISIBLE);
                }
            }
        });

        thresholdInv_switch.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v4) {

                // check current state of a Switch (true or false).
                Boolean switchState = thresholdInv_switch.isChecked();
                if (switchState == true){
                    Log.d(TAG, "Switch Activated");
                    switch_ThresholdInv_state = true;
                }
                else{
                    Log.d(TAG, "Switch not Activated");
                    switch_ThresholdInv_state = false;
                }
            }
        });


        switch_activity.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v3) {
                startActivity(new Intent(backCameraActivity.this, MainActivity.class));
            }
        });

        mOpenCvCameraView.setOnClickListener(new DoubleClickListener() {

            @Override
            public void onSingleClick(View v) {

            }

            @Override
            public void onDoubleClick(View v) {
                startActivity(new Intent(backCameraActivity.this, frontCameraActivity.class));
            }
        });
    }
    @Override
    public void onPause()
    {
        super.onPause();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }
    @Override
    public void onResume()
    {
        super.onResume();
        if (!OpenCVLoader.initDebug()) {
            Log.d(TAG, "Internal OpenCV library not found. Using OpenCV Manager for initialization");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, this, mLoaderCallback);
        } else {
            Log.d(TAG, "OpenCV library found inside package. Using it!");
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
    }
    public void onDestroy() {
        super.onDestroy();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }
    public void onCameraViewStarted(int width, int height) {
        mRgba = new Mat(height, width, CvType.CV_8U);
        mRgbaF = new Mat(height, width, CvType.CV_16U);
        mRgbaT = new Mat(width, width, CvType.CV_8UC4);
        resizeimage = new Mat(height, width, CvType.CV_8UC4);
        grayImage = new Mat();
        detectedEdges = new Mat();
        grad_x = new Mat();
        grad_y = new Mat();
        abs_grad_x = new Mat();
        abs_grad_y = new Mat();
        implant = new Mat();
        szImplant = new Size(height, width);
        cropped = new Mat();
        image_width = width;
        image_height = height;
        cropped = new Mat();
    }

    public void onCameraViewStopped() {
        mRgba.release();
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
        // Get black and white input image
        Mat mRgba = inputFrame.rgba();

        // Auto brightness control
        /*
        Core.MinMaxLocResult mmr = Core.minMaxLoc(gray);
        int mRgba_max_val = (int) mmr.maxVal; //Get max pixel level
        int mRgba_min_val = (int) mmr.minVal; //Get min pixel level
        int input_range = mRgba_max_val - mRgba_min_val;
        if(input_range == 0){ //Avoid division by zero crash
            Log.d(TAG, "Zero!!!");
            input_range = 1;
        }
        alpha = 255/input_range; // Alpha is a global pixel multiplier
        beta = -mRgba_min_val * alpha; // Beta is a global pixel offset
        gray.convertTo(gray, -1, alpha, beta); //Apply brightness control
        */
        try{
            //Apply region of interest
            zoom = seekBarValue; // Eventually link with slider

            // Find bounds of region of interest
            orig = mRgba.size();
            offx = (int) (0.5 * (1.0 - zoom) * orig.width);
            offy = (int) (0.5 * (1.0 - zoom) * orig.height);
            rowEnd = (int) (orig.height - offy);
            colEnd = (int) (orig.width - offx);

            // crop the part, you want to zoom into:
            cropped = mRgba.submat(offy, rowEnd, offx, colEnd);
            // resize to original screen resolution
            Imgproc.resize(cropped, cropped, orig); //Maybe remove?
        }
        catch(Exception e){
            Log.d(TAG, "An Error occured while zooming");
        }
        // Image processing starts here
        if (switch_HoughCircles_state) {
            Imgproc.cvtColor(cropped, grayImage, Imgproc.COLOR_BGR2GRAY);
            Imgproc.medianBlur(grayImage, grayImage, 5);
            Mat circles = new Mat();
            Imgproc.HoughCircles(grayImage, circles, Imgproc.HOUGH_GRADIENT, 1.0,
                    (double)grayImage.rows()/16, // change this value to detect circles with different distances to each other
                    100.0, 30.0, 1, 100); // change the last two parameters

            if (circles.cols() > 0) {
                double[] c = circles.get(0, 0);
                Point center = new Point(Math.round(c[0]), Math.round(c[1]));
                // circle center
                Imgproc.circle(cropped, center, 1, new Scalar(0,100,100), 2, 8, 0 );
                // circle outline
                int radius = (int) Math.round(c[2]);
                Imgproc.circle(cropped, center, radius, new Scalar(255,0,255), 3, 8, 0 );
            }

        }
        if(switch_EllipsFitting_state){
            int threshold = thresholdValue;
            int maxval = 255;
            Size kernelSize = new Size(8, 8);

            Imgproc.cvtColor(cropped, grayImage, Imgproc.COLOR_BGR2GRAY);
            Imgproc.medianBlur(grayImage, grayImage, 5);
            Imgproc.threshold(grayImage, grayImage, threshold, maxval, Imgproc.THRESH_BINARY_INV);
            if(switch_ThresholdInv_state){
                return grayImage;
            }
            Mat element = Imgproc.getStructuringElement(Imgproc.MORPH_RECT, kernelSize);

            Imgproc.morphologyEx(grayImage, grayImage, Imgproc.MORPH_OPEN, element);

            Imgproc.morphologyEx(grayImage, grayImage, Imgproc.MORPH_CLOSE, element);

            List<MatOfPoint> contours = new ArrayList<>();
            Mat hierarchy = new Mat();
            Imgproc.findContours(grayImage, contours, hierarchy, Imgproc.RETR_TREE, Imgproc.CHAIN_APPROX_SIMPLE);

            if (contours.size() != 0){
                double area_max = 0;
                int index_area_max = 0;
                for(int i = 0; i < contours.size(); i++){
                    double area = Imgproc.contourArea(contours.get(i), false);
                    if(area > area_max){
                        index_area_max = i;
                        area_max = area;
                    }
                }

                if(contours.get(index_area_max).height() > 5){
                    RotatedRect minEllipse = Imgproc.fitEllipse(new MatOfPoint2f(contours.get(index_area_max).toArray()));
                    Imgproc.ellipse(cropped, minEllipse, new Scalar(255,0,255), 2);
                    Imgproc.circle(cropped, minEllipse.center, 2, new Scalar(255,0,255), 3, 8, 0 );
                }
            }

        }

        return cropped; // This function must return
    }

}



