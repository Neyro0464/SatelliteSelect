#include <sstream>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "SatelliteSelect.h"

using namespace std::chrono;

SatelliteSelect::SatelliteSelect():
Reader(nullptr)
{
    station = {};
    Sgp4Calc = {};
    satellite = {};
    NoradData = {};
}

SatelliteSelect::SatelliteSelect(StationClass::StationParams st) :
    Reader(nullptr), station(st)
{
    Sgp4Calc = {};
    satellite = {};
    NoradData = {};
}

ErrorHandler::Error SatelliteSelect::SetReadModel(IReader* readModel, const std::string& filename)
{
    Reader = readModel;
    if (Reader == nullptr) {
        return { ErrorHandler::SOLVER, "ERROR: readModel is nullptr" };
    }
    if (!Reader->SetReaderSrc(filename)) {
        return { ErrorHandler::SOLVER, "ERROR: couldn't open the file" };
    }
    return { ErrorHandler::SUCCESS, "" };
}

ErrorHandler::Error SatelliteSelect::SetStationParam()
{
    ErrorHandler::Error e{};
    e = station.InitParamsConsole();
    if (!e)
        return e;
    return { ErrorHandler::SUCCESS, "" };
}

ErrorHandler::Error SatelliteSelect::SetStationParam(const StationClass::StationParams& param) {
    ErrorHandler::Error e = {};
    e = station.SetStationPos(param.geo);
    if (!e)
        return e;
    e = station.SetStationFilter(param.lim);
    if (!e)
        return e;
    return { ErrorHandler::SUCCESS, "" };
}

void SatelliteSelect::SetNorad(FileTxtReader::dataFrame& data) {
    Sgp4Calc.SetSatellite(std::get<0>(data), std::get<1>(data), std::get<2>(data));
    return;
}

void SatelliteSelect::DataPrepare() {
    FileTxtReader::dataFrame tmp = {};
    while (true) {
        auto&&[line0, line1, line2] = Reader->GetNextData();

        if (line0.empty() && line1.empty() && line2.empty()) break;

        Sgp4Calc.SetSatellite(line0, line1, line2);
        
        if (!isSpeedMatch()) {
            continue;
        }
        NoradData.push_back({ line0, line1, line2 });
        
    }
}


// Checking if velocity of satellite does not exceed 1.9 deg/min
// return true if >= 1.9
bool SatelliteSelect::isSpeedMatch() {
    CoordWorkerUtils::CoordDecart Satellite_1{};
    CoordWorkerUtils::CoordDecart Satellite_2{};
    CoordWorkerUtils::CoordGeodetic SatelliteGEO{};

    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);
    struct tm now_tm;
    gmtime_s(&now_tm, &now_time_t);

    tm Time2 = now_tm;
    Time2.tm_min += 1;

    double time = Sgp4Calc.JulianDate(now_tm);
    double time2 = Sgp4Calc.JulianDate(Time2);

    Sgp4Calc.SGP(time);
    Satellite_1 = SatellitePos(SatelliteGEO, Satellite_1, time);
    Satellite_1 = CoordWorkerUtils::CoordShift(Satellite_1, station.GetStationGeodetic());

    double x = station.GetStationDecart().x;
    double y = station.GetStationDecart().y;
    double z = station.GetStationDecart().z;

    double theta = atan(sqrt(pow(Satellite_1.x + 6371.0 - x, 2) + pow(Satellite_1.y - y, 2)) / (Satellite_1.z - z));
    theta = CoordWorkerUtils::RadToDeg(theta);

    Sgp4Calc.SGP(time2);
    Satellite_2 = SatellitePos(SatelliteGEO, Satellite_2, time);
    Satellite_2 = CoordWorkerUtils::CoordShift(Satellite_2, station.GetStationGeodetic());

    double theta2 = atan(sqrt(pow(Satellite_2.x + 6371.0 - x, 2) + pow(Satellite_2.y - y, 2)) / (Satellite_2.z - z));
    theta2 = CoordWorkerUtils::RadToDeg(theta2);

    return (std::abs(theta2 - theta) < 1.9);
}


// SGP satellite position with class function
// then convert to Decart coordinates 
CoordWorkerUtils::CoordDecart SatelliteSelect::SatellitePos(CoordWorkerUtils::CoordGeodetic& SatelliteLLA_1, CoordWorkerUtils::CoordDecart Satellite_1, double time) {

    Sgp4Calc.SGP(time);
    Sgp4Calc.CalculateLatLonAlt(time);
    SatelliteLLA_1.Lat = Sgp4Calc.GetLat();
    SatelliteLLA_1.Lon = Sgp4Calc.GetLon();
    SatelliteLLA_1.Alt = Sgp4Calc.GetAlt();

    SatelliteLLA_1.Lat = CoordWorkerUtils::DegToRad(SatelliteLLA_1.Lat);
    SatelliteLLA_1.Lon = CoordWorkerUtils::DegToRad(SatelliteLLA_1.Lon);

    Satellite_1 = CoordWorkerUtils::ConvertGEOtoDecart(SatelliteLLA_1);
    return Satellite_1;
}



