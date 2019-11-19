#include "testvideo.h"

int mouseX = 0;
int mouseY = 0;

//function for testing algo precition
static void onMouse( int event, int x, int y, int, void* )
{
    if( event == cv::EVENT_LBUTTONDOWN )
    {
        //cv::Point seed = cv::Point(x,y);
        //std::cout<< "mouse postion : " << seed << std::endl;
        mouseX = x;
        mouseY = y;
        return;
    }

}

inline std::string getCurrentDateTime( std::string s ){
    time_t now = time(0);
    struct tm  tstruct;
    char  buf[80];
    tstruct = *localtime(&now);
    if(s=="now")
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    else if(s=="date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return std::string(buf);
}

inline void Logger( std::string logMsg ){

    std::string filePath = "log_"+getCurrentDateTime("date")+".txt";
    std::string now = getCurrentDateTime("now");
    std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << now << '\t' << logMsg << '\n';
    ofs.close();
}

//write a video for testing
void writeCamVideo(char *filename)
{
    int indexCam = 0;
    cv::VideoCapture camera(indexCam); //index for camera

    if(!camera.isOpened())
    {
        std::cout << "erreur sur l'ouverture de la camera "<< indexCam << std::endl;
    }

    //window for preview
    cv::namedWindow("record feed",cv::WINDOW_AUTOSIZE);

    double camWidth = camera.get(cv::CAP_PROP_FRAME_WIDTH);
    double camHeight = camera.get(cv::CAP_PROP_FRAME_HEIGHT);

    //save frame size
    cv::Size frameSize(static_cast<int>(camWidth), static_cast<int>(camHeight));

    //initialiser videoWriter
    cv::VideoWriter VideoFile(filename, cv::VideoWriter::fourcc('P','I','M','1'), camera.get(cv::CAP_PROP_FPS), frameSize, true);
    //PIM1 for mpeg-1


    if(!VideoFile.isOpened())
    {
        std::cout << "erreur dans ouverture du ficher" << std::endl;
    }

    cv::Mat frame;

    camera.read(frame);
    cv::imshow("record feed",frame);

    //wait for keypress to start recording
    while(1)
    {

        camera.read(frame);
        cv::imshow("record feed",frame);

        char c;
        c = cv::waitKey(10);
        if(c== 'r')
        {
            break;
        }
    }
    while(1)
    {
        bool videoState = camera.read(frame);

        if (!videoState)
        {
            std::cout << "erreur no frame" << std::endl;
        }

        //write file in file
        VideoFile.write(frame);

        //show red circle on feed for prove of recordin
        cv::circle(frame, cv::Point(10,10),5,cv::Scalar(0,0,255),-1,8,0);


        //show preview
        cv::imshow("record feed",frame);

        if(cv::waitKey(10)== 27)
        {
            break;
        }

    }


}

//write in doc point of center of pupil
void getPointFromVideo(char *filename)
{
    //get video from file
    cv::VideoCapture videoFile(filename);

    if(!videoFile.isOpened())
    {
        std::cout << "error file empty" << std::endl;
    }

    cv::Mat frame,frame2;

    cv::namedWindow( "image", 0 );
    cv::setMouseCallback( "image", onMouse, 0 );

    char c;

    while(1)
    {
        videoFile >> frame;


        if(frame.empty())
        {
            std::cout << "End of video"<< std::endl;
            break;
        }
        cv::imshow("image",frame);
        c = (char) cv::waitKey(10);

        while(1)
        {
            //get position of mouse
            //std::cout << "position recue : ( " << mouseX << ", " << mouseY << " )" << std::endl;

            frame2 = frame.clone();
            cv::circle(frame2,cv::Point(mouseX,mouseY),5,cv::Scalar(0,0,255),-1,8,0);
            cv::imshow("image",frame2);
            c = (char) cv::waitKey(10);

            if (c == 'q')
            {
                //cv::imshow("image",frame);
                break;
            }
            //std::cout << "next frame" << std::endl;

        }

        //cv::imshow("image",frame);
        std::cout << "next frame" << std::endl;

        //log position
        //decouper en frame?
        //identifier la frame et link la position?
        std::string text = "position X : ";
        text += std::to_string(mouseX);
        Logger(text);


        //c = (char) cv::waitKey(0);


        //next frame

    }

    videoFile.release();

}
