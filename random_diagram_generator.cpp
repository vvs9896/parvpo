#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cstring>

extern "C" {
    #include "pikchr.h"
}

const std::vector<std::string> firstNames = {
    "Александр", "Алексей", "Анастасия", "Андрей", "Анна", "Артем", "Борис", "Вадим", 
    "Валентина", "Валерий", "Василий", "Виктор", "Виктория", "Владимир", "Галина", 
    "Геннадий", "Георгий", "Дарья", "Денис", "Дмитрий", "Евгений", "Евгения", "Егор", 
    "Екатерина", "Елена", "Иван", "Игорь", "Илья", "Ирина", "Кирилл", "Константин", 
    "Лариса", "Леонид", "Людмила", "Максим", "Марина", "Мария", "Михаил", "Надежда", 
    "Наталья", "Николай", "Олег", "Ольга", "Павел", "Петр", "Роман", "Светлана", 
    "Сергей", "Станислав", "Татьяна", "Федор", "Юлия", "Юрий", "Яна"
};

const std::vector<std::string> lastNames = {
    "Иванов", "Смирнов", "Кузнецов", "Попов", "Васильев", "Петров", "Соколов", 
    "Михайлов", "Новиков", "Федоров", "Морозов", "Волков", "Алексеев", "Лебедев", 
    "Семенов", "Егоров", "Павлов", "Козлов", "Степанов", "Николаев", "Орлов", 
    "Андреев", "Макаров", "Никитин", "Захаров", "Зайцев", "Соловьев", "Борисов", 
    "Яковлев", "Григорьев", "Романов", "Воробьев", "Сергеев", "Кузьмин", "Фролов", 
    "Александров", "Дмитриев", "Королев", "Гусев", "Киселев", "Ильин", "Максимов", 
    "Поляков", "Сорокин", "Виноградов", "Ковалев", "Белов", "Медведев", "Антонов", 
    "Тарасов", "Жуков", "Баранов", "Филиппов", "Комаров", "Давыдов", "Беляев", 
    "Герасимов", "Богданов", "Осипов", "Сидоров", "Матвеев", "Титов", "Марков", 
    "Миронов", "Крылов", "Куликов", "Карпов", "Власов", "Мельников", "Денисов", 
    "Гаврилов", "Тихонов", "Казаков", "Афанасьев", "Данилов", "Савельев", "Тимофеев", 
    "Фомин", "Чернов", "Абрамов", "Мартынов", "Ефимов", "Федотов", "Щербаков", 
    "Назаров", "Калинин", "Исаев", "Чернышев", "Быков", "Маслов", "Родионов", 
    "Коновалов", "Лазарев", "Воронин", "Климов", "Филатов", "Пономарев", "Голубев", 
    "Кудрявцев", "Прохоров", "Наумов", "Потапов", "Журавлев", "Овчинников", "Трофимов", 
    "Леонов", "Соболев", "Ермаков", "Колесников", "Гончаров", "Емельянов", "Никифоров", 
    "Грачев", "Котов", "Гришин", "Ефремов", "Архипов", "Громов"
};

// Класс для генерации случайных чисел
class RandomGenerator {
private:
    std::mt19937 gen;

public:
    RandomGenerator() : gen(std::random_device()()) {}

