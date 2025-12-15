#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <cstring> 

using namespace std;



class Material {
private:
    int materialID;
    char title[50];

public:

    Material();
    Material(int id, const char* title);
    virtual ~Material() = default;


    int getMaterialID() const { return materialID; }
    char* getTitle() { return title; }


    virtual void show() = 0;
    void upload();
};

#endif // MATERIAL_H
