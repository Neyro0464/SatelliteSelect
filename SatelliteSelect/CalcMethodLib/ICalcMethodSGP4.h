#ifndef ICALCMETHODSGP4_H
#define ICALCMETHODSGP4_H

#include <string>
#include <ctime>
#include "ICalcMethod.h"

class ICalcMethodSGP4: public ICalcMethod{
public:
    virtual void SetSatellite(char* m_cLine0, char* m_cLine1, char* m_cLine2) = 0;
    virtual void SetSatellite(std::string& cLine0, std::string& cLine1, std::string& cLine2) = 0;
    virtual std::string GetSatName() const = 0;
    virtual int GetNORAD() const = 0;
    virtual double JulianDate(tm time) = 0;
    virtual double JulianDate(double st) = 0;
    virtual tm CalendarDate(double dJulian) = 0;

};


#endif // ICALCMETHODSGP4_H
