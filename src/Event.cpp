#include "Event.h"
#include <sstream>

Event::Event() : id(0), table(0) {} // конструктор класса
Event::Event(CustomTime t, int i, std::string c, int tbl) : time(t), id(i), client(c), table(tbl) {}

const CustomTime& Event::getTime() const { return time; }                 //функция получения времени
int Event::getId() const { return id; }                             // для ID
const std::string& Event::getClient() const { return client; }      // имя
int Event::getTable() const { return table; }                       // стол клиента

bool Event::isValidClientName(const std::string& name) {  // меотд для проверки корректного имени 
    std::regex name_regex(R"(^[a-z0-9_-]+$)"); // используется регулярное выражение для символов из алфавита a..z, 0..9, _, -
    return std::regex_match(name, name_regex); 
}

bool Event::isValidTableEventBody(const std::string& name) { // проверка корректного тела события если в него входит 
    std::regex name_regex(R"(^[a-z0-9_-]+\s[0-9]+$)"); // не только имя
    return std::regex_match(name, name_regex);
}

Event Event::parse(const std::string& line, int total_tables, const CustomTime& prev_time) { // метод парсер данных из файла
    std::istringstream iss(line);
    Event event;
    std::string time_str; // объявляются начальные переменные
    
    iss >> time_str; // считывается время
    event.time = CustomTime::fromString(time_str); // преобразовывается в нужный формат
    
    if (!(prev_time < event.time)) { // проверка на то, что текущее событие по времени после предыдущего
        if(!(prev_time.getHours() == 0 && prev_time.getMinutes() == 0)){
            throw std::runtime_error(""); // обработка ошибки
        }
    }
    
    iss >> event.id; // считывание ID
    
    if (event.id < 1 || event.id > 4) { // проверка на валидность
        throw std::runtime_error("");
    }

    if (event.id == 2) { // если id = 2, то в теле события находится номер стола
        std::string line;
        getline(iss >> std::ws, line); // считывается информация
        if (!isValidTableEventBody(line)) { // проверка на валидность с учетом номера стола
            throw std::runtime_error("");
        }
        size_t space_pos = line.find(' '); // поиск индекса символа разделения имени и стола
        if (space_pos != std::string::npos) {
            event.client = line.substr(0, space_pos); // запись имени
            event.table = stoi(line.substr(space_pos + 1)); // запись номера стола
            if (event.table < 1 || event.table > total_tables) {
                throw std::runtime_error("");
            }
        }
    } else { // в других случаях тело события содержит только имя клиента
        iss >> event.client; // считываем
        if (!isValidClientName(event.client)) { // проверяем
            throw std::runtime_error("");
        }    
    }

    return event; // возвращаем объект
}