    int getRandomInt(int min, int max) {
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    std::string getRandomName() {
        return firstNames[getRandomInt(0, firstNames.size() - 1)];
    }

    std::string getRandomLastName() {
        std::string lastName = lastNames[getRandomInt(0, lastNames.size() - 1)];
        // Женский вариант фамилии для женских имен
        if (getRandomInt(0, 1) == 0) {
            char lastChar = lastName.back();
            if (lastChar == '\xD0' || lastChar == '\xD1') {
                lastName += "а";
            } else if (lastChar == '\xD0') {
                lastName.pop_back();
                lastName += "ая";
            } else if (lastChar == '\xD1') {
                lastName.pop_back();
                lastName += "я";
            }
        }
        return lastName;
    }
};

// Функция для генерации диаграммы группы
std::string generateGroupDiagram(RandomGenerator& rng) {
    std::ostringstream diagram;
    
    std::string groupName = "Б22-" + std::to_string(rng.getRandomInt(100, 999));
    
    diagram << "box width 2in height 0.7in \"Учебная группа " << groupName << "\" fill lightyellow\n";
    diagram << "arrow down from last box.s\n\n";
    
    std::string headName = rng.getRandomName() + " " + rng.getRandomLastName();
    diagram << "box width 1.5in height 0.5in \"Староста\\n" << headName << "\" fill lightblue\n";
    diagram << "arrow down from last box.s\n\n";
    
    diagram << "line down 0.5in from last arrow.end\n";
    diagram << "line right 1.5in from last line.end\n";
    diagram << "line left 1.5in from last line.start\n\n";
    
    int numSubgroups = rng.getRandomInt(2, 3);
    
    for (int i = 0; i < numSubgroups; i++) {
        float position = -1.5 + (3.0 / (numSubgroups - 1)) * i;
        std::string subgroupHead = rng.getRandomName() + " " + rng.getRandomLastName();
        
        diagram << "# Подгруппа " << (i + 1) << "\n";
        diagram << "arrow down from (last line.start.x + " << position << "in, last line.start.y)\n";
        diagram << "box width 1.2in height 0.5in \"Подгруппа " << (i + 1) << "\\n" 
               << subgroupHead << "\" fill lightgreen\n\n";
        
        int numStudents = rng.getRandomInt(3, 7);
        
        diagram << "# Студенты подгруппы " << (i + 1) << "\n";
        diagram << "move to last box.s\n";
        
        for (int j = 0; j < numStudents; j++) {
            std::string studentName = rng.getRandomName() + " " + rng.getRandomLastName();
            diagram << "arrow down 0.3in\n";
            diagram << "box width 1in height 0.4in \"" << studentName << "\" fill lightcyan\n";
        }
        
        diagram << "\n";
    }
    
    return diagram.str();
}

struct PerfMetrics {
    long long totalInputBytes;
    long long totalOutputBytes;
    long long totalProcessingTimeNs;
    int totalDiagrams;
};

void measurePerformance(int numberOfDiagrams, PerfMetrics& metrics) {
    RandomGenerator rng;
    
    metrics = {0, 0, 0, 0};
    
    for (int i = 0; i < numberOfDiagrams; i++) {
        std::string pikchrCode = generateGroupDiagram(rng);
        metrics.totalInputBytes += pikchrCode.length();
        
        auto start = std::chrono::high_resolution_clock::now();
        
        int width = 0;
        int height = 0;
        unsigned int flags = 0;
        char* svg = pikchr(pikchrCode.c_str(), nullptr, flags, &width, &height);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        
        metrics.totalProcessingTimeNs += duration.count();
        metrics.totalDiagrams++;
        
        if (svg) {
            metrics.totalOutputBytes += strlen(svg);
            
            if (i < 5) {
                std::string filename = "random_diagram_" + std::to_string(i) + ".svg";
                std::ofstream outfile(filename);
                outfile << svg;
                outfile.close();
                std::cout << "Сохранена диаграмма: " << filename << std::endl;
            }
            
            free(svg);
        } else {
            std::cerr << "Ошибка при генерации диаграммы #" << i << std::endl;
        }
        
        if (i % 10 == 0) {
            std::cout << "Обработано диаграмм: " << i << "/" << numberOfDiagrams
                      << " (" << (i * 100 / numberOfDiagrams) << "%)" << std::endl;
        }
    }
}

void printResults(const PerfMetrics& metrics) {
    std::cout << "\n=== РЕЗУЛЬТАТЫ ИЗМЕРЕНИЙ ===\n";
    std::cout << std::fixed << std::setprecision(2);
    
    // a. Соотношение объёма текстовых данных на входе к объёму диаграмм на выходе
    double ratio = (double)metrics.totalOutputBytes / metrics.totalInputBytes;
    std::cout << "a. Соотношение выход/вход: " << ratio << " к 1\n";
    std::cout << "   Объем входных данных: " << metrics.totalInputBytes / (1024.0 * 1024.0) << " МБ\n";
    std::cout << "   Объем выходных данных: " << metrics.totalOutputBytes / (1024.0 * 1024.0) << " МБ\n\n";
    
    // b. Среднее время обработки одного байта входных данных
    double avgTimePerByte = (double)metrics.totalProcessingTimeNs / metrics.totalInputBytes;
    std::cout << "b. Среднее время обработки одного байта: " << avgTimePerByte << " нс\n\n";
    
    // c. Среднее время генерации одной диаграммы
    double avgTimePerDiagram = (double)metrics.totalProcessingTimeNs / metrics.totalDiagrams / 1000.0;
    std::cout << "c. Среднее время генерации одной диаграммы: " << avgTimePerDiagram << " мкс\n\n";
    
    // Дополнительная статистика
    std::cout << "Дополнительная статистика:\n";
    std::cout << "Всего сгенерировано диаграмм: " << metrics.totalDiagrams << "\n";
    std::cout << "Средний размер входной диаграммы: " << metrics.totalInputBytes / metrics.totalDiagrams << " байт\n";
    std::cout << "Средний размер выходной SVG: " << metrics.totalOutputBytes / metrics.totalDiagrams << " байт\n";
    std::cout << "Общее время обработки: " << metrics.totalProcessingTimeNs / 1000000.0 << " мс\n";
}

int main() {
    std::cout << "Генератор случайных диаграмм pikchr\n";
    
    int estimatedDiagramsNeeded = 100 * 1024;
    
    PerfMetrics metrics;
    
    std::cout << "Генерация случайных диаграмм (примерно 100МБ входных данных)...\n";
    measurePerformance(estimatedDiagramsNeeded, metrics);
    
    printResults(metrics);
    
    return 0;
}