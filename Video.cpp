#include "Video.h"
#include <iostream>
#include <cstring>
using namespace std;

Video::Video() : Material(), duration(0) {}
Video::Video(int id, const char* title, int dur): Material(id, title), duration(dur) 
{}

void Video::show() 
{
    cout << "\n--- Video Material ---" << endl;
    cout << "  Title: " << getTitle() << endl;
    cout << "  ID: " << getMaterialID() << endl;
    cout << "  Duration: " << duration << " seconds" << endl;
    cout << "  Action: Playing video..." << endl;
}