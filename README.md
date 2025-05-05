# Инструкция по сборке и запуску проекта
## Сборка на Ubuntu
1. Обновите список пакетов:
   ```bash
   sudo apt update
   ```
2. Установите необходимые пакеты:
   ```bash
   sudo apt-get install libgtest-dev
   sudo apt-get install cmake
   ```
3. Соберите Google Test:
   ```bash
   cd /usr/src/gtest
   sudo cmake CMakeLists.txt
   sudo make
   ```
4. Создайте папку для сборки, находясь в папке с проектом:
   ```bash
   mkdir build
   cd build
   ```
5. Запустите CMake:
   ```bash
   cmake ..
   ```
6. Соберите проект:
   ```bash
   make
   ```
После успешной сборки в папке build будут созданы исполняемые файлы computer_club и computer_club_tests.
Поместите файл с входными данными в папку build
Запустите программу:
   ```bash
   ./computer_club <filename>
   ```
Например:
   ```bash
   ./computer_club input.txt
   ```

Запуск тестов (Ubuntu)
   ```bash
   ./computer_club_tests
   ```




## Сборка на Windows
Установка MinGW-w64
Скачайте MinGW-w64 с официального сайта (https://winlibs.com/)
Распакуйте архив в удобное место
Добавьте путь к MinGW в PATH:
1. Нажмите Win + R, введите sysdm.cpl
2. Перейдите на вкладку "Дополнительно" → "Переменные среды"
3. В разделе "Системные переменные" найдите Path, нажмите "Изменить"
4. Добавьте новый путь к папке bin (например, C:\mingw64\bin)

Установка CMake
Скачайте CMake с официального сайта (https://cmake.org/download/)

Установка Google Test
Откройте командную строку (cmd) или PowerShell
Выполните следующие команды:
   ```bash
   git clone https://github.com/google/googletest.git
   cd googletest
   mkdir build
   cd build
   cmake -G "MinGW Makefiles" ..
   cmake --build .
   ```
После сборки GTest в папке build выполните команду установки:
   ```bash
   cmake --install . --prefix "C:\gtest"
   ```
Необходимо добавить путь GTest в PATH
Откройте Параметры Windows → Система → Дополнительные параметры системы → Переменные среды.
В системных переменных найдите Path и добавьте путь к C:\gtest\bin.
Создайте новую переменную:
Имя: GTEST_ROOT
Значение: C:\gtest

1. Создайте папку для сборки (в папке с проектом):
   ```bash
   mkdir build
   cd build
   ```
2. Запустите CMake:
   ```bash
   cmake -G "MinGW Makefiles" ..
   ```
3. Соберите проект:
   ```bash
   mingw32-make
   ```

После успешной сборки в папке build будут созданы исполняемые файлы computer_club.exe и computer_club_tests.exe.
Поместите файл с входными данными в папку build
Запустите программу:
   ```bash
   computer_club.exe <filename>
   ```
Например:
   ```bash
   computer_club.exe input.txt
   ```

Запуск тестов
   ```bash
   computer_club_tests.exe
   ```
