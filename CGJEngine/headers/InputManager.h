//
// Created by joao on 11/15/17.
//

#ifndef CGJM_INPUTMANAGER_H
#define CGJM_INPUTMANAGER_H


#include <functional>
#include <unordered_map>
#include <map>

#define SPECIAL_KEYS_COUNT 4

class InputManager {
private:
    static InputManager* _ourInstance;

    unsigned int _updateInterval = 10;

    long _lastUpdateTime = 0;

    int _mouseX, _mouseY;
    bool _mouseDirty = true;
    bool _grabMouse = true;

    std::unordered_map<unsigned char, std::function<void(int dt)>> _keyCallbacks;//Call it until key is up
    std::unordered_map<int, std::function<void(int dt)>> _specialKeyCallbacks;

    std::unordered_map<unsigned char, std::function<void()>> _keyOnceCallbacks; //Only call it once per key down
    std::unordered_map<int, std::function<void()>> _specialKeyOnceCallbacks;

    std::map<unsigned char, std::function<void(int dt)>> _pendingKeyCallbacks;
    std::map<int, std::function<void(int dt)>> _pendingSpecialKeyCallbacks;

    std::function<void(int x, int y, int dt)> _mouseMovementCallback = nullptr;

    InputManager() = default;

public:
    static InputManager* getInstance();
    static long getTime();

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
    void grabMouse(bool grab);

    static void update();
};


#endif //CGJM_INPUTMANAGER_H
