#ifndef PATH_H
#define PATH_H

#include "Objects.h"

namespace Shapes{
    class Command{
    private:
        char cmd;
        vector<float> nums;
    public:
        Command();

        char getCmd();
        void setCmd(char c);

        vector<float> getNums();
        void addNum(float p);

        string toString();
    };

    class Path:public Object{
    private:
        vector<Command> cmd;
        GraphicsPath path;
    public:
        Path();

        vector<Command> getCmd();
        Command getCmdAt(int idx);

        void add(Command c);
        void setPath(Gdiplus::GraphicsPath* p2);
        Gdiplus::GraphicsPath& getPath();
        void setBoundingBox(Gdiplus::RectF& box) override;
    };
}

#endif
