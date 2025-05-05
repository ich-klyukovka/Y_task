#pragma once
#include "CustomTime.h"
#include "TableManager.h"
#include "ClientManager.h"
#include "ErrorHandler.h"
#include <iostream>

class ClubManager {
private:
    CustomTime openTime; // время открытия
    CustomTime closeTime; // время закрытия
    TableManager tables; // менеджер столов
    ClientManager clients; // менеджер клиентов
    int hourlyRate; // стоимость часа

    void handleClientArrival(const Event& event); // клиент приходит в клуб
    void handleClientSit(const Event& event);     // клиент садится за стол
    void handleClientWait(const Event& event);    // клиент встает в очередь
    void handleClientLeave(const Event& event);   // клиент покидает клуб
    void forceLeave(const std::string& clientName, const CustomTime& currentTime, const std::string& reason); // клиент принудительно покидает клуб

public:
    ClubManager(int totalTables, int rate, const CustomTime& open, const CustomTime& close); // конструктор
    void processEvent(const Event& event); // обработка событий
    void closeClub(); // закрытие клуба
    void printStats() const; // вывод статистики
};