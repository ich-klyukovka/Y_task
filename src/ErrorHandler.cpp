#include "ErrorHandler.h"

void ErrorHandler::printError(const CustomTime& time, const std::string& message, std::ostream& os) { // общая функция для вывода ошибок
    time.print(os); // вывод времени
    os << " 13 " << message << std::endl; // вывод сообщения 
}

void ErrorHandler::printEvent(const CustomTime& time, int id, const std::string& message, std::ostream& os) { // общая функция для вывода исходящих событий кроме ошибок
    time.print(os); // время
    os << " " << id << " " << message << std::endl; // вывод сообщения
}