#pragma once
#include "Material.h"
#include <iostream>
#include <cstring>

class Document : public Material {
private:
    char filePath[100];
public:
    Document();
    Document(int id, const char* title, const char* path);
    void show() override;
};
