//
// Created by joao on 11/15/17.
//

#ifndef CGJM_INPUTMANAGER_H
#define CGJM_INPUTMANAGER_H


#include <functional>
#include <unordered_map>
#include <map>
#include <GL/freeglut.h>

#define SPECIAL_KEYS_COUNT 4

class InputManager {
private:
    static InputManager* ourInstance;

    unsigned int updateInterval = 10;
    int updateCallbackCounter = 0;

    long lastUpdateTime = 0;

    int mouseX, mouseY;
    std::unordered_map<unsigned char, std::function<void(int dt)>> keyCallbacks;//Call it until key is up
    std::unordered_map<int, std::function<void(int dt)>> specialKeyCallbacks;

    std::unordered_map<unsigned char, std::function<void()>> keyOnceCallbacks; //Only call it once per key down
    std::unordered_map<int, std::function<void()>> specialKeyOnceCallbacks;

    std::map<unsigned char, std::function<void(int dt)>> pendingKeyCallbacks;
    std::map<int, std::function<void(int dt)>> pendingSpecialKeyCallbacks;

    std::function<void(int x, int y, int dt)> mouseMovementCallback = nullptr;

    InputManager() = default;

public:
    static InputManager* getInstance();

    /*Interval at wich registered callbacks will be called if events occured*/
    void setActionInterval(unsigned int milliseconds);

    /*Assign actions with these functions*/
    void addKeyAction(unsigned char key, std::function<void(int dt)> callback);
    void addSpecialKeyAction(int key, std::function<void(int dt)> callback);

    void addKeyActionOnce(unsigned char key, std::function<void()> callback);
    void addSpecialKeyActionOnce(int key, std::function<void()> callback);

    void setMouseAction(std::function<void(int x, int y, int dt)> callback);

    /*Call these functions from GLUT callbacks*/
    void keyDown(unsigned char key);
    void keyUp(unsigned char key);
    void specialKeyDown(int key);
    void specialKeyUp(int key);
    void mouseMovement(int x, int y);

    static void update(int value);
};


#endif //CGJM_INPUTMANAGER_H
