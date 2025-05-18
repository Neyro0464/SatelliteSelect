#ifndef SGP4CALCMETHOD_H
#define SGP4CALCMETHOD_H

#include <cstring>
#include <string>
#include <ctime>
#include <time.h>
#include <cmath> 

//* Already defined in  >stdafx.h<
struct tagVECTOR {
    double x, y, z, w;
};

struct tagSATELLITE {
    char    cSatelliteName[23];
    int     iSecondMeanMotion;
    int     iSatelliteNumber;
    int     iLaunchYear;
    int     iLaunchNumber;
    char    cLaunchPiece[3];
    int     iEpochYear;
    double  fEpochDay;
    int     iEpochDay;
    double  fEpochFraction;
    double  fBalisticCoefficient;
    double  fSecondMeanMotion;
    double  fRadiationCoefficient;
    char    cEmphemeristType[2];
    int     iElementNumber;

    double  fInclination;
    double  fRightAscending;
    double  fEccentricity;
    double  fPeregee;
    double  fMeanAnomaly;
    double  fMeanMotion;
    int     iRevAtEpoch;
    double  fJulianEpoch;
};

#ifndef PI
#define PI 3.141592653589793
#endif

class Sgp4CalcMethod {
private:
    char        m_cLine0[23], m_cLine1[70], m_cLine2[70];
    tagSATELLITE   m_Sat;
    tagVECTOR      m_vLLA;
    tagVECTOR      m_vPOS;
    tagVECTOR      m_vVEL;
    tagVECTOR      m_vUPos;
    tagVECTOR      m_vUVel;
    tagVECTOR      m_vObs;
    tagVECTOR      m_vRad;
    tagVECTOR      m_vSolarPosition;
    double      m_fTime;

    bool        m_bLatLonAlt;
    double      ae;
    double      tothrd;
    double      xkmper;
    double      f;
    double      ge;
    double      J2;
    double      J3;
    double      J4;
    double      ck2;
    double      ck4;
    double      xj3;
    double      qo;
    double      s;
    double      e6a;
    double      dpinit;
    double      dpsec;
    double      dpper;
    long        secday;
    double      omega_E;
    double      sr;
    double      AU;

    int         iflag, ideep;
    double      xke, xmnpda;

    double      eqsq, siniq, cosiq, rteqsq, ao, cosq2, sinomo, cosomo;
    double      bsq, xlldot, omgdt, xnodot, xnodp;
    double      xll, omgasm, xnodes, _em, xinc, xn, t;
    double      qoms2t;
    bool        m_bEclipsed;


    double      Modulus(double a1, double a2);
    void        ConvertData();
    void        Init();
    long        round(double arg);
    double      Fmod2p(double arg);
    double      AcTan(double sinx, double cosx);
    double      sqr(double arg) const { return arg * arg; }
    void        Magnitude(tagVECTOR* pVector);
    double      Dot(tagVECTOR v1, tagVECTOR v2);
    double      VecDot(double* X, double* Y, int N);
    void        VecCross(double* X, double* Y, double* Z, int N);
    double      VecMag(double* X, int N);
    void        UnitVec(double* X, double* Y, int N);
    void        VecDiff(double* X, double* Y, double* Z, int N);
    void        VecSum(double* X, double* Y, double* Z, int N);
    void        VecScale(double u, double* X, double* Y, int N);

    bool SGP4(double tsince, int iflag, tagVECTOR&pos, tagVECTOR&vel);
    bool SDP4(double tsince, int iflag, tagVECTOR& pos, tagVECTOR& vel);
    void Call_dpper(double* e, double* xincc, double* omgadf, double* xnode, double* xmam);
    void Call_dpsec(double* xmdf, double* omgadf, double* xnode, double* emm, double* xincc,
        double* xnn, double* tsince);
    void Call_dpinit(double* eosq, double* sinio, double* cosio, double* betao, double* aodp,
        double* theta2, double* sing, double* cosg, double* betao2, double* xmdot,
        double* omgdot, double* xnodott, double* xnodpp);
    bool Deep(int ideep);

    void ConvertSatState(tagVECTOR& pos, tagVECTOR& vel);
    void InitSatellite();

    tagVECTOR CalculateLatLonAlt(tagVECTOR vPos, double time);

public:
    Sgp4CalcMethod(char* m_cLine0, char* m_cLine1, char* m_cLine2);
    //Sgp4CalcMethod(SATELLITE* pSat);
    Sgp4CalcMethod();
    ~Sgp4CalcMethod() = default;

    void SetSatellite(char* m_cLine0, char* m_cLine1, char* m_cLine2);
    void SetSatellite(std::string& cLine0, std::string& cLine1, std::string& cLine2);
    //void SetSatellite(SATELLITE* pSat);
    
    bool SGP(double time);
   
    void CalculateLatLonAlt(double time);


    //char* GetString(int iStart, int iEnd, char* cLine);
    double      RadToDeg(double arg) { return (double)(arg / (2.0 * PI) * 360.0); }
    double      DegToRad(double arg) { return (double)(arg / 360.0 * (2.0 * PI)); }
    double      JulianDate(tm st);
    double      JulianDate(double st);
    double      JulianDateOfYear(int yr);
    int         EpocheYear(int iYear);
    int         DayOfYear(int, int, int);
    double      FractionOfDay(int, int, int);
    double      ThetaG(double jd);
    tm          CalendarDate(double dJulian);
    double      SideralTime(double jd);
    

    double          GetFloat(int iStart, int iEnd, char* cLine);
    long            GetInt(int iStart, int iEnd, char* cLine);
    std::string     GetString(int iStart, int iEnd, char* cLine);
    tagVECTOR          GetPos() const{ return m_vPOS; }
    tagVECTOR          GetVel() const { return m_vVEL; }
    double          GetLat() const;
    double          GetLon() const;
    double          GetAlt() const;
    std::string     GetSatName() const;
    double          GetTime() const { return m_fTime; }
    tagVECTOR	        GetUserPos(void) { return m_vUPos; }
    tagVECTOR	        GetUserVel(void) { return m_vUVel; }
    tagVECTOR	        GetObserver(void) { return m_vObs; }
    tagVECTOR	        GetRADec(void) { return m_vRad; }
    tagSATELLITE       GetSatellite() const;
    int             GetNORAD() const;
    
   
 

   
};


#endif //SGPSDP_H
