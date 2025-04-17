#include <iostream>
#include <fstream>
#include <string>

extern "C" {
    #include "pikchr.h"
}

int main() {
    std::string pikchr_code = R"(
    # Основные параметры
    scale = 1.0
    
    # Определяем основной блок группы
    Group: box width 2in height 0.7in "Учебная группа Б22-525" fill lightyellow
    
    # Староста
    arrow down from Group.s
    Monitor: box width 1.5in height 0.5in "Староста Иванов А.В." fill lightblue
    
    # Горизонтальная линия под старостой
    arrow down from Monitor.s
    Node: dot invisible
    
    # Центральная линия для разветвления
    line from Node left 1.5in
    line from Node right 1.5in
    
    # Подгруппа 1 - левая ветвь
    SG1_start: dot invisible at (Node.x - 1.5in, Node.y)
    arrow down from SG1_start
    Subgroup1: box width 1.2in height 0.5in "Подгруппа 1 Сидоров П.А." fill lightgreen
    
    # Подгруппа 2 - правая ветвь
    SG2_start: dot invisible at (Node.x + 1.5in, Node.y)
    arrow down from SG2_start
    Subgroup2: box width 1.2in height 0.5in "Подгруппа 2 Петров В.С." fill lightgreen
    
    # Студенты подгруппы 1
    arrow down from Subgroup1.s
    Student1_1: box width 1in height 0.4in "Алексеев И.К." fill lightcyan
    
    arrow down from Student1_1.s
    Student1_2: box width 1in height 0.4in "Борисов Д.М." fill lightcyan
    
    arrow down from Student1_2.s
    Student1_3: box width 1in height 0.4in "Волкова Е.А." fill lightcyan
    
    # Студенты подгруппы 2
    arrow down from Subgroup2.s
    Student2_1: box width 1in height 0.4in "Григорьев К.Л." fill lightcyan
    
    arrow down from Student2_1.s
    Student2_2: box width 1in height 0.4in "Дмитриев О.П." fill lightcyan
    
    arrow down from Student2_2.s
    Student2_3: box width 1in height 0.4in "Егорова С.В." fill lightcyan
    )";
    
    int width = 0;
    int height = 0;
    unsigned int flags = 0;
    
    char *svg = pikchr(pikchr_code.c_str(), nullptr, flags, &width, &height);
    if (svg) {
        std::ofstream outfile("cpp_generated.svg");
        outfile << svg;
        outfile.close();
        std::cout << "SVG успешно сгенерирован и сохранен в cpp_generated.svg" << std::endl;
        std::cout << "Ширина: " << width << ", Высота: " << height << std::endl;
        
        free(svg);
        return 0;
    } else {
        std::cerr << "Ошибка при генерации SVG" << std::endl;
        return 1;
    }
}