// Checking intersection: if satellite in view area of station
bool SatelliteSelect::SatInViewOfStation(const CoordWorkerUtils::CoordDecart Satellite_1, double& theta, double& fi) {
    double x = Satellite_1.x;
    double y = Satellite_1.y;
    double z = Satellite_1.z;

    // longtitude and latitude of the station to rotate around the sphere(Earth) 
    
    double alpha = station.GetStationGeodetic().Lon;
    double beta = station.GetStationGeodetic().Lat;
    double R = 6371.0 + station.GetStationGeodetic().Alt;

    double new_x = z * cos(beta) - sin(beta) * (y * sin(alpha) + x * cos(alpha));
    double new_y = y * cos(alpha) - x * sin(alpha);
    double new_z = (z * sin(beta) + cos(beta) * (y * sin(alpha) + x * cos(alpha))) - R;

    if (new_z <= 0) {
        return false;
    }

    fi = atan(new_y / new_x) + 3.141592653589793; // +PI to normalize lower and upper bounds
    theta = 3.141592653589793 / 2 - acos(new_z / sqrt((pow(new_x, 2) + pow(new_y, 2) + pow(new_z, 2)))); // PI/2 to start from the bottom to the top

    if (fi <= station.GetStationVision().maxAzm && fi >= station.GetStationVision().minAzm) {
        if (theta <= station.GetStationVision().maxElv && theta >= station.GetStationVision().minElv) {
            return true;
        }
    }
    return false;
}


// checks the intersection of the satellite with the station's visibility cone (+30 sec to +30 min)
//  Filter Sattelites that would be in cone of visibility for timeMinObserve(sec)
std::optional<double> SatelliteSelect::isTimeIntersect(double& theta, double& fi, tm temp, tm upperTime, tm lowerTime) {


    double lowerTm = Sgp4Calc.JulianDate(lowerTime); // lower bound of time interval
    double tempTm = Sgp4Calc.JulianDate(temp); // time cursor that move between lower and upper bounds of time interval
    double upperTm = Sgp4Calc.JulianDate(upperTime); // upper bound of time interval

    CoordWorkerUtils::CoordGeodetic SatelliteLLA_1{};
    CoordWorkerUtils::CoordDecart Satellite_1{};
    bool check1 = 0;

    //searching if Satellite will be in vision cone sometime in the time interval
    while ((check1 == false || tempTm <= lowerTm) && tempTm <= upperTm) {
        tempTm = Sgp4Calc.JulianDate(temp);
        Satellite_1 = SatellitePos(SatelliteLLA_1, Satellite_1, tempTm);
        check1 = SatInViewOfStation(Satellite_1, theta, fi);
        temp.tm_sec += 1;
    }

    if (tempTm > upperTm || tempTm < lowerTm) {
        return std::nullopt;
    }

    double tmpfi = fi;
    double tmtheta = theta;

    // block for time filter
    // checking if satellite is in vision cone after minTimeObserve
    tm filterTime = temp;
    filterTime.tm_sec += station.GetStationVision().timeMinObserveSec - 1;
    double checkTime = Sgp4Calc.JulianDate(filterTime);
    Sgp4Calc.SGP(checkTime);
    Satellite_1 = SatellitePos(SatelliteLLA_1, Satellite_1, checkTime);
    if (SatInViewOfStation(Satellite_1, tmtheta, tmpfi)) {
        return tempTm;
    }
    else
        return std::nullopt;
}

// Check direction by defining sign of the angle between station and satellite
bool SatelliteSelect::GetSatDiraction() {
    CoordWorkerUtils::CoordDecart Satellite_1{}; // coordinates at the first moment
    CoordWorkerUtils::CoordDecart Satellite_2{}; // coordinates at the second moment
    CoordWorkerUtils::CoordGeodetic SatelliteLLA;

    double len = 0.0, len2 = 0.0; // to calculate radius vector

    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);
    struct tm now_tm;
    gmtime_s(&now_tm, &now_time_t); // Working in UTC time

    tm Time2 = now_tm;
    Time2.tm_min += 1;

    double time = Sgp4Calc.JulianDate(now_tm);
    double time2 = Sgp4Calc.JulianDate(Time2);

    Sgp4Calc.SGP(time);
    Satellite_1 = SatellitePos(SatelliteLLA, Satellite_1, time);

    double x = station.GetStationDecart().x;
    double y = station.GetStationDecart().y;
    double z = station.GetStationDecart().z;

    len = sqrt(pow(Satellite_1.x - x, 2) + pow(Satellite_1.y - y, 2) + pow(Satellite_1.z - z, 2));

    Sgp4Calc.SGP(time2);
    Satellite_2 = SatellitePos(SatelliteLLA, Satellite_2, time);
    len2 = sqrt(pow(Satellite_2.x - x, 2) + pow(Satellite_2.y - y, 2) + pow(Satellite_2.z - z, 2));

    if (len2 - len < 0) {   // If diff < 0 that means satellite go closer
        return true;
    }
    else {                  // If diff > 0 that means satellite go further
        return false;
    }
}

