#include <iostream>

#include "StationClass.h"

StationClass::StationClass(double latitude, double longitude, double altitude,
             double minAzmute, double maxAzmute, double minElavation, double maxElavation, std::size_t timeMinObserveSeconds):
geo{latitude, longitude, altitude}, lim{minAzmute, maxAzmute, minElavation, maxElavation, timeMinObserveSeconds}
{
    if(!CheckParams()){
        throw std::invalid_argument("Wrong station params");
    }
    CalcCoordDecart();
    ConvertStationParamToRad();
}

StationClass::StationClass(double latitude, double longitude, double altitude, StationVision limits):
    geo{latitude, longitude, altitude}, lim{limits}
{
    if(!CheckParams()){
        throw std::invalid_argument("Wrong station params");
    }
    CalcCoordDecart();
    ConvertStationParamToRad();
}

StationClass::StationClass(CoordWorkerUtils::CoordGeodetic geodetic, StationVision limits):
    geo{geodetic}, lim{limits}
{
    if(!CheckParams()){
        throw std::invalid_argument("Wrong station params");
    }
    CalcCoordDecart();
    ConvertStationParamToRad();
}
void StationClass::ConvertStationParamToRad(){
    geo.Lat = CoordWorkerUtils::DegToRad(geo.Lat);
    geo.Lon = CoordWorkerUtils::DegToRad(geo.Lon);

    lim.minAzm = CoordWorkerUtils::DegToRad(lim.minAzm);
    lim.maxAzm = CoordWorkerUtils::DegToRad(lim.maxAzm);

    lim.minElv = CoordWorkerUtils::DegToRad(lim.minElv);
    lim.maxElv = CoordWorkerUtils::DegToRad(lim.maxElv);
    return;
}

bool StationClass::CheckParams() const{
    if (geo.Lat < -90 || geo.Lat > 90)      return false;
    if(geo.Lon < -180 || geo.Lon > 180)     return false;
    if( geo.Alt < 0) return false;
    if (lim.minAzm < 0 || lim.minAzm > lim.maxAzm || lim.maxAzm >= 360) return false;
    if (lim.minElv < 10 || lim.maxElv < lim.minElv || lim.maxElv > 180) return false;
    if (lim.timeMinObserveSec < 1) return false;
    return true;
};
