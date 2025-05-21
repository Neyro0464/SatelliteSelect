#ifndef STATIONCLASS_H
#define STATIONCLASS_H

#include "CoordWorkerUtils.h"


class StationClass{
public:
    struct StationVision {
        double minAzm = 0;
        double maxAzm = 0;
        double minElv = 0;
        double maxElv = 0;
        std::size_t timeMinObserveSec = 0;
    };

    StationClass(double latitude, double longitude, double altitude,
                 double minAzmute, double maxAzmute, double minElavation, double maxElavation, std::size_t timeMinObserveSeconds);
    StationClass(double latitude, double longitude, double altitude, StationVision limits);
    StationClass(CoordWorkerUtils::CoordGeodetic geodetic, StationVision limits);

    CoordWorkerUtils::CoordDecart GetCoordDecart() const { return dec; };
    CoordWorkerUtils::CoordGeodetic GetStationGeodetic() const { return geo; };
    StationVision GetStationVision() const { return lim; };

private:
    CoordWorkerUtils::CoordDecart dec{};
    const CoordWorkerUtils::CoordGeodetic geo{};
    StationVision lim{};
    // const StationParams station;

    void ConvertLimDegToRad();
    void CalcCoordDecart() {  dec = CoordWorkerUtils::ConvertGEOtoDecart(geo); };
    bool CheckParams() const;

};

#endif // !STATIONCLASS_H

