#pragma once
#include "CustomTime.h"
#include <string>
#include <regex>
#include <stdexcept>

class Event {
private:
    CustomTime time;          // время события
    int id;             // идентификатор события
    std::string client; // имя клиента
    int table;          // номер стола (по умолчанию 0)

public:
    Event();
    Event(CustomTime t, int i, std::string c, int tbl = 0);

    const CustomTime& getTime() const;            // получаем время
    int getId() const;                      // получаем ID
    const std::string& getClient() const;   // получаем имя
    int getTable() const;                   // получаем номер таблицы

    static bool isValidClientName(const std::string& name); // проверка валидности имени
    static bool isValidTableEventBody(const std::string& name); // проверка валидности тела события
    static Event parse(const std::string& line, int total_tables, const CustomTime& prev_time); // парсер входных данных
};