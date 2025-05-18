#ifndef STATIONCLASS_H
#define STATIONCLASS_H

#include "CoordWorkerUtils.h"
#include "ErrorHandler.h"

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

    public:
        CoordWorkerUtils::CoordGeodetic geo{};
        StationVision lim{};        
        CoordWorkerUtils::CoordDecart GetCoordDecart() const { return dec; };
        void CalcCoordDecart() { dec = CoordWorkerUtils::ConvertGEOtoDecart(geo); };
    };

private:
	StationParams station;
    bool CheckGeo(CoordWorkerUtils::CoordGeodetic geo) const;
    bool CheckLim(StationVision vis) const;
	
public:
    StationClass() = default;
    StationClass(StationParams a);

    ErrorHandler::Error InitParamsConsole();
	/*
	Function for filling in the value from the console
	param in: empty Station struct
	param out: filled Station strut and bool(true if success; false if error)
	*/

    ErrorHandler::Error SetStationPos(CoordWorkerUtils::CoordGeodetic geo);
	
	/*
	Function for filling in the value from the console
	param in: empty Station struct
	param out: filled Station strut and bool(true if success; false if error)
	*/
    void SetStation(StationParams tmp);
    ErrorHandler::Error SetStationFilter(StationVision limits);


    StationParams GetStation() const { return station; };
    CoordWorkerUtils::CoordDecart GetStationDecart() const { return station.GetCoordDecart(); };
	CoordWorkerUtils::CoordGeodetic GetStationGeodetic() const { return station.geo; };
	StationVision GetStationVision() const { return station.lim; };
};

#endif // !STATIONCLASS_H

