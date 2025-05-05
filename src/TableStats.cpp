#include "TableStats.h"

TableStats::TableStats() : revenue(0), occupiedMinutes(0) {} // конструктор

void TableStats::addUsage(int minutes, int hourlyRate) { // добавляем статистику использования
    int hours = minutes / 60;
    if (minutes % 60 != 0) hours++; // подсчет дохода и времени
    revenue += hours * hourlyRate;
    occupiedMinutes += minutes;
}

int TableStats::getRevenue() const { return revenue; } // доход
int TableStats::getOccupiedMinutes() const { return occupiedMinutes; } //сколько минут использован

void TableStats::print(std::ostream& os) const { // вывод согласно требованиям
    int h = occupiedMinutes / 60;
    int m = occupiedMinutes % 60;
    os << " " << revenue << " " 
       << std::setfill('0') << std::setw(2) << h << ":" << std::setw(2) << m << std::setfill(' ');
}