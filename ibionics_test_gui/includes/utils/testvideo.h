#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>


#ifndef TESTVIDEO_H
#define TESTVIDEO_H

extern int mouseX;
extern int mouseY;

//write video from camera feed
void writeCamVideo(char *filename);

//log file
void write_text_to_log_file( const std::string &text );

//get video and log position of eye
void getPointFromVideo(char *filename);


//run algo and test the precision and time to execute


#endif // TESTVIDEO_H
