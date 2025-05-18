#ifndef STATIONCLASS_H
#define STATIONCLASS_H

#include "CoordWorkerUtils.h"


class StationClass{
private:
    struct StationVision {
		double minAzm = 0;
		double maxAzm = 0;
		double minElv = 0;
		double maxElv = 0;
        std::size_t timeMinObserveSec = 0;
	};

public:
    struct StationParams
    {
    private:
        CoordWorkerUtils::CoordDecart dec{};
        bool CheckParams() const;
        void CalcCoordDecart() { dec = CoordWorkerUtils::ConvertGEOtoDecart(geo); };

    public:
        StationParams(double latitude, double longitude, double altitude,
                      double minAzmute, double maxAzmute, double minElavation, double maxElavation, std::size_t timeMinObserveSeconds);

        StationParams(CoordWorkerUtils::CoordGeodetic geodetic, StationVision limits);

        CoordWorkerUtils::CoordGeodetic geo{};
        StationVision lim{};

        CoordWorkerUtils::CoordDecart GetCoordDecart() const { return dec; };
    };

private:
    const StationParams station;
	
public:
    StationClass(StationParams a);

    CoordWorkerUtils::CoordDecart GetStationDecart() const { return station.GetCoordDecart(); };
    CoordWorkerUtils::CoordGeodetic GetStationGeodetic() const { return station.geo; };
    StationVision GetStationVision() const { return station.lim; };
};

#endif // !STATIONCLASS_H

