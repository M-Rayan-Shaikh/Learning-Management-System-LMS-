#include "Material.h"
#include <iostream>
#include <cstring> 

using namespace std;

Material::Material() : materialID(0) {
    strcpy_s(title, sizeof(title), "");
}
Material::Material(int id, const char* title)
    : materialID(id) {

    strncpy_s(this->title, sizeof(this->title), title, 50 - 1);
    this->title[50 - 1] = '\0';
}
void Material::upload() 
{
    cout << "Material '" << title << "' (ID: " << materialID << ") is being uploaded..." << endl;
}