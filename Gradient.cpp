#include "Gradient.h"

LinearGradient::LinearGradient(){
    id = "";
    start.SetPoint(0,0);
    end.SetPoint(0,0);
    amount = 0;
}

LinearGradient::LinearGradient(string I, Shapes::Point S, Shapes::Point E){
    id = I;
    start.SetPoint(S.GetX(), S.GetY());
    end.SetPoint(E.GetX(), E.GetY());
    amount = 0;
}

Shapes::Point& LinearGradient::get_start(){
    return start;
}

Shapes::Point& LinearGradient::get_end(){
    return end;
}

std::string& LinearGradient::get_id(){
    return id;
}

void LinearGradient::set_start(Shapes::Point p){
    start.SetPoint(p.GetX(), p.GetY());
}

void LinearGradient::set_end(Shapes::Point p){
    end.SetPoint(p.GetX(), p.GetY());
}

float* LinearGradient::get_stops(){
    return stops;
}

Gdiplus::Color* LinearGradient::get_colors(){
    return colors;
}

vector <LinearGradient>& LinearVector::get_content(){
    return content;
}

int LinearGradient::get_amount(){
    return amount;
}

void LinearGradient::set_amount(int s){
    amount = s;
}

void LinearGradient::set_id(std::string str){
    id = str;
}

void LinearVector::read_gradient(tinyxml2::XMLElement* defs){
    int idx;
    for (XMLElement* gradientElem = defs->FirstChildElement("linearGradient");
         gradientElem; gradientElem = gradientElem->NextSiblingElement("linearGradient")) {
        
        LinearGradient LG;
        LG.set_id(gradientElem->Attribute("id"));
        // Set start and end pos;
        if(gradientElem->Attribute("x1")){
            float x1 = atof(gradientElem->Attribute("x1")), y1 = atof(gradientElem->Attribute("y1"));
            float x2 = atof(gradientElem->Attribute("x2")), y2 = atof(gradientElem->Attribute("y2"));
            Shapes::Point s(x1,y1), e(x2,y2);
            LG.set_start(s);
            LG.set_end(e);
        }
        else{
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            Shapes::Point s(0,0), e(screenWidth,0);
            LG.set_start(s);
            LG.set_end(e);
        }
        idx = 0;
        // Read all <stop> elements
        for (XMLElement* stopElem = gradientElem->FirstChildElement("stop");
             stopElem; stopElem = stopElem->NextSiblingElement("stop")) {
            float offset = 0.0f;
            stopElem->QueryFloatAttribute("offset", &offset);
            string colorHex = stopElem->Attribute("stop-color");
            float stopOpacity = 1.0f;
            stopElem->QueryFloatAttribute("stop-opacity", &stopOpacity);
            Shapes::RGBA color;
            color.SetRGB(colorHex);
            color.SetAlpha(stopOpacity);
            Gdiplus::Color c(stopOpacity*255, color.GetRed(), color.GetGreen(), color.GetBlue());
            if(idx == 0 && offset != 0){
                LG.get_stops()[idx] = 0;
                LG.get_colors()[idx] = c;
                LG.set_amount(LG.get_amount() + 1);
                idx++;
            }
            LG.get_stops()[idx] = offset;
            LG.get_colors()[idx] = c;
            idx++;
            LG.set_amount(LG.get_amount() + 1);
        }
        // Add the gradient to the vector
        if(LG.get_stops()[idx-1] != 1){
            LG.get_stops()[idx] = 1;
            LG.get_colors()[idx] = LG.get_colors()[idx-1];
            LG.set_amount(LG.get_amount() + 1);
        }
        content.push_back(LG);
    }
    return;
}