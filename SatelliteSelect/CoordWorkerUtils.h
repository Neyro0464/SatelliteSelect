#ifndef COORDWORKERUTILS_H
#define COORDWORKERUTILS_H

#include <cmath>


class CoordWorkerUtils final {
public:
	CoordWorkerUtils() = delete;

	struct CoordDecart {
		double x = 0;
		double y = 0;
		double z = 0;
	};
	struct CoordTopocentric {
		double azm = 0;
		double elv = 0;
	};
	struct CoordGeodetic {
		double Lat = 0;
		double Lon = 0;
		double Alt = 0;
	};
	struct COORDS {
		CoordTopocentric topo{};
		CoordGeodetic geo{};
	};

    static double RadToDeg(double arg) noexcept { return (arg / (2.0 * PI_M) * 360.0); };
    static double DegToRad(double arg) noexcept { return (arg / 360.0 * (2.0 * PI_M)); };

	static CoordDecart ConvertGEOtoDecart(const CoordGeodetic Object);
	static CoordDecart ConvertGEOtoDecart(const double Lat, const double Lon, const double Alt);
	static CoordDecart CoordShift(const CoordDecart shiftedObject, const CoordGeodetic object);

private:
	constexpr static double R = 6371.0; // Earth's radius (rounded up)
	constexpr static double PI_M = 3.141592653589793;
};


#endif // !COORDWORKERUTILS_H
