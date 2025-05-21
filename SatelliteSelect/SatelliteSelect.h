#ifndef SATELLITESELECT_H
#define SATELLITESELECT_H

#include <string>
#include <optional>
#include <vector>
#include <cmath>
#include <memory>
#include "time.h"


#include "CalcMethodLib/Sgp4CalcMethod.h"
#include "StationClass.h"
#include "CoordWorkerUtils.h"
#include "ReaderLib/FileTxtReader.h"



class SatelliteSelect {
public:

	struct SatelliteData {
		std::string name{};
		std::string onTime{};
       CoordWorkerUtils::COORDS coords{};
        std::optional<double> time = 0;
		unsigned int noradNumber = 0;
		bool dirPositive{};
	};

private:

    std::unique_ptr<ICalcMethod> Sgp4Calc;
    ICalcMethodSGP4* extMethod;
    std::unique_ptr<IReader> Reader;

    std::vector<SatelliteData> satellite;
	std::vector<FileTxtReader::dataFrame> NoradData;
	StationClass station;

    double timeBuffer;

	void SetNorad(FileTxtReader::dataFrame& data);


	bool isSpeedMatch();
	bool SatInViewOfStation(const CoordWorkerUtils::CoordDecart Satellite_1, double& theta, double& fi);
	bool GetSatDiraction();
	void FillSatData(SatelliteData& tmp, const double time, const double theta, const double fi);
	
    std::optional<double> isTimeIntersect(double& theta, double& fi, tm upperTime, tm lowerTime);
	CoordWorkerUtils::CoordDecart SatellitePos(CoordWorkerUtils::CoordGeodetic& SatelliteLLA_1, CoordWorkerUtils::CoordDecart Satellite_1, double time);
	

public:
    SatelliteSelect(double latitude, double longitude, double altitude,
                    double minAzmute, double maxAzmute, double minElavation, double maxElavation, std::size_t timeMinObserveSeconds);
    SatelliteSelect(double latitude, double longitude, double altitude, StationClass::StationVision limits);
    SatelliteSelect(CoordWorkerUtils::CoordGeodetic geodetic, StationClass::StationVision limits);
    SatelliteSelect(StationClass st);

    bool SetReadModel( std::unique_ptr<IReader> readModel);
    bool SetCalcMethod(std::unique_ptr<ICalcMethod> calcModel);
    // bool SetStationParam();
    // b    ool SetStationParam(const StationClass::StationParams &param);

    void DataPrepare();

    void Solve();

	/*
	Function return final satellite array
	*/
    const std::vector<SatelliteSelect::SatelliteData> GetSatArray() const;
	/*
	Function output final satellite array to the console
	*/
	void ShowResult() const;
    void ShowResult(const std::vector<SatelliteSelect::SatelliteData>& result) const;

    ~SatelliteSelect() = default;
};

#endif // SOLVER_H
