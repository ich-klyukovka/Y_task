#include "ClientManager.h"

bool ClientManager::hasClient(const std::string& name) const {
    return clients.count(name) > 0; // проверка на наличие клиента
}

void ClientManager::addClient(const Event& event) {
    clients[event.getClient()] = event; // добавить клиента
}

void ClientManager::removeClient(const std::string& name) {
    clients.erase(name); // удалить
}

void ClientManager::addToQueue(const std::string& name) {
    waitingQueue.push(name); // добавить в очередь
}

std::string ClientManager::getNextInQueue() {
    if (waitingQueue.empty()) return ""; // если очередь пустая, то возвращаем ничего
    std::string client = waitingQueue.front(); // иначе сохраняется первый элемент
    waitingQueue.pop(); // удаляется из очереди
    return client; // возврат
}

bool ClientManager::isQueueEmpty() const { // проверка на пустую очередь
    return waitingQueue.empty();
}

size_t ClientManager::queueSize() const { // возврат размера очереди
    return waitingQueue.size();
}

Event ClientManager::getClientEvent(const std::string& name) const { // получить по имени
    return clients.at(name);
}

std::vector<std::string> ClientManager::getAllClients() const { // вернуть список всех клиентов
    std::vector<std::string> clientList; // создаем пустой список
    for (const auto& [name, _] : clients) { 
        clientList.push_back(name); // проходимся по каждому и добавляем
    }
    std::sort(clientList.begin(), clientList.end()); // стандартная сортировка
    return clientList; // возврат в алфавитном порядке
}