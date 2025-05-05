#include "CustomTime.h"

CustomTime::CustomTime() : hours(0), minutes(0) {} // конструктор класса
CustomTime::CustomTime(int h, int m) : hours(h), minutes(m) {}

int CustomTime::getHours() const { return hours; } // получить часы
int CustomTime::getMinutes() const { return minutes; } // минуты

bool CustomTime::operator<(const CustomTime& other) const { // перегрузка оператора сравнения
    // считаем, что время меньше, если (часы меньше) или (часы равны, а минуты меньше)
    return (hours < other.hours) || (hours == other.hours && minutes < other.minutes);
}

bool CustomTime::operator == (const CustomTime& other) const { // перегрузка оператора равенства
    return (hours == other.hours) && (minutes == other.minutes); // если равны часы и минуты
}

int CustomTime::toMinutes() const { // перевод в минуты
    return hours * 60 + minutes;
}

bool CustomTime::isValidFormat(const std::string& time_str) { // метод проверки времени на валидность
    std::regex time_regex(R"(^([01][0-9]|2[0-3]):([0-5][0-9])$)"); // формат XX:XX
    return std::regex_match(time_str, time_regex);
}

CustomTime CustomTime::fromString(const std::string& time_str) { // перевод из строки
    if (!isValidFormat(time_str)) { // если формат верный
        throw std::runtime_error("");
    }
    size_t colon_pos = time_str.find(':');
    return CustomTime(stoi(time_str.substr(0, colon_pos)), stoi(time_str.substr(colon_pos + 1))); // преобразовываем
}

void CustomTime::print(std::ostream& os) const { // вывод времени согласно требованиям
    os << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes << std::setfill(' ');
}