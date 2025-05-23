#include <sstream>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "SatelliteSelect.h"

using namespace std::chrono;

SatelliteSelect::SatelliteSelect(double latitude, double longitude, double altitude,
                 double minAzmute, double maxAzmute, double minElavation, double maxElavation, std::size_t timeMinObserveSeconds):
    Reader(nullptr),
    station (latitude, longitude, altitude, minAzmute, maxAzmute, minElavation, maxElavation, timeMinObserveSeconds)
{
    Sgp4Calc = {};
    satellite = {};
    NoradData = {};
}

SatelliteSelect::SatelliteSelect(StationClass st):
    Sgp4Calc(nullptr), Reader(nullptr), station(st)
{
    Sgp4Calc = {};
    satellite = {};
    NoradData = {};
}

bool SatelliteSelect::SetReadModel(std::unique_ptr<IReader> readModel)
{
    if (readModel == nullptr) {
        return false;
    }
    Reader = std::move(readModel);
    return true;
}

bool SatelliteSelect::SetCalcMethod(std::unique_ptr<ICalcMethod> calcModel) {
    if (calcModel == nullptr) {
        return false;
    }
    auto* sgp4Calc = dynamic_cast<ICalcMethodSGP4*>(calcModel.get());
    if (sgp4Calc == nullptr) {
        return false;
    }
    Sgp4Calc = std::move(calcModel);
    extMethod = sgp4Calc;
    return true;
}

// bool SatelliteSelect::SetStationParam()
// {
//     e = station.InitParamsConsole();
//     if (!e)
//         return e;
//     return { ErrorHandler::SUCCESS, "" };
// }

// ErrorHandler::Error SatelliteSelect::SetStationParam(const StationClass::StationParams& param) {
//     ErrorHandler::Error e = {};
//     e = station.SetStationPos(param.geo);
//     if (!e)
//         return e;
//     e = station.SetStationFilter(param.lim);
//     if (!e)
//         return e;
//     return { ErrorHandler::SUCCESS, "" };
// }



void SatelliteSelect::SetNorad(FileTxtReader::dataFrame& data) {
    extMethod->SetSatellite(std::get<0>(data), std::get<1>(data), std::get<2>(data));
    return;
}

void SatelliteSelect::DataPrepare() {
    while (true) {
        auto&& b = Reader->GetFrame();
        if(!b.has_value()) break;

        extMethod->SetSatellite(std::get<0>(b.value()), std::get<1>(b.value()), std::get<2>(b.value()));

        if (!isSpeedMatch()) {
            continue;
        }
        NoradData.push_back(b.value());
        
    }
}

