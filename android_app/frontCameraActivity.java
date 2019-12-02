package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
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
import org.opencv.core.Point;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Scalar;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;
import android.view.View;

public class frontCameraActivity extends AppCompatActivity implements CvCameraViewListener2{
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
    double zoom = 1;
    Size orig;
    int offx;
    int offy;
    int rowEnd;
    int colEnd;

    public boolean switch_HoughCircles_state = false;
    //public boolean switch_EllipsFitting_state = false;
    public double seekBarValue = 1;

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

    public frontCameraActivity() {
        Log.i(TAG, "Instantiated new " + this.getClass());
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.front_camera);
        mOpenCvCameraView = (JavaCameraView) findViewById(R.id.show_camera_activity_java_surface_view);
        mOpenCvCameraView.setMaxFrameSize(720, 1280);
        mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);
        mOpenCvCameraView.setCvCameraViewListener(this);


        final Switch houghCircles_Switch = findViewById(R.id.switch_HoughCircle);
        //final Switch ellipsFitting_Switch = findViewById(R.id.switch_EllipsFitting);
        final ImageButton switch_Camera = findViewById(R.id.switch_camera);

        seekBarZoom=findViewById(R.id.seekBarZoomid);
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
        /*
        ellipsFitting_Switch.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v2) {

                // check current state of a Switch (true or false).
                Boolean switchState = ellipsFitting_Switch.isChecked();
                if (switchState == true){
                    Log.d(TAG, "Switch Activated");
                    switch_EllipsFitting_state = true;
                }
                else{
                    Log.d(TAG, "Switch not Activated");
                    switch_EllipsFitting_state = false;
                }
            }
        });
        */

        switch_Camera.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v3) {
                startActivity(new Intent(frontCameraActivity.this, MainActivity.class));
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
        Mat gray = new Mat();
        Imgproc.cvtColor(mRgba, gray, Imgproc.COLOR_BGR2GRAY);


        // Auto brightness control
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

        try{
            //Apply region of interest
            zoom = seekBarValue; // Eventually link with slider

            // Find bounds of region of interest
            orig = gray.size();
            offx = (int) (0.5 * (1.0 - zoom) * orig.width);
            offy = (int) (0.5 * (1.0 - zoom) * orig.height);
            rowEnd = (int) (orig.height - offy);
            colEnd = (int) (orig.width - offx);

            // crop the part, you want to zoom into:
            cropped = gray.submat(offy, rowEnd, offx, colEnd);
            // resize to original screen resolution
            Imgproc.resize(cropped, cropped, orig); //Maybe remove?
        }
        catch(Exception e){
            Log.d(TAG, "An Error occured while zooming");
        }
        // Image processing starts here
        if (switch_HoughCircles_state) {
            Imgproc.medianBlur(cropped, cropped, 5);
            Mat circles = new Mat();
            Imgproc.HoughCircles(cropped, circles, Imgproc.HOUGH_GRADIENT, 1.0,
                    (double)cropped.rows()/16, // change this value to detect circles with different distances to each other
                    100.0, 30.0, 1, 30); // change the last two parameters

            for (int x = 0; x < circles.cols(); x++) {
                double[] c = circles.get(0, x);
                Point center = new Point(Math.round(c[0]), Math.round(c[1]));
                // circle center
                Imgproc.circle(mRgba, center, 1, new Scalar(0,100,100), 2, 8, 0 );
                // circle outline
                int radius = (int) Math.round(c[2]);
                Imgproc.circle(mRgba, center, radius, new Scalar(255,0,255), 3, 8, 0 );
            }
        }

        return mRgba; // This function must return
    }

}


