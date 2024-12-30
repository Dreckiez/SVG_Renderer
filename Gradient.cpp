#include "Gradient.h"

Gradient::Gradient(){
    id = "";
    start.SetPoint(0,0);
    amount = 0;
}

Gradient::Gradient(string I, Shapes::Point S){
    id = I;
    start.SetPoint(S.GetX(), S.GetY());
    amount = 0;
}

Shapes::Point& Gradient::get_start(){
    return start;
}

Shapes::Point& LinearGradient::get_end(){
    return end;
}

std::string& Gradient::get_id(){
    return id;
}

void Gradient::set_start(Shapes::Point p){
    start.SetPoint(p.GetX(), p.GetY());
}

void LinearGradient::set_end(Shapes::Point p){
    end.SetPoint(p.GetX(), p.GetY());
}

double* Gradient::get_stops(){
    return stops;
}

Gdiplus::Color* Gradient::get_colors(){
    return colors;
}

vector <Gradient*>& GradientVector::get_content(){
    return content;
}

int Gradient::get_amount(){
    return amount;
}

void Gradient::set_amount(int s){
    amount = s;
}

void Gradient::set_id(std::string str){
    id = str;
}

void LinearGradient::read(XMLElement* gradientElem){
    int idx;
    set_id(gradientElem->Attribute("id"));
    // Set start and end pos;
    if(gradientElem->Attribute("x1")){
        float x1 = atof(gradientElem->Attribute("x1")), y1 = atof(gradientElem->Attribute("y1"));
        float x2 = atof(gradientElem->Attribute("x2")), y2 = atof(gradientElem->Attribute("y2"));
        Shapes::Point s(x1,y1), e(x2,y2);
        set_start(s);
        set_end(e);
    }
    else{
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        Shapes::Point s(0,0), e(screenWidth,0);
        set_start(s);
        set_end(e);
    }
    if(gradientElem->Attribute("gradientTransform")){
        Transform = (gradientElem->Attribute("gradientTransform"));
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
            get_stops()[idx] = 0;
            get_colors()[idx] = c;
            set_amount(get_amount() + 1);
            idx++;
        }
        get_stops()[idx] = offset;
        get_colors()[idx] = c;
        idx++;
        set_amount(get_amount() + 1);
    }
    if(get_stops()[idx-1] != 1){
        get_stops()[idx] = 1;
        get_colors()[idx] = get_colors()[idx-1];
        set_amount(get_amount() + 1);
    }
}

float RadialGradient::get_radius(){
    return radius;
}

void RadialGradient::read(XMLElement* gradientElem){
    int idx;
    set_id(gradientElem->Attribute("id"));
    // Set start and end pos;
    float x1 = 0, y1 = 0;
    if(gradientElem->Attribute("cx")){
        x1 = atof(gradientElem->Attribute("cx"));
    }
    if(gradientElem->Attribute("cy")){
        y1 = atof(gradientElem->Attribute("cy"));
    }
    if(gradientElem->Attribute("r")){
        radius = atof(gradientElem->Attribute("r"));
    }
    else{
        radius = 0;
    }
    if(gradientElem->Attribute("gradientTransform")){
        Transform = (gradientElem->Attribute("gradientTransform"));
    }
    Shapes::Point s(x1,y1);
    set_start(s);
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
            get_stops()[idx] = 0;
            get_colors()[idx] = c;
            set_amount(get_amount() + 1);
            idx++;
        }
        get_stops()[idx] = offset;
        get_colors()[idx] = c;
        idx++;
        set_amount(get_amount() + 1);
    }
    if(get_stops()[idx-1] != 1){
        get_stops()[idx] = 1;
        get_colors()[idx] = get_colors()[idx-1];
        set_amount(get_amount() + 1);
    }
}

void GradientVector::read_gradient(tinyxml2::XMLElement* defs){
    int idx;
    if(!defs->FirstChildElement("linearGradient") && !defs->FirstChildElement("radialGradient")){
        return;
    }
    XMLElement* gradientElem = defs->FirstChildElement();
    while (gradientElem) {
        Gradient* LG = nullptr;
        
        if(string(gradientElem->Name()) == "linearGradient"){
            LG = new LinearGradient;
            LG->read(gradientElem);
        }
        else if(string(gradientElem->Name()) == "radialGradient"){
            LG = new RadialGradient;
            LG->read(gradientElem);
        }
        gradientElem = gradientElem->NextSiblingElement();
        // Add the gradient to the vector
        content.push_back(LG);
    }
    return;
}

void GradientVector::clear(){
    for(int i = 0; i < content.size(); i++){
        delete content[i];
    }
    content.clear();
}

void Gradient::setTransform(Gdiplus::LinearGradientBrush* gb, float s, Gdiplus::PointF anchor){
    string type, para;
    float translate_x = 0, translate_y = 0, rotate = 0, scale_x = 1, scale_y = 1;
    stringstream ss(Transform);
    while(getline(ss, type, '(')){
        if(type == "translate"){
            getline(ss, para, ')');
            int size = para.length();
            string number_string;
            for(int i = 0; i < size; i++){
                if(para[i] != ',' && para[i] != ' '){
                    number_string += para[i];
                }
                else if(translate_x == 0){
                    translate_x = stof(number_string);
                    number_string = "";
                    if(para[i+1] == ' ' || para[i+1] == ','){
                        i++;
                    }
                }
                if(i == size-1){
                    translate_y = stof(number_string);
                }
            }
            getline(ss, para, ' ');
            translate_x*=s;
            translate_y*=s;
            gb->TranslateTransform(-translate_x/3.3, -translate_y/3.3);
            translate_x = 0;
            translate_y = 0;
        }
        else if(type == "scale"){
            string temp;
            getline(ss, temp, ')');
            stringstream scale_stream(temp);
            if (temp.find(',') != std::string::npos){
                getline(scale_stream, para, ',');
                scale_x = atof(para.c_str());
                getline(scale_stream, para, ')');
                scale_y = atof(para.c_str());
                getline(ss, para, ' ');
            }
            else{
                getline(scale_stream, para, ')');
                scale_x = atof(para.c_str());
                scale_y = atof(para.c_str());
                getline(ss, para, ' ');
            }
            gb->ScaleTransform(scale_x, scale_y);
        }
        else if(type == "rotate"){
            getline(ss, para, ')');
            rotate = atof(para.c_str());
            float radian = rotate * 3.1415926 / (float)180;
            getline(ss, para, ' ');
            gb->RotateTranform(rotate);
        }
    }
}