void SatelliteSelect::showNorad(){
    for (auto& i: NoradData){
        std::cout<< std::get<0>(i) << "\n";
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
    if (std::gmtime(&now_time_t) == nullptr) {
        return false;
    }
    now_tm = *std::gmtime(&now_time_t); // Copy the tm structure

    tm Time2 = now_tm;
    Time2.tm_min += 1;

    double time = extMethod->JulianDate(now_tm);
    double time2 = extMethod->JulianDate(Time2);

    // Sgp4Calc->Calculate(time);
    Satellite_1 = SatellitePos(SatelliteGEO, Satellite_1, time);
    Satellite_1 = CoordWorkerUtils::CoordShift(Satellite_1, station.GetStationGeodetic());

    double x = station.GetCoordDecart().x;
    double y = station.GetCoordDecart().y;
    double z = station.GetCoordDecart().z;

    double theta = atan(sqrt(pow(Satellite_1.x + 6371.0 - x, 2) + pow(Satellite_1.y - y, 2)) / (Satellite_1.z - z));
    theta = CoordWorkerUtils::RadToDeg(theta);

    // Sgp4Calc->Calculate(time2);
    Satellite_2 = SatellitePos(SatelliteGEO, Satellite_2, time2);
    Satellite_2 = CoordWorkerUtils::CoordShift(Satellite_2, station.GetStationGeodetic());

    double theta2 = atan(sqrt(pow(Satellite_2.x + 6371.0 - x, 2) + pow(Satellite_2.y - y, 2)) / (Satellite_2.z - z));
    theta2 = CoordWorkerUtils::RadToDeg(theta2);

    return (std::abs(theta2 - theta) < 1.9);
}


// SGP satellite position with class function
// then convert to Decart coordinates 
CoordWorkerUtils::CoordDecart SatelliteSelect::SatellitePos(CoordWorkerUtils::CoordGeodetic& SatelliteLLA_1, CoordWorkerUtils::CoordDecart Satellite_1, double time) {

    Sgp4Calc->Calculate(time);
    SatelliteLLA_1.Lat = Sgp4Calc->GetLat();
    SatelliteLLA_1.Lon = Sgp4Calc->GetLon();
    SatelliteLLA_1.Alt = Sgp4Calc->GetAlt();

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
std::optional<double> SatelliteSelect::isTimeIntersect(double& theta, double& fi, tm upperTime, tm lowerTime) {

    tm timeSearcher = lowerTime;
    double lowerTm = extMethod->JulianDate(lowerTime); // lower bound of time interval
    double tempTm = extMethod->JulianDate(timeSearcher); // time cursor that move between lower and upper bounds of time interval
    double upperTm = extMethod->JulianDate(upperTime); // upper bound of time interval

    CoordWorkerUtils::CoordGeodetic SatelliteLLA_1{};
    CoordWorkerUtils::CoordDecart Satellite_1{};
    bool checkIntersection = false;

    //searching if Satellite will be in vision cone sometime in the time interval
    // check postition every minTimeObserveSec
    while (checkIntersection == false && tempTm <= upperTm) {
        tempTm = extMethod->JulianDate(timeSearcher);
        Satellite_1 = SatellitePos(SatelliteLLA_1, Satellite_1, tempTm);
        checkIntersection = SatInViewOfStation(Satellite_1, theta, fi);

        timeSearcher.tm_sec += station.GetStationVision().timeMinObserveSec;
    }
    // Chech if satellite intersect in needed time period
    if (tempTm > upperTm) {
        return std::nullopt;
    }

    // Searching accurate time(sec) of intersecting field od view
    // go back in time by 1 sec while satellite is in field of view
    while(checkIntersection == true){
        timeSearcher.tm_sec -= 1;
        tempTm = extMethod->JulianDate(timeSearcher);
        Satellite_1 = SatellitePos(SatelliteLLA_1, Satellite_1, tempTm);
        checkIntersection = SatInViewOfStation(Satellite_1, theta, fi);
    }

    timeSearcher.tm_sec += 1; // to neutralize last iteration and do satellite intersect
    tempTm = extMethod->JulianDate(timeSearcher);
    Satellite_1 = SatellitePos(SatelliteLLA_1, Satellite_1, tempTm);
    checkIntersection = SatInViewOfStation(Satellite_1, theta, fi);

    double tmpfi = fi;
    double tmtheta = theta;

    // Check if satellite will be in view of station for minTimeObserveSec
    tm filterTime = timeSearcher;
    filterTime.tm_sec += station.GetStationVision().timeMinObserveSec;
    double checkTime = extMethod->JulianDate(filterTime);
    extMethod->Calculate(checkTime);
    Satellite_1 = SatellitePos(SatelliteLLA_1, Satellite_1, checkTime);

    if (SatInViewOfStation(Satellite_1, tmtheta, tmpfi)) {
        if (tempTm < lowerTm) {
            return lowerTm;
        }
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
    if (std::gmtime(&now_time_t) == nullptr) {
        return false;
    }
    now_tm = *std::gmtime(&now_time_t); // Copy the tm structure

    tm Time2 = now_tm;
    Time2.tm_min += 1;

    double time = extMethod->JulianDate(now_tm);
    double time2 = extMethod->JulianDate(Time2);

    // extMethod->Calculate(time);
    Satellite_1 = SatellitePos(SatelliteLLA, Satellite_1, time);

    double x = station.GetCoordDecart().x;
    double y = station.GetCoordDecart().y;
    double z = station.GetCoordDecart().z;

    len = sqrt(pow(Satellite_1.x - x, 2) + pow(Satellite_1.y - y, 2) + pow(Satellite_1.z - z, 2));

    // extMethod->Calculate(time2);
    Satellite_2 = SatellitePos(SatelliteLLA, Satellite_2, time2);
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

    tmp.noradNumber = extMethod->GetNORAD();
    std::string satName = extMethod->GetSatName();
    tmp.dirPositive = GetSatDiraction();
    extMethod->Calculate(time);
    tmp.coords.geo.Lat = extMethod->GetLat();
    tmp.coords.geo.Lon = extMethod->GetLon();
    tmp.coords.geo.Alt = extMethod->GetAlt();

    tmp.coords.topo.azm = CoordWorkerUtils::RadToDeg(fi);
    tmp.coords.topo.elv = CoordWorkerUtils::RadToDeg(theta);
    tmp.name = satName;

    timeDate = extMethod->CalendarDate(time);
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

    tm now_tm;
    if (std::gmtime(&now_time_t) == nullptr) {
        return;
    }
    now_tm = *std::gmtime(&now_time_t); // Copy the tm structure

    tm lowerTime = now_tm;  // lower bound of time (current time + 30 sec)
    lowerTime.tm_sec += 30;

    tm upperTime = now_tm;  // upper bound of time (current time + 30 min)
    upperTime.tm_min += 30;

    for (unsigned int i = 0; i < N; i++) {
        tmpSat = {};
        SetNorad(NoradData[i]);
        timeCheck = isTimeIntersect(theta, fi, upperTime, lowerTime);
        if (timeCheck.has_value()) {
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


