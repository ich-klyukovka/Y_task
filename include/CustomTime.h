#pragma once
#include <iostream>
#include <iomanip>
#include <regex>
#include <stdexcept>

class CustomTime {
private:
    int hours;   // часы
    int minutes; // минуты

public:
    CustomTime();
    CustomTime(int h, int m);

    int getHours() const;                   // получить часф
    int getMinutes() const;                 // получить минуты

    bool operator<(const CustomTime& other) const; // перегрузка оператора <
    bool operator==(const CustomTime& other) const; // перегрузка оператора ==

    int toMinutes() const;                  // перевод в минуты

    static bool isValidFormat(const std::string& time_str); // проверка на формат
    static CustomTime fromString(const std::string& time_str);    // перевод из строки 
    void print(std::ostream& os) const;                     // вывод
};