Для запуска необходимо зайти в папку cmake для использования CMakeLists (cd cmake)

Чтобы поменять тип (float или double) необходимо в файле main.cpp закомментировать один из define (#define TYPE float и #define TYPE double, соответственно для float  необходимо, чтобы была раскомментированна строка #define TYPE float)

Для запуска программы с cmake:
    
    mkdir tmp
    
    cd tmp
    
    cmake ..
    
    make
    
    ./main

Вывод данных:

    float: 0.0782718
    double: 4.89582e-11

Вывод: так как double дает определяет точность числа в виде 16 десятичных знаков, а float в виде 7 знаков, double точнее и как следствие сумма гораздо ближе к нулю
