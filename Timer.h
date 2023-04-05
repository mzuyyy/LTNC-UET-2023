//
// Created by MZuy on 3/30/2023.
//

#ifndef BTL_TIMER_H
#define BTL_TIMER_H
#include "logStatus.h"

class Timer {
private:
    Log* consoleTimer = nullptr;
    Uint32 startTicks;
    Uint32 pausedTicks;
    bool paused;
    bool started;
public:
    Timer(){
        consoleTimer = new Log("Timer");
        startTicks = 0;
        pausedTicks = 0;
        paused = false;
        started = false;
     };
    ~Timer(){
        delete consoleTimer;
        consoleTimer = nullptr;
    };
    void start();
    void stop();
    void pause();
    void unpause();
    int getTicks() const;
    bool isStarted() const;
    bool isPaused() const;
};


#endif //BTL_TIMER_H
