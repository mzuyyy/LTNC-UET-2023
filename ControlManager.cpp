//
// Created by MZuy on 4/5/2023.
//

#include "ControlManager.h"

ControlManager::ControlManager() {
    consoleControlManager = new Log("ControlManager");
    consoleControlManager->updateStatus("ControlManager created!");
    setControlType(ARROW);
}

ControlManager::~ControlManager() {
    delete consoleControlManager;
    consoleControlManager = nullptr;
}
void ControlManager::setControlType(int type) {
    for (int i = 0; i < CONTROL_DIRECTION_TOTAL; i++) {
        keyCode[i] = CONTROL_TYPE[type][i];
    }
}
