#include "Style.h"

Style::Style(){
    
}

Style::Style(const Style& other){
    style = other.style;
    name = other.name;
}

void Style::setStyle(string s){
    removeSpareSpaces(s);
    string type;
    string temp = "";
    float opacity = 0;
    stringstream ss(s);
    while(!ss.eof()){
        getline(ss, type, ':');
        if(type == "fill"){
            getline(ss, temp, ';');
            style.SetColor(temp);
        }
        else if(type == "stroke"){
            getline(ss, temp, ';');
            style.SetStroke(temp);
        }else if(type == "opacity"){
            getline(ss, temp, ';');
            style.SetColorAlpha(stof(temp));
            style.SetStrokeAlpha(stof(temp));
        }
    }
}

Shapes::Object Style::getStyle(){
    return style;
}

void Style::setName(vector<string> name){
    this->name = name;
}

void Style::addName(string name){
    this->name.push_back(name);
}

vector<string> Style::getName(){
    return name;
}

// void Style::setType(string type){
//     this->type = type;
// }

// string Style::getType(){
//     return type;
// }



void StyleGroup::addStyle(string s){
    stringstream ss(s);
    string temp = "";
    Style style;

    getline(ss, temp, '{');

    string attri = "";
    getline(ss, attri);
    style.setStyle(attri);
    
    if (s.find('.') == string::npos){
        style.addName(temp);
        addGeneralStyle(style);
    }else if (temp[0] == '.'){
        temp.erase(0, 1);
        replaceCharWithSpaces(temp, '.');
        removeSpareSpaces(s);
        stringstream name(temp);
        while(name >> temp)
            style.addName(temp);
        addSharedStyle(style);
    }
}

void StyleGroup::addGeneralStyle(Style gen){
    general.push_back(gen);
}

vector<Style> StyleGroup::getGeneralStyle(){
    return general;
}

void StyleGroup::addSharedStyle(Style shared){
    this->shared.push_back(shared);
}
vector<Style> StyleGroup::getSharedStyle(){
    return shared;
}