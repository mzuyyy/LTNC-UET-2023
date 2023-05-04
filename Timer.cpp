//
// Created by MZuy on 3/30/2023.
//
#include <SDL.h>
#include "Timer.h"

Timer::Timer() {
    consoleTimer = new Log("Timer");

    startTicks = 0;
    pausedTicks = 0;

    paused = false;
    started = false;
}
void Timer::init(){
    startTicks = 0;
    pausedTicks = 0;

    paused = false;
    started = false;

    start();

    consoleTimer->updateStatus("Timer init!");
}
void Timer::start() {
    started = true;
    paused = false;

    startTicks = SDL_GetTicks();
    pausedTicks = 0;

    consoleTimer->updateStatus("Timer started!");
}

void Timer::stop() {
    started = false;
    paused = false;

    startTicks = 0;
    pausedTicks = 0;

    consoleTimer->updateStatus("Timer stopped!");
}

void Timer::pause() {
    if (started && !paused) {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
        consoleTimer->updateStatus("Timer paused!");
    }
}

void Timer::unpause() {
if (started && paused) {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
        consoleTimer->updateStatus("Timer unpause!");
    }
}

int Timer::getTicks() const {
    if (started) {
        if (paused) {
            return pausedTicks;
        } else {
            return SDL_GetTicks() - startTicks;
        }
    }
    return 0;
}

bool Timer::isStarted() const {
    return started;
}

bool Timer::isPaused() const {
    return paused && started;
}

Timer::~Timer(){
    delete consoleTimer;
    consoleTimer = nullptr;
}
