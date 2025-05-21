#include <iostream>
#include <memory>
#include "../SatelliteSelect.h"
#include "../CalcMethodLib/Sgp4CalcMethod.h"
#include "../ReaderLib/FileTxtReader.h"

int main() {
    // 1. Создание объекта станции
    // Параметры: широта (градусы), долгота (градусы), высота (км),
    // минимальный/максимальный азимут (рад), минимальный/максимальный угол места (рад), минимальное время наблюдения (сек)
    StationClass station(
        56.14717,
        37.76275,
        100,
        120.0,
        200.0,
        10.0,
        90.0,
        20
        );

    // 2. Создание объекта SatelliteSelect
    SatelliteSelect selector(station);

    // 3. Создание и установка модели чтения данных
    auto reader = std::make_unique<FileTxtReader>("C:\\Qt\\Projects\\SatelliteSelect\\SatelliteSelect\\TLE.txt");
    if (!selector.SetReadModel(std::move(reader))) {
        std::cerr << "Failed to set reader model!" << std::endl;
        return 1;
    }

    // 4. Создание и установка метода расчёта
    auto calcMethod = std::make_unique<Sgp4CalcMethod>();
    if (!selector.SetCalcMethod(std::move(calcMethod))) {
        std::cerr << "Failed to set calculation method!" << std::endl;
        return 1;
    }

    // 5. Подготовка данных (фильтрация спутников по скорости)
    selector.DataPrepare();

    // 6. Выполнение расчётов (поиск спутников в зоне видимости)
    selector.Solve();

    // 7. Вывод результатов
    std::cout << "Satellites in view:\n";
    selector.ShowResult();

    for(std::size_t i = 100000000; i > 0; i--)
        int a = 0;
    selector.Solve();
    std::cout << "Satellites in view:\n";
    selector.ShowResult();


    return 0;
}
