#include "CoordWorkerUtils.h"

CoordWorkerUtils::CoordDecart CoordWorkerUtils::ConvertGEOtoDecart(const CoordGeodetic Object) {
    double x = (R + Object.Alt) * cos(Object.Lat) * cos(Object.Lon);
    double y = (R + Object.Alt) * cos(Object.Lat) * sin(Object.Lon);
    double z = (R + Object.Alt) * sin(Object.Lat);

    return CoordDecart{ x, y, z };
}
CoordWorkerUtils::CoordDecart CoordWorkerUtils::ConvertGEOtoDecart(const double Lat, const double Lon, const double Alt) {
    double x = (R + Alt) * cos(Lat) * cos(Lon);
    double y = (R + Alt) * cos(Lat) * sin(Lon);
    double z = (R + Alt) * sin(Lat);

    return CoordDecart{ x, y, z };
}

// Transfer satellite coordinate system in station coodinate system
CoordWorkerUtils::CoordDecart CoordWorkerUtils::CoordShift(const CoordDecart shiftedObject, const CoordGeodetic object) {
    double x = shiftedObject.x + R + object.Alt;
    double y = shiftedObject.y;
    double z = shiftedObject.z;

    double alpha = object.Lon;
    double beta = object.Lat;

    double x0 = cos(beta) * (x * cos(alpha) - y * sin(alpha)) - z * sin(beta);
    double y0 = x * sin(alpha) + y * cos(alpha);
    double z0 = sin(beta) * (x * cos(alpha) - y * sin(alpha)) + z * cos(beta);

    return CoordDecart{ x0, y0, z0 };
}
