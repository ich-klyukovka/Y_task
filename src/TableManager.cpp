#include "TableManager.h"

TableManager::TableManager(int total, int rate) : // конструктор класса менеджера столов
    totalTables(total), hourlyRate(rate) {
    occupiedTables.resize(total + 1);
    stats.resize(total + 1);
    for (int i = 1; i <= total; ++i) {
        freeTables.insert(i);
    }
}

void TableManager::occupyTable(int tableNum, const Event& event) { // метод для занятия стола
    occupiedTables[tableNum] = event; // добавляем стол в список занятых
    freeTables.erase(tableNum); // удаляем его из списка свободных
}

void TableManager::freeTable(int tableNum, const CustomTime& currentTime) { // метод освобождения стола
    if (!isTableFree(tableNum)) {  // если стол занят
        const Event& startEvent = occupiedTables[tableNum]; // событие когда он был занят
        CustomTime startTime = startEvent.getTime(); // получаем время отсчета
        int minutes = currentTime.toMinutes() - startTime.toMinutes(); // вычисляется продолжительность использования
        stats[tableNum].addUsage(minutes, hourlyRate); // добавляем в статтистику использования
    }
    
    occupiedTables[tableNum] = Event(); // освобождаем
    freeTables.insert(tableNum); // добавляем стол в список свободных
}

bool TableManager::isTableFree(int tableNum) const { // свободный ли стол
    return freeTables.count(tableNum) > 0; // если их количество > 0
}

int TableManager::getFreeTableCount() const { // количество свободных столов
    return freeTables.size();
}

int TableManager::getTotalTables() const{ // общее количество столов
    return totalTables;
}

int TableManager::findFirstFree() const { // поиск первого свободного стола
    return freeTables.empty() ? -1 : *freeTables.begin();
}

const Event& TableManager::getTableInfo(int tableNum) const { // дает информацию о конкретном столе
    return occupiedTables[tableNum];
}

std::vector<int> TableManager::getOccupiedTables() const {  // получить занятые столы
    std::vector<int> occupied; // ветор
    for (int i = 1; i <= totalTables; ++i) {
        if (!isTableFree(i)) { // если не свободный
            occupied.push_back(i); // добавляем
        }
    }
    return occupied; 
}

void TableManager::printStats(const CustomTime& closeTime, std::ostream& os) const { // вывод информации о столах (ииспользовалось при отладке)
    for (int i = 1; i <= totalTables; ++i) {
        os << i;
        
        if (!isTableFree(i)) {
            const Event& startEvent = occupiedTables[i];
            CustomTime startTime = startEvent.getTime();
            int minutes = closeTime.toMinutes() - startTime.toMinutes();
            
            TableStats temp = stats[i];
            temp.addUsage(minutes, hourlyRate);
            temp.print(os);
        } else {
            stats[i].print(os);
        }
        os << std::endl;
    }
}