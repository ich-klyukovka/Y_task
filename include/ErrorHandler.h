#pragma once
#include "CustomTime.h"
#include <iostream>

class ErrorHandler {
public:
    static void printError(const CustomTime& time, const std::string& message, std::ostream& os = std::cout); // вывод ошибок
    static void printEvent(const CustomTime& time, int id, const std::string& message, std::ostream& os = std::cout); // вывод исходящих
};