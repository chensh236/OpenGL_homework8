#pragma once
#include <iostream>
class Mouse
{
public:
    Mouse(){
        pos_x = 0.0;
        pos_y = 0.0;
        isPress = false;
        left = true;
    }
    ~Mouse(){}
    static Mouse* getInstance() {
        if (instance == NULL) {
            instance = new Mouse();
        }
        return instance;
    }
    double pos_x, pos_y;
    bool isPress, left;

private:
    
    static Mouse* instance;
};