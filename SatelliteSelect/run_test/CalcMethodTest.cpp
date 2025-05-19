// #include <iostream>
// #include <memory>
// #include <string>

// #include "CalcMethodLib/Sgp4CalcMethod.h"
// #include "CalcMethodLib/ICalcMethodSGP4.h"


// int main() {
//     // FileTxtReader Reader("C:\\Qt\\Projects\\SatelliteSelect\\SatelliteSelect\\testTLE.txt");

//     // Пример данных TLE (обычно они читаются из файла)
//     std::string line0 = "ISS (ZARYA)";
//     std::string line1 = "1 25544U 98067A   23277.53611111  .00016717  00000-0  10270-3 0  9000";
//     std::string line2 = "2 25544  51.6416 247.4627 0006703 130.5360 325.0288 15.72125391563537";

//     // Создаём объект Sgp4CalcMethod через указатель на ICalcMethodSGP4
//     std::unique_ptr<ICalcMethodSGP4> calcMethod = std::make_unique<Sgp4CalcMethod>();

//     // Устанавливаем данные TLE
//     calcMethod->SetSatellite(line0, line1, line2);

//     // Вызываем методы из ICalcMethodSGP4
//     std::cout << "Satellite Name: " << calcMethod->GetSatName() << std::endl;
//     std::cout << "NORAD ID: " << calcMethod->GetNORAD() << std::endl;

//     // Пример работы с юлианской датой
//     std::tm timeStruct = {};
//     timeStruct.tm_year = 2023 - 1900; // Год - 1900
//     timeStruct.tm_mon = 10;           // Месяц (0-11)
//     timeStruct.tm_mday = 4;           // День
//     double julianDate = calcMethod->JulianDate(timeStruct);
//     std::cout << "Julian Date: " << julianDate << std::endl;

//     // Преобразование юлианской даты обратно в календарную
//     std::tm calendarDate = calcMethod->CalendarDate(julianDate);
//     std::cout << "Calendar Date: " << asctime(&calendarDate);

//     if (calcMethod->Calculate(julianDate)) {
//         std::cout << "Latitude: " << calcMethod->GetLat() << " degrees" << std::endl;
//         std::cout << "Longitude: " << calcMethod->GetLon() << " degrees" << std::endl;
//         std::cout << "Altitude: " << calcMethod->GetAlt() << " km" << std::endl;
//     } else {
//         std::cerr << "Calculation failed" << std::endl;
//     }

//     return 1;
// }

