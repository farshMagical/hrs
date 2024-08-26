# HRS  
ПО для управления спектрометром высокого разрешения для установки Т15МД. По установленным значениям дисперсии и длины волны устанавливает прибор в рабочее положение. Представляет клиент серверный комплекс программ.

## Функции HRS  
* Включение/выключение прибора  
* Установка значения дисперсии
* Установка значения длины волны
* Отображение состояния (Connect or not), текущего положения, статуса (Waiting command or Busy) зеркал (по сути - шагового двигателя), состояния концевиков зеркал
* Отображение состояния (Connect or not), текущего положения, статуса (Waiting command or Busy) решеток (по сути - шагового двигателя), состояния концевиков решеток 
* Установка положения зеркал
* Установка положения решеток

# Сервер  
Сервер - демон, который можно `start`, `stop` и `verbose`. Работает по протоколу websocket. ~~IP адрес, порт и url пути можно настроить.~~
* ws://localhost:3000/monitor - по этому адресу клиент получает статус работы прибора
* ws://localhost:3000/control - клиент отправляет на этот адрес управляющие команды

### Формат пакетов

Для передачи данных используется `json`, библиотека [nlohmann/json](https://github.com/nlohmann/json?tab=readme-ov-file#cmake).

# Клиент
В простейшем случае, клиент представляется собой консольное приложение, которое умеет обращаться по http и ws к серверу, передавая команды.

### Переводы единиц
### Константы шаговых двигателей

# Сборка
* Установка [vcpkg](https://learn.microsoft.com/ru-ru/vcpkg/get_started/get-started?pivots=shell-cmd), [cmake](https://cmake.org/download/), [msvc](https://visualstudio.microsoft.com/ru/downloads/), [llvm](https://github.com/llvm/llvm-project/releases/tag/llvmorg-18.1.8)
* Установка зависимостей с помощью команды `vcpkg install` в дериктории проекта
* mkdir build && cd build && cmake . && cmake --build .

## Установка
`cmake install`  
Проверка корректного нахождения библиотеки NiFPGA.dll

## Логирование

#### Ссылки
* [Руководство по оформлению Markdown файлов](https://gist.github.com/Jekins/2bf2d0638163f1294637)
* [NI FPGA guide](https://www.ni.com/docs/en-US/bundle/fpga-interface-c-api-ref/page/capi/fpgac.html) for c/c++ applications
* [Advanced server example](https://www.boost.org/doc/libs/1_85_0/libs/beast/example/advanced/server/advanced_server.cpp)