# HRS  
ПО для управления спектрометром высокого разрешения для установки Т15МД. По установленным значениям дисперсии и длины волны устанавливает прибор в рабочее положение. Представляет клиент серверный комплекс программ. Сервер принимает http и ws запросы по установленному в файле конфигурации IP и порту.

## Функции HRS  
* Включение/выключение прибора  
* Установка значения дисперсии
* Установка значения длины волны
* Отображение состояния (Connect or not), текущего положения, статуса (Waiting command or Busy) зеркал (по сути - шагового двигателя), состояния концевиков зеркал
* Отображение состояния (Connect or not), текущего положения, статуса (Waiting command or Busy) решеток (по сути - шагового двигателя), состояния концевиков решеток 
* Установка положения зеркал
* Установка положения решеток

# Сервер  
Сервер - демон, который можно `start` и `stop`. Слушает определенный порт.  

## Запросы серверу
* POST для установки значений
* ws для контроля за текущими значениями переменных

# Клиент
В простейшем случае, клиент представляется собой консольное приложение, которое умеет обращаться по http и ws к серверу, передавая команды.

### Переводы единиц
### Константы шаговых двигателей

# Сборка
* Установка [vcpkg](https://learn.microsoft.com/ru-ru/vcpkg/get_started/get-started?pivots=shell-cmd), [cmake](https://cmake.org/download/), [msvc](https://visualstudio.microsoft.com/ru/downloads/), [llvm](https://github.com/llvm/llvm-project/releases/tag/llvmorg-18.1.8)
* Установка зависимостей с помощью команды `vcpkg install` в дериктории проекта
* mkdir build && cd build && cmake . && cmake --build .

## Установка

#### Ссылки
* [Руководство по оформлению Markdown файлов](https://gist.github.com/Jekins/2bf2d0638163f1294637)