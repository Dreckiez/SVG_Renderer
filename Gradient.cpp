#include "Gradient.h"

Gradient::Gradient(){
    id = "";
    start.SetPoint(0,0);
    amount = 0;
    isObjectBoundingBox = true;
}

Gradient::Gradient(string I, Shapes::Point S){
    id = I;
    start.SetPoint(S.GetX(), S.GetY());
    amount = 0;
    isObjectBoundingBox = true;
}

Shapes::Point& Gradient::get_start(){
    return start;
}

Shapes::Point& Gradient::get_end(){
    return end;
}

std::string& Gradient::get_id(){
    return id;
}

void Gradient::set_start(Shapes::Point p){
    start.SetPoint(p.GetX(), p.GetY());
}

void Gradient::set_end(Shapes::Point p){
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

bool Gradient::getIsBoundingBox(){
    return isObjectBoundingBox;
}

void LinearGradient::read(XMLElement* gradientElem){
    int idx;
    set_id(gradientElem->Attribute("id"));
    // Set start and end pos;
    if(gradientElem->Attribute("x1")){
        float x1 = ConvertUnit(gradientElem->Attribute("x1")), y1 = ConvertUnit(gradientElem->Attribute("y1"));
        float x2 = ConvertUnit(gradientElem->Attribute("x2")), y2 = ConvertUnit(gradientElem->Attribute("y2"));
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
    if(gradientElem->Attribute("gradientUnits") && strcmp(gradientElem->Attribute("gradientUnits"), "userSpaceOnUse") == 0){
        isObjectBoundingBox = false;
    }
    idx = 0;
    // Read all <stop> elements
    for (XMLElement* stopElem = gradientElem->FirstChildElement("stop");
            stopElem; stopElem = stopElem->NextSiblingElement("stop")) {
        float offset = 0.0f;
        stopElem->QueryFloatAttribute("offset", &offset);
        string OffsetString = stopElem->Attribute("offset");
        if(OffsetString.find('%') != std::string::npos){
            offset /= 100;
        }
        const char* Style = stopElem->Attribute("style");
        const char* colorHex = stopElem->Attribute("stop-color");
        const char* O = stopElem->Attribute("stop-opacity");
        float stopOpacity = 1.0f;
        Shapes::RGBA color;
        if(O){
            stopOpacity = atof(O);
        }
        if(colorHex){
            color.SetRGB(colorHex);
        }
        if(Style){
            string StyleString = Style;
            removeSpareSpaces(StyleString);
            stringstream ss(StyleString);
            while(!ss.eof()){
                string type;
                getline(ss, type, ':');
                string parameter;
                getline(ss, parameter, ';');
                removeSpareSpaces(parameter);
                if(type == "stop-color"){
                    color.SetRGB(parameter);
                }
                else if(type == "stop-opacity"){
                    stopOpacity = stof(parameter);
                }
            
            }
        }
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
    float x1 = 50, y1 = 50, fx = 50, fy = 50;
    if(gradientElem->Attribute("cx")){
        x1 = ConvertUnit(gradientElem->Attribute("cx"));
    }
    if(gradientElem->Attribute("cy")){
        y1 = ConvertUnit(gradientElem->Attribute("cy"));
    }
    if(gradientElem->Attribute("r")){
        radius = ConvertUnit(gradientElem->Attribute("r"));
    }
    else{
        radius = 0;
    }
    if(gradientElem->Attribute("fx")){
        fx = ConvertUnit(gradientElem->Attribute("fx"));
    }
    if(gradientElem->Attribute("fy")){
        fy = ConvertUnit(gradientElem->Attribute("fy"));
    }
    if(gradientElem->Attribute("gradientTransform")){
        Transform = (gradientElem->Attribute("gradientTransform"));
    }
    if(gradientElem->Attribute("gradientUnits") && strcmp(gradientElem->Attribute("gradientUnits"), "userSpaceOnUse") == 0){
        isObjectBoundingBox = false;
    }
    Shapes::Point s(x1,y1);
    set_start(s);
    Shapes::Point e(fx,fy);
    set_end(e);
    idx = 0;
    // Read all <stop> elements
    for (XMLElement* stopElem = gradientElem->FirstChildElement("stop");
            stopElem; stopElem = stopElem->NextSiblingElement("stop")) {
        float offset = 0.0f;
        stopElem->QueryFloatAttribute("offset", &offset);
        string OffsetString = stopElem->Attribute("offset");
        if(OffsetString.find('%') != std::string::npos){
            offset /= 100;
        }
        const char* Style = stopElem->Attribute("style");
        const char* colorHex = stopElem->Attribute("stop-color");
        const char* O = stopElem->Attribute("stop-opacity");
        float stopOpacity = 1.0f;
        Shapes::RGBA color;
        if(O){
            stopOpacity = atof(O);
        }
        if(colorHex){
            color.SetRGB(colorHex);
        }
        if(Style){
            string StyleString = Style;
            removeSpareSpaces(StyleString);
            stringstream ss(StyleString);
            while(!ss.eof()){
                string type;
                getline(ss, type, ':');
                string parameter;
                getline(ss, parameter, ';');
                removeSpareSpaces(parameter);
                if(type == "stop-color"){
                    color.SetRGB(parameter);
                }
                else if(type == "stop-opacity"){
                    stopOpacity = stof(parameter);
                }
            
            }
        }
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

void LinearGradient::setTransform(Gdiplus::LinearGradientBrush* gb, float s){
    string type, para;
    float translate_x = 0, translate_y = 0, rotate = 0, scale_x = 1, scale_y = 1;
    stringstream ss(Transform);
    while(getline(ss, type, '(')){
        removeSpareSpaces(type);
        if(type == "translate"){
            getline(ss, para, ')');
            addSpaces(para);
            removeSpareSpaces(para);
            stringstream ssPara(para);
            ssPara >> translate_x >> translate_y;
            translate_x*=s;
            translate_y*=s;
            if(start.GetX() < end.GetX())   gb->TranslateTransform(translate_x, translate_y);
            else gb->TranslateTransform(-translate_x/2, -translate_y/2);
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

void Gradient::addStops(int& stops_amount, float alpha, Gdiplus::Color color_array[50], float stop_array[50]){
    int idx = 0;
    for(int i = 0; i < stops_amount; i++){
        float opacity = alpha * ((float)colors[idx].GetAlpha());
        if(i == 0){
            Gdiplus::Color c(opacity, colors[idx].GetRed(), colors[idx].GetGreen(), colors[idx].GetBlue());
            color_array[i] = c;
            stop_array[i] = 0;
            i++;
            stops_amount++;
        }

        if(opacity < 50 && idx != 0){
            Gdiplus::Color backup(alpha * ((colors[idx].GetAlpha() + colors[idx-1].GetAlpha())/2),  colors[idx-1].GetRed()*0.5 + colors[idx].GetRed()*0.5, colors[idx-1].GetGreen()*0.5 + colors[idx].GetGreen()*0.5, colors[idx-1].GetBlue()*0.5 + colors[idx].GetBlue()*0.5);
            color_array[i] = backup;
            stop_array[i] = (get_stops()[idx] - ((get_stops()[idx] - get_stops()[idx-1]) / 2) + 3)/7;
            i++;
            stops_amount++;
        }
        Gdiplus::Color c(opacity, colors[idx].GetRed(), colors[idx].GetGreen(), colors[idx].GetBlue());
        color_array[i] = c;
        stop_array[i] = (get_stops()[idx]+ 3) / 7;

        if(opacity  < 50 && idx < get_amount() - 1){
            i++;
            Gdiplus::Color backup(alpha * (colors[idx].GetAlpha() + colors[idx+1].GetAlpha())/2,  colors[idx+1].GetRed()*0.5 + colors[idx].GetRed()*0.5, colors[idx+1].GetGreen()*0.5 + colors[idx].GetGreen()*0.5, colors[idx+1].GetBlue()*0.5 + colors[idx].GetBlue()*0.5);
            color_array[i] = backup;
            stop_array[i] = (get_stops()[idx] + ((get_stops()[idx+1] - get_stops()[idx]) / 2) + 3) / 7;
            stops_amount++;
        }

        if(i == stops_amount - 1){
            i++;
            Gdiplus::Color backup(opacity, colors[idx].GetRed(), colors[idx].GetGreen(), colors[idx].GetBlue());
            color_array[i] = backup;
            stop_array[i] = 1;
            stops_amount++;
        }
        idx++;
    }
}

void LinearGradient::setBrush(Shapes::Object* obj, Gdiplus::LinearGradientBrush*& gb, float alpha, float s){
    Gdiplus::Color color_array[50];
    float stop_array[50];
    Gdiplus::PointF p1, p2;
    if(!getIsBoundingBox()){
        Gdiplus::PointF p3(((float)(start.GetX() + end.GetX()) / 2 - 3.5*(end.GetX() - start.GetX())) * s, ((float)(start.GetY() + end.GetY()) / 2 - 3.5*(end.GetY() - start.GetY())) * s);
        Gdiplus::PointF p4(((float)(start.GetX() + end.GetX()) / 2 + 3.5*(end.GetX() - start.GetX())) * s, ((float)(start.GetY() + end.GetY()) / 2 + 3.5*(end.GetY() - start.GetY())) * s);
        p1 = p3;
        p2 = p4;
    }
    else{
        Gdiplus::RectF boundingBox;
        obj->setBoundingBox(boundingBox);
        float x1 = boundingBox.GetLeft() + (boundingBox.GetRight() - boundingBox.GetLeft()) * get_start().GetX()/100, y1 = boundingBox.GetTop() + (boundingBox.GetBottom() - boundingBox.GetTop()) * get_start().GetY()/100;
        float x2 = boundingBox.GetLeft() + (boundingBox.GetRight() - boundingBox.GetLeft()) * get_end().GetX()/100, y2 = boundingBox.GetTop() + (boundingBox.GetBottom() - boundingBox.GetTop()) * get_end().GetY()/100;
        Gdiplus::PointF p3( (x1 + x2) / 2 - 3.5*(x2 - x1), (y1 + y2) / 2 - 3.5*(y2 - y1));
        Gdiplus::PointF p4( (x1 + x2) / 2 + 3.5*(x2 - x1), (y1 + y2) / 2 + 3.5*(y2 - y1));
        p1 = p3;
        p2 = p4;
    }
    gb = new Gdiplus::LinearGradientBrush(p1, p2, colors[0], colors[get_amount()-1]);
    int stops_amount = get_amount();
    addStops(stops_amount, alpha, color_array, stop_array);
    gb->SetWrapMode(Gdiplus::WrapModeTileFlipXY);
    setTransform(gb,s);
    gb->SetInterpolationColors(color_array, stop_array, stops_amount);
}

void RadialGradient::setBrush(GraphicsPath& path, Gdiplus::PathGradientBrush*& rgb, float alpha, float s){
    Gdiplus::Color color_array[50];
    float stop_array[50];
    Gdiplus::PointF p1, p2;
    if(!getIsBoundingBox()){
        Gdiplus::PointF p3(start.GetX()*s, start.GetY()*s);
        Gdiplus::PointF p4(end.GetX()*s, end.GetY()*s);
        p1 = p3;
        p2 = p4;
    }
    else{
        Gdiplus::RectF boundingBox;
        path.GetBounds(&boundingBox);
        float x1 = boundingBox.GetLeft() + (boundingBox.GetRight() - boundingBox.GetLeft()) * get_start().GetX()/100, y1 = boundingBox.GetTop() + (boundingBox.GetBottom() - boundingBox.GetTop()) * get_start().GetY()/100;
        float x2 = boundingBox.GetLeft() + (boundingBox.GetRight() - boundingBox.GetLeft()) * get_end().GetX()/100, y2 = boundingBox.GetTop() + (boundingBox.GetBottom() - boundingBox.GetTop()) * get_end().GetY()/100;
        Gdiplus::PointF p3( (x1 + x2) / 2 - 3.5*(x2 - x1), (y1 + y2) / 2 - 3.5*(y2 - y1));
        Gdiplus::PointF p4( (x1 + x2) / 2 + 3.5*(x2 - x1), (y1 + y2) / 2 + 3.5*(y2 - y1));
        p1 = p3;
        p2 = p4;
    }
    rgb = new Gdiplus::PathGradientBrush(&path);
    int stops_amount = amount;
    addStops(stops_amount, alpha, color_array, stop_array);
    rgb->SetCenterColor(colors[0]); 
    rgb->SetSurroundColors(color_array, &stops_amount);
    rgb->SetWrapMode(Gdiplus::WrapModeTileFlipX);
    rgb->SetInterpolationColors(color_array, stop_array, stops_amount);
}

void RadialGradient::addStops(int& stops_amount, float alpha, Gdiplus::Color color_array[50], float stop_array[50]){
    for(int i = 0; i < stops_amount-1; i++){
        float opacity = alpha * ((float)colors[i].GetAlpha());
        Gdiplus::Color c(opacity, colors[i+1].GetRed(), colors[i+1].GetGreen(), colors[i+1].GetBlue());
        color_array[i] = c;
        stop_array[i] = stops[i];
    }
    stops_amount--;
}