#ifndef ICALCMETHOD_H
#define ICALCMETHOD_H


class ICalcMethod{
public:
    virtual ~ICalcMethod() = default;
    virtual void Calculate() = 0;


};


#endif // ICALCMETHOD_H
