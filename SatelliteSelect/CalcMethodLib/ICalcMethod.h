#ifndef ICALCMETHOD_H
#define ICALCMETHOD_H


class ICalcMethod{
public:
    struct name
    {
        name() {}
    };
    virtual ~ICalcMethod() = default;
    virtual bool Calculate(const double time) = 0;
    virtual double GetLat() const = 0;
    virtual double GetLon() const = 0;
    virtual double GetAlt() const = 0;


};


#endif // ICALCMETHOD_H
