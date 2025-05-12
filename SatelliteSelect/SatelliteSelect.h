#ifndef SATELLITESELECT_H
#define SATELLITESELECT_H

#include <string>
#include <optional>
#include <vector>
#include <cmath>
#include "time.h"


#include "Sgp4CalcMethod.h"
#include "StationClass.h"
#include "CoordWorkerUtils.h"
#include "FileTxtReader.h"



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

	Sgp4CalcMethod Sgp4Calc;
	std::vector<SatelliteData> satellite;
	IReader* Reader;
	std::vector<FileTxtReader::dataFrame> NoradData;
	StationClass station;

	void SetNorad(FileTxtReader::dataFrame& data);
	bool isSpeedMatch();
	bool SatInViewOfStation(const CoordWorkerUtils::CoordDecart Satellite_1, double& theta, double& fi);
	bool GetSatDiraction();
	void FillSatData(SatelliteData& tmp, const double time, const double theta, const double fi);
	
    std::optional<double> isTimeIntersect(double& theta, double& fi, tm temp, tm upperTime, tm lowerTime);
	CoordWorkerUtils::CoordDecart SatellitePos(CoordWorkerUtils::CoordGeodetic& SatelliteLLA_1, CoordWorkerUtils::CoordDecart Satellite_1, double time);
	

public:
    SatelliteSelect();
    SatelliteSelect(StationClass::StationParams st);

	ErrorHandler::Error SetReadModel(IReader* readModel, const std::string& filename);
	/*ErrorHandler::Error SetCalcMethod(ICalcMethod* calcModel);*/
	ErrorHandler::Error SetStationParam();
    ErrorHandler::Error SetStationParam(const StationClass::StationParams &param);

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