void SatelliteSelect::FillSatData(SatelliteData& tmp, const double time, const double theta, const  double fi) {
    tm timeDate = {};
    std::stringstream onTime1;

    tmp.noradNumber = Sgp4Calc.GetNORAD();
    std::string satName = Sgp4Calc.GetSatName();
    tmp.dirPositive = GetSatDiraction();
    Sgp4Calc.SGP(time);
    Sgp4Calc.CalculateLatLonAlt(time);
    tmp.coords.geo.Lat = Sgp4Calc.GetLat();
    tmp.coords.geo.Lon = Sgp4Calc.GetLon();
    tmp.coords.geo.Alt = Sgp4Calc.GetAlt();
    tmp.coords.topo.azm = Sgp4Calc.RadToDeg(fi);
    tmp.coords.topo.elv = Sgp4Calc.RadToDeg(theta);
    tmp.name = satName;

    timeDate = Sgp4Calc.CalendarDate(time);
    onTime1 << std::to_string(timeDate.tm_mday)
        << "/" << std::to_string(timeDate.tm_mon + 1)
        << "/" << std::to_string(timeDate.tm_year + 1900)
        << "\t" << std::to_string(timeDate.tm_hour)
        << ":" << std::to_string(timeDate.tm_min)
        << ":" << std::to_string(timeDate.tm_sec);
    tmp.onTime = onTime1.str();

    tmp.time = time;
}

void SatelliteSelect::Solve() {
    satellite.clear();
    unsigned int N = static_cast<unsigned int>(NoradData.size());      // number of Sattelites that passed verification by speed
    std::optional<double> timeCheck = 0;                // time when satellite will be in cone of view
    double theta = 0;               // ELV
    double fi = 0;                  // AZM
    std::stringstream onTime1;
    SatelliteData tmpSat{};


    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);

    struct tm now_tm;
    gmtime_s(&now_tm, &now_time_t);  // Working in UTC time

    tm lowerTime = now_tm;  // lower bound of time (current time + 30 sec)
    lowerTime.tm_sec += 30;
    tm temp = now_tm;       // time that we shift to find required time (current time)
    tm upperTime = now_tm;  // upper bound of time (current time + 30 min)
    upperTime.tm_min += 30;

    for (unsigned int i = 0; i < N; i++) {
        tmpSat = {};
        SetNorad(NoradData[i]);
        timeCheck = isTimeIntersect(theta, fi, temp, upperTime, lowerTime);
        if (timeCheck != std::nullopt) {
            double time = timeCheck.value();
            FillSatData(tmpSat, time, theta, fi);
            if(tmpSat.coords.geo.Alt < 1151)
                satellite.push_back(tmpSat);
            onTime1.str().clear();
        }
    }

    std::sort(satellite.begin(), satellite.end(), [](const SatelliteData& a, const SatelliteData& b)
        {return a.time < b.time; }
    );

}

const std::vector<SatelliteSelect::SatelliteData> SatelliteSelect::GetSatArray() const{
    return satellite;
}


void SatelliteSelect::ShowResult() const
{
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    for (const auto& item : satellite) {

        std::cout << item.name << "|" << item.noradNumber << " |" << item.coords.topo.azm << "|" << item.coords.topo.elv << "| "
            << item.coords.geo.Lat << " |" << item.coords.geo.Lon << " |" << item.coords.geo.Alt << " | "
            << item.dirPositive << " | " << item.onTime << std::endl;
    }
}

void SatelliteSelect::ShowResult(const std::vector<SatelliteData>& SattelitesRes) const{
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    for (const auto& item : SattelitesRes) {

        std::cout << item.name << "|" << item.noradNumber << " |" << item.coords.topo.azm << "|" << item.coords.topo.elv << "| "
            << item.coords.geo.Lat << " |" << item.coords.geo.Lon << " |" << item.coords.geo.Alt << " | "
            << item.dirPositive << " | " << item.onTime << std::endl;
    }
}


