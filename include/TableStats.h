#pragma once
#include <iostream>
#include <iomanip>

class TableStats {
private:
    int revenue;            // доход
    int occupiedMinutes;    // время использования

public:
    TableStats();                                   // конструктор
    void addUsage(int minutes, int hourlyRate);     // добавить пользование
    int getRevenue() const;                         // расчет дохода
    int getOccupiedMinutes() const;                 // сколько времени
    void print(std::ostream& os) const;             // вывод
};