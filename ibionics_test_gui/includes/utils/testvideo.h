#ifndef TESTVIDEO_H
#define TESTVIDEO_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>

extern int mouseX;
extern int mouseY;

void writeCamVideo(char *filename);

void write_text_to_log_file( const std::string &text );

void getPointFromVideo(char *filename);

#endif // TESTVIDEO_H
