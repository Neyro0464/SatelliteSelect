#include <iostream>

#include "StationClass.h"

/**************************************************
 * STATIONPARAMS BLOCK START
 *
 *
 *
 *
 **************************************************/
StationClass::StationParams::StationParams(double latitude, double longitude, double altitude,
                                           double minAzmute, double maxAzmute, double minElavation, double maxElavation,
                                           std::size_t timeMinObserveSeconds):
geo{latitude, longitude, altitude}, lim{minAzmute, maxAzmute, minElavation, maxElavation, timeMinObserveSeconds}
{
    if(!CheckParams()){
        throw std::invalid_argument("Wrong station params");
    }
    CalcCoordDecart();
};
StationClass::StationParams::StationParams(CoordWorkerUtils::CoordGeodetic geodetic, StationVision limits):
    geo{geodetic}, lim{limits}
{
    if(!CheckParams()){
        throw std::invalid_argument("Wrong station params");
    }
    CalcCoordDecart();
}

bool StationClass::StationParams::CheckParams() const{
    if (geo.Lat < -90 || geo.Lat > 90)      return false;
    if(geo.Lon < -180 || geo.Lon > 180)     return false;
    if( geo.Alt < 0) return false;
    if (lim.minAzm < 0 || lim.minAzm > lim.maxAzm || lim.maxAzm >= 360) return false;
    if (lim.minElv < 10 || lim.maxElv < lim.minElv || lim.maxElv > 180) return false;
    if (lim.timeMinObserveSec < 1) return false;
    return true;
};

/**************************************************
 * STATIONPARAMS BLOCK END
 *
 *
 *
 *
 **************************************************/



// ErrorHandler::Error StationClass::InitParamsConsole(){
//     StationParams param;
//     double Lat = 0, Lon = 0, Alt = -1;
//     std::cout << "ENTER Latitude, Longitude, Altitude of the station: ";
//     std::cin >> Lat >> Lon >> Alt;
//     double minAzm = 0, maxAzm = 0;
//     double minElv = 0, maxElv = 0;
//     unsigned int timeMinObserveSec = 0;
//     std::cout << "ENTER min and max azimut limits of the station: ";
//     std::cin >> minAzm >> maxAzm ;
//     std::cout << "ENTER min and max elevation limits of the station: ";
//     std::cin >> minElv >> maxElv;
//     std::cout << "ENTE min time observed of the station: ";
//     std::cin >> timeMinObserveSec;
//     param.geo = {Lat, Lon, Alt};
//     param.lim = {minAzm, maxAzm, minElv, maxElv, timeMinObserveSec};
//     if(CheckGeo(param.geo) &&
//         CheckLim(param.lim))
//     {
//         SetStationPos(param.geo);
//         SetStationFilter(param.lim);
//         return {ErrorHandler::SUCCESS, ""};
//     }
//     else
//         return {ErrorHandler::STATION, "Wrong parameters"};
// }

// void StationClass::SetStation(StationParams tmp){
//     station = tmp;
// }

// ErrorHandler::Error StationClass::SetStationPos(CoordWorkerUtils::CoordGeodetic geo) {
//     if(!CheckGeo(geo))
//         return {ErrorHandler::STATION, "Wrong parameters"};
//     station.geo.Lat = CoordWorkerUtils::DegToRad(geo.Lat);
//     station.geo.Lon = CoordWorkerUtils::DegToRad(geo.Lon);
//     station.geo.Alt = geo.Alt;
//     station.CalcCoordDecart();
//     return { ErrorHandler::SUCCESS, "" };
// }

// ErrorHandler::Error StationClass::SetStationFilter(StationVision limits)
// {
//     if(!CheckLim(limits))
//         return {ErrorHandler::STATION, "Wrong parameters"};
//     //station.lim.Init(limits);
//     station.lim.minAzm = CoordWorkerUtils::DegToRad(limits.minAzm);
//     station.lim.maxAzm = CoordWorkerUtils::DegToRad(limits.maxAzm);
//     station.lim.minElv = CoordWorkerUtils::DegToRad(limits.minElv);
//     station.lim.maxElv = CoordWorkerUtils::DegToRad(limits.maxElv);
//     station.lim.timeMinObserveSec = limits.timeMinObserveSec;
//     return { ErrorHandler::SUCCESS, "" };
// }
