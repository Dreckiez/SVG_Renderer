#ifndef STYLE_H
#define STYLE_H

#include <string>
#include <sstream>
#include <vector>
#include "objects.h"

using namespace std;

class Style{
private:
    Shapes::Object style;
    vector<string> name;
    // string type;
public:
    Style();
    Style(const Style& );
    void setStyle(string s);
    Shapes::Object getStyle();

    void setName(vector<string> name);
    void addName(string name);
    vector<string> getName();

    void setObjectStyle(string s, Shapes::Object* obj);

    // void setType(string type);  //General, Shared and Specific
    // string getType();
};

class StyleGroup{
private:
    vector<Style> general;  //Apply to all same element
    vector<Style> shared;   //Apply only when same class
public:
    void addStyle(string s);

    void addGeneralStyle(Style gen);
    vector<Style> getGeneralStyle();

    void addSharedStyle(Style shared);
    vector<Style> getSharedStyle();
};

#endif