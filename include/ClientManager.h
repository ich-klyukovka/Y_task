#pragma once
#include "Event.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
#include <algorithm>

class ClientManager {
private:
    std::unordered_map<std::string, Event> clients; // словарь/хеш таблица слиентов
    std::queue<std::string> waitingQueue; // очередь

public:
    bool hasClient(const std::string& name) const; // есть ли клиент
    void addClient(const Event& event);            // добавить клиента 
    void removeClient(const std::string& name);    // удалить клиента
    void addToQueue(const std::string& name);      // добавить в очередь
    std::string getNextInQueue();                  // получить следующего в очереди
    bool isQueueEmpty() const;                     // пуста ли очередь
    size_t queueSize() const;                      // размер очереди
    Event getClientEvent(const std::string& name) const; // получить клиента
    std::vector<std::string> getAllClients() const; // всех клиентов, отсортированных по алфавиту
};