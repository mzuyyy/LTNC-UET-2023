//
// Created by MZuy on 2/5/2023.
//
#pragma once
#ifndef BTL__LOG_H
#define BTL__LOG_H

#include <bits/stdc++.h>

class Log{
    private:
        std::string location = "";
    public:
        Log(std::string area){
            Log::location = area;
        }
        void updateStatus(const std::string& message){
           std::cout << location << " : " << message << std::endl;
        }
};
#endif //BTL__LOG_H
