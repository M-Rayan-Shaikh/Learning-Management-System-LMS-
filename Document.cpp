#include "Document.h"
#include <iostream>
#include <cstring>
using namespace std;

Document::Document()
    : Material() {
    strcpy_s(filePath, sizeof(filePath), "");
}

Document::Document(int id, const char* title, const char* path)
    : Material(id, title) {

    strncpy_s(this->filePath, sizeof(this->filePath), path, 100 - 1);
    this->filePath[100 - 1] = '\0';
}

void Document::show() {
    cout << "\n--- Document Material ---" << endl;
    cout << "  Title: " << getTitle() << endl;
    cout << "  ID: " << getMaterialID() << endl;
    cout << "  Relative Path: " << filePath << endl;
    cout << "  Displaying document..." << endl;
}