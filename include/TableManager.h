#pragma once
#include "Event.h"
#include "TableStats.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>

class TableManager {
private:
    std::vector<Event> occupiedTables;   // вектор занятых столов
    std::unordered_set<int> freeTables;  // вектор свободных столов
    std::vector<TableStats> stats;       // вектор статистик
    int totalTables;                     // всего столов
    int hourlyRate;                      // почасовая стоимость

public:
    TableManager(int total, int rate);                      // конструктор
    void occupyTable(int tableNum, const Event& event);     // занимаем стол
    int getTotalTables() const;
    void freeTable(int tableNum, const CustomTime& currentTime);  // освобождаем
    bool isTableFree(int tableNum) const;                   // свободный ли стол
    int getFreeTableCount() const;                          // количество свободных столов
    int findFirstFree() const;                              // поиск первого свободного
    const Event& getTableInfo(int tableNum) const;          // инфо о столе
    std::vector<int> getOccupiedTables() const;             // занятые столы
    void printStats(const CustomTime& closeTime, std::ostream& os) const; // вывод статистики
};