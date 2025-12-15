#pragma once
#include "Material.h"
#include <iostream>
#include <cstring>

class Video : public Material 
{
private:
    int duration;

public:
    Video();
    Video(int id, const char* title, int dur);
    void show() override;
};
