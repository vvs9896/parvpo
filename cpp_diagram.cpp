#include <iostream>
#include <fstream>
#include <string>

// Подключим заголовочный файл pikchr
extern "C" {
    #include "pikchr.h"
}

int main() {
    // Текстовое описание диаграммы
    std::string pikchr_code = R"(
    box width 2in height 0.7in "Учебная группа Б22-525" fill lightyellow
    arrow down from last box.s

    # Староста
    box width 1.5in height 0.5in "Староста\nИванов А.В." fill lightblue
    arrow down from last box.s

    # Подгруппы
    line down 0.5in from last arrow.end
    line right 1.5in from last line.end
    line left 1.5in from last line.start

    # Подгруппа 1
    arrow down from (last line.start.x, last line.start.y)
    box width 1.2in height 0.5in "Подгруппа 1\nСидоров П.А." fill lightgreen

    # Подгруппа 2
    arrow down from (last line.end.x, last line.end.y)
    box width 1.2in height 0.5in "Подгруппа 2\nПетров В.С." fill lightgreen

    # Студенты подгруппы 1
    move to (last box.x - 1.5in, last box.y)
    arrow down
    box width 1in height 0.4in "Алексеев И.К." fill lightcyan
    arrow down from last box.s
    box width 1in height 0.4in "Борисов Д.М." fill lightcyan
    arrow down from last box.s
    box width 1in height 0.4in "Волкова Е.А." fill lightcyan

    # Студенты подгруппы 2
    move to (last box.x + 3in, last box.y + 1.2in)
    arrow down
    box width 1in height 0.4in "Григорьев К.Л." fill lightcyan
    arrow down from last box.s
    box width 1in height 0.4in "Дмитриев О.П." fill lightcyan
    arrow down from last box.s
    box width 1in height 0.4in "Егорова С.В." fill lightcyan
    )";

    // Вызов функции pikchr для генерации SVG
    int width = 0;
    int height = 0;
    unsigned int flags = 0;
    
    // Исправленный вызов с правильными типами аргументов
    char *svg = pikchr(pikchr_code.c_str(), nullptr, flags, &width, &height);

    if (svg) {
        // Запись SVG в файл
        std::ofstream outfile("cpp_generated.svg");
        outfile << svg;
        outfile.close();
        
        std::cout << "SVG успешно сгенерирован и сохранен в cpp_generated.svg" << std::endl;
        std::cout << "Ширина: " << width << ", Высота: " << height << std::endl;
        
        // Освобождение памяти
        free(svg);
        return 0;
    } else {
        std::cerr << "Ошибка при генерации SVG" << std::endl;
        return 1;
    }
}