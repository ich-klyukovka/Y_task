#include "ClubManager.h"

ClubManager::ClubManager(int totalTables, int rate, const CustomTime& open, const CustomTime& close) :
    tables(totalTables, rate), openTime(open), closeTime(close), hourlyRate(rate) { // конструктор класса
        openTime.print(std::cout); // Вывод времени открытия
        std::cout << std::endl;
    }

void ClubManager::processEvent(const Event& event) { // разветвление по ID
    switch (event.getId()) {
        case 1: handleClientArrival(event); break; // Клиент пришел
        case 2: handleClientSit(event); break;     // Клиент сел за стол 
        case 3: handleClientWait(event); break;    // Клиент ожидает
        case 4: handleClientLeave(event); break;   // Клиент ушел
    }
}

void ClubManager::closeClub() { // формируется событие - закрытие клуба
    auto allClients = clients.getAllClients(); // собираются все оставшиеся клиенты
    for (const std::string& client : allClients) {
        forceLeave(client, closeTime, "Club closed"); // удаляются
    }
}

void ClubManager::printStats() const { // функция вывода состояния столов
    closeTime.print(std::cout); // использовалась при отладке
    std::cout << std::endl;
    tables.printStats(closeTime, std::cout);
}

void ClubManager::handleClientArrival(const Event& event) { // событие - Клиент пришел
    if (!(openTime < event.getTime() && event.getTime() < closeTime)) { // если время клиента не попадает в диапазон работы
        ErrorHandler::printError(event.getTime(), "NotOpenYet"); // выводится ошибка и событие не обрабатывается
        return; // пропуск действия
    }
    if (clients.hasClient(event.getClient())) { // если такой клиент уже есть
        ErrorHandler::printError(event.getTime(), "YouShallNotPass"); // выводится ошибка 
        return; // событие не обрабатывается
    }
    clients.addClient(event); // если ошибок не случилось, то добавляем клиента в список
}

void ClubManager::handleClientSit(const Event& event) { // событие - Клиент сел за стол
    if (!clients.hasClient(event.getClient())) { // если клиента нет в списках
        ErrorHandler::printError(event.getTime(), "ClientUnknown"); // выводится ошибка, тк некого садить =)
        return; // пропуск действия
    }

    if (tables.isTableFree(event.getTable())) { // если стол свободен
        tables.occupyTable(event.getTable(), event); // обновляем информацию о столах (сажаем клиента)
        if (clients.getClientEvent(event.getClient()).getTable() != 0) { // проверяется - сидел ли уже клиент за каким либо столом
            tables.freeTable(clients.getClientEvent(event.getClient()).getTable(), event.getTime()); // если сидел, то он освобождается
        }
        clients.addClient(event); // обновляется список клиентов (указывается, что клиент сел за стол)
    } else {
        ErrorHandler::printError(event.getTime(), "PlaceIsBusy"); // если стол все же был занят, то возвращается ошибка
    }
}

void ClubManager::handleClientWait(const Event& event) { // событие - клиент встал в очередь
    if (!clients.hasClient(event.getClient())) { // если клиент не находится в клубе
        ErrorHandler::printError(event.getTime(), "ClientUnknown"); // обработка ошибки
        return; // пропуск действия
    }

    if (clients.getClientEvent(event.getClient()).getTable() != 0) { // если клиент уже сидит за столом, то вставать в очередь нельзя
        ErrorHandler::printError(event.getTime(), "ClientAlreadySeated"); // генерируется ошибка
        return;
    }
    
    if (tables.getFreeTableCount() > 0) { // если имеются пустые столы, то генерируется ошибка
        ErrorHandler::printError(event.getTime(), "ICanWaitNoLonger!");
        return;
    }
    
    //std::cout<<clients.queueSize() << "\n";
    if (clients.queueSize()+1 > tables.getTotalTables()) { // если очередь слишком большая,
        ErrorHandler::printEvent(event.getTime(), 11, event.getClient()); // обработка исходящего события ID-11
        ClubManager::handleClientLeave(event); // клиент уходит
        return;
    }
    
    clients.addToQueue(event.getClient()); // если ошибок нет, то клиент добавляется в очередь
}

void ClubManager::handleClientLeave(const Event& event) { // событие - Клиент уходит
    if (!clients.hasClient(event.getClient())) { // если клиента и так нет в зале, генерируется ошибка
        ErrorHandler::printError(event.getTime(), "ClientUnknown"); 
        return;
    }

    int freedTable = clients.getClientEvent(event.getClient()).getTable(); // получаем освободившийся стол
    clients.removeClient(event.getClient()); // клиент удаляется из списков

    if (freedTable > 0) { // если у клиента был стол
        tables.freeTable(freedTable, event.getTime()); // статус стола обновляется на свободный
    }

    if (!clients.isQueueEmpty() && freedTable > 0) { // если очередь не пустая
        std::string nextClient = clients.getNextInQueue(); // берется первый из очереди
        ErrorHandler::printEvent(event.getTime(), 12, nextClient + " " + std::to_string(freedTable)); // обработка исходящего события ID-12
        
        Event sitEvent(event.getTime(), 2, nextClient, freedTable); 
        handleClientSit(sitEvent); // клиент садится за освободившийся стол
    }
}

void ClubManager::forceLeave(const std::string& clientName, const CustomTime& currentTime, const std::string& reason) { // событие - выгнать клиента
    ErrorHandler::printEvent(currentTime, 11, clientName); // вывод информации о событии
    
    if (clients.hasClient(clientName)) { // если такой клиент есть в списках
        int tableNum = clients.getClientEvent(clientName).getTable(); // берем номер стола
        if (tableNum > 0) { // если стол есть
            tables.freeTable(tableNum, currentTime); // освобождаем
        }
        clients.removeClient(clientName); // удаляем клиента из списков (выгоняем)
    }
}
