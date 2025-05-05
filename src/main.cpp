#include "ClubManager.h"
#include "Event.h"
#include "CustomTime.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>

std::vector<std::string> readAllLines(std::ifstream& file) { // функция для считывания всех строк с файла
    std::vector<std::string> lines; // вектор
    std::string line; // строк
    while (std::getline(file, line)) { // получаем строку
        if (!line.empty()) {    // не пустая
            lines.push_back(line); // добавляем в вектор
        }
    }
    return lines;
}

int main(int argc, char* argv[]) { // используется считывание названия файла из командной строки
    if (argc != 2) { // если нету, то завершаем работу
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) { // проверка файла
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    try {
        std::vector<std::string> lines = readAllLines(inputFile); // считываем входные данные
        inputFile.close();

        if (lines.empty()) { // если их нет, то завершаем работу
            return 1;
        }

        // Проверка количества столов (первая строка)
        int totalTables;
        try {
            totalTables = std::stoi(lines[0]); // преобразоываем строку
            if (totalTables <= 0) { // проверка на положительное число
                std::cerr << lines[0] << std::endl; // ошибка с выводом строки
                return 1;
            }
        } catch (const std::exception&) {
            std::cerr << lines[0] << std::endl; //обработка ошибки
            return 1;
        }

        // Проверка времени работы (вторая строка)
        if (lines.size() < 2) { 
            return 1;
        }

        CustomTime openTime, closeTime; // создаются переменные
        try {
            std::istringstream timeIss(lines[1]); 
            std::string openStr, closeStr;
            if (!(timeIss >> openStr >> closeStr)) { // Пытаемся извлечь время открытия и закрытия из строки
                std::cerr << lines[1] << std::endl; // Если извлечь не удалось  — выводим ошибку
                return 1;
            }
            openTime = CustomTime::fromString(openStr); // преобразуем строки в объекты типа CustomTime
            closeTime = CustomTime::fromString(closeStr);
            if (closeTime<openTime || closeTime == openTime){ // проверка времени на корректность
                // время открытия не может быть больше времени закрытия, например открытие в 23:00, а закрытие в 01:00
                // поскольку входные данные никак не могут учитывать день события
                // например, клиент приходит в 23:01, а следующий в 00:02
                // это противоречит условию входных данных - "Все события идут последовательно во времени. (время события N+1) ≥ (время события N)."
                // даже если предположить, что первый клиент со временем, которое идет "не последовательно" - приходит на следующие сутки
                // то в таком случае невозможно обработать ситуацию, когда происходит ошибка входных данных
                // client1 - 14:00, client2 - 11:00, client3 - 15:00
                // поэтому "ночных смен" быть не может из за невозможности их определения наверняка
                std::cerr << lines[1] << std::endl;
                return 1;
            }
        } catch (const std::exception& e) { // вывод ошибки
            std::cerr << lines[1] << std::endl;
            //std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }

        // Проверка стоимости часа (третья строка)
        if (lines.size() < 3) { 
            return 1;
        }

        int hourlyRate;
        try {
            hourlyRate = std::stoi(lines[2]); // преобразование числа
            if (hourlyRate <= 0) { // проверка на неотрицательность 
                std::cerr << lines[2] << std::endl; // ошибка
                return 1;
            }
        } catch (const std::exception&) {
            std::cerr << lines[2] << std::endl;
            return 1;
        }

        // Проверка событий
        CustomTime prevTime(0, 0); // самое раннее время берется минимально возможным - 00:00
        for (size_t i = 3; i < lines.size(); ++i) {
            try {
                Event::parse(lines[i], totalTables, prevTime); // попытка парсить данные со строки в объект
                prevTime = CustomTime::fromString(lines[i].substr(0, 5)); // обновляется время предыдущего события
            } catch (const std::exception& e) {
                std::cerr << lines[i] << std::endl; // ошибка обработки строки
                return 1;
            }
        }

        // Если все проверки пройдены, обрабатываем события
        ClubManager club(totalTables, hourlyRate, openTime, closeTime); // создаем объект слуба
        
        prevTime = CustomTime(0, 0);
        for (size_t i = 3; i < lines.size(); ++i) {
            std::cout << lines[i] << std::endl;
            Event event = Event::parse(lines[i], totalTables, prevTime); 
            club.processEvent(event); // обрабатываем события каждой строки
            prevTime = event.getTime();
        }
        
        club.closeClub(); // клуб закрывается 
        club.printStats(); // выводится статистика
        
    } catch (const std::exception& e) {
        return 1;
    }

    return 0;
}