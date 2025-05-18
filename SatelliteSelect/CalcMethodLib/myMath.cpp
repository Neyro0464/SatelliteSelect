#include "Sgp4CalcMethod.h" 
#include "myVector.h"

//double Sgp4CalcMethod::sqr (double arg)
//{
//	return arg*arg;
//}

//double Sgp4CalcMethod::RadToDeg (double arg)
//{
//	return (double)(arg/(2.0*PI)*360.0);	
//}
//
//double Sgp4CalcMethod::DegToRad (double arg)
//{	
//	return (double)(arg/360.0*(2.0*PI));
//}

double Sgp4CalcMethod::Fmod2p (double arg)
{
	double modu, ret;
	double twopi = 2.0*PI;
	modu = arg - (int)(arg/twopi) * twopi;
	if (modu >= 0.0)
		ret = modu;
	else
		ret = modu + twopi;
	return ret;
}

double Sgp4CalcMethod::Modulus(double arg1, double arg2)
{
	double modu;
	modu = arg1 - (long)(arg1/arg2) * arg2;
	if (modu >= 0.0)	return modu;
	else			return modu + arg2;
}

double Sgp4CalcMethod::AcTan(double sinx, double cosx)
{
	// The AcTan - bug. Here some e-mail excerpts ...

// The "AcTan bug" was introduced in Dr. Kelso's translation of the
// original FORTRAN code to his Pascal version. The code has a
// two-argument arctangent function, which returns a value from 0( to
// 360( in the FORTRAN version, but goes from -90( to 270( in the
// Pascal version. It may have been that this change was made so that
// AcTan could be used when determining latitude, && because some
// SatteliteModel1 cases may have shown absolutely no effect from the change.
// Indeed, sgp4 results appear to not be affected at all by this
// change. And some sdp4 cases are also not affected (probably because
// the calls to AcTan were not in the fourth quadrant, which is the
// only place that there is a difference). However, sdp4 results can
// be affected ...

// But this ACTAN function is completely unnecessary. I never coded it. As you 
// pointed out, every decent programming language has a 2-argument arctangent 
// function that is quadrant-preserving. For my FORTRAN, it's ATAN2(Y,X), where 
// Y is the sine of the angle, && X is the cosine. It returns values from -pi 
// to +pi. A simple change will return values from 0 to 2*pi: 
// Angle = Pi - ATAN2(-Y,-X) 

// atan2 definnition in C++ :
// atan2 returns a value in the range -pi to +pi radians, 
// using the signs of both parameters to determine the quadrant 
// of the return value. 
	double ret;

	if ( (sinx == 0.0) && (cosx == 0.0) )
		ret = 270.0;
	else	{
		ret = atan2(sinx, cosx);
		if (ret <= -PI/2.000000001)	// This will adapt this version with the 'ugly' 
			ret += 2.0*PI;			// one below. Both functions return exactly the same values
	}
// the old version self made ...
/*
	if (cosx == 0.0)	{
		if (sinx > 0.0)
			ret = PI/2.0;
		else
			ret = 3.0*PI/2.0;
	}
	else if (cosx > 0.0) {
	// --------- correction to match FORTRAN version ----------}
//		if (sinx > 0)						// Add
			ret = atan(sinx/cosx);
//		else								// Add
//			ret = 2*PI + atan(sinx/cosx);	// Add
	// --------------------------------------------------------}
 	}
	else
		ret = PI + atan(sinx/cosx);
	}
*/
	return ret;

}

long Sgp4CalcMethod::round (double arg)
{
	double fFrac, fInt;
	fFrac = modf(arg, &fInt);
	if (fFrac >= 0.5) fInt ++;
	return (long) fInt;
}

void Sgp4CalcMethod::Magnitude (tagVECTOR *pVector)
{
	pVector->w = sqrt(sqr(pVector->x) + sqr(pVector->y) + sqr(pVector->z));
}

double Sgp4CalcMethod::Dot (tagVECTOR v1, tagVECTOR v2)
{
	double fRet;
	fRet = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return fRet;
}
//////////////// End of conversion of Math - PASCAL routines ////////////////////
double Sgp4CalcMethod::VecDot( double *X, double *Y, int N)
{
 /* Purpose 
	calculate the dot (inner) product of X && Y
    Inputs
	X, Y are the input vectors of length N
 */
  int i;
  double M;
  M = 0.0;;
  for ( i=0; i<N; i++ ) M += X[i] * Y[i];
  return M;
}
/*-----------------------------------------------------------------------*/
void Sgp4CalcMethod::VecCross( double *X, double *Y, double *Z, int N)
{
  /*
    Purpose
	    calculate the vector cross product z of x && y.
    Inputs
	    X && Y of length N
    Output
	    Z of length N
  */
  int j,k,m;
  for( m = 0; m < N ; m++ ) {
    j = (m+1)%N; k = (m+2)%N;
    Z[m] = X[j] * Y[k] - X[k] * Y[j];
  }
}
/*-----------------------------------------------------------------------*/
double Sgp4CalcMethod::VecMag( double *X, int N)
{
/*  
   Purpose
	   calculate the magnitude of the vector X of length N
   Input 
	   X
   returns the magnitude of X
*/
   return sqrt( VecDot( X, X, N ));
}
/*------------------------------------------------------------------------*/
void Sgp4CalcMethod::UnitVec( double *X, double *Y, int N )
{
  /*
     Purpose
	     calculate a unit vector in the direction of the input
	     vector
     Input 
	     X, a vector
	     N  the length of X && Y
     Output
	     Y, a unit vector in the direction of X
  */
  int i;
  double M;

  M = VecMag( X, N );
  if (M == 0.0) for (i=0;i<N;i++) Y[i] = 0.0;
  else          for (i=0;i<N;i++) Y[i] = X[i]/M;
}
/*-----------------------------------------------------------------------*/
void Sgp4CalcMethod::VecDiff( double *X, double *Y, double *Z, int N)
{
/*  
   Purpose
	    Calculate the vector difference of the input vectors
   Input
	    X, Y are vectors of length N
   Output  
	    Z = X - Y
*/
  int i;
  for (i=0;i<N;i++) Z[i] = X[i] - Y[i];
}
/*-----------------------------------------------------------------------*/
void Sgp4CalcMethod::VecSum( double *X, double *Y, double *Z, int N)
{
  /* purpose sum of two vectors */
  /* input X && Y are vectors of length N
     output Z another vector of length N
  */
  int i;
  for (i=0;i<N;i++) Z[i] = X[i] + Y[i];
}
/*-----------------------------------------------------------------------*/
void Sgp4CalcMethod::VecScale( double u, double *X, double *Y, int N)
{
/*
  Purpose
	  scale a vector
  Input
	  u: a scalar
	  X: the vector to be scaled 
	  N: the length of X
  Output
	  Y: the scaled vector
*/
  int i;
  for(i=0;i<N;i++)Y[i] = X[i] * u;
}

//////////////////////////////////////////////////////////////////////////////
///////////////////Construction area for a CVector class /////////////////////
//////////////////////////////////////////////////////////////////////////////
CVector::CVector() 
{
	m_iDepth = 3;
	m_vector.x = m_vector.y = m_vector.z = m_vector.w = 0.0;
	return;
}

CVector::CVector(tagVECTOR vIn) 
{
	m_iDepth = 3;
	SetVector(vIn);
	return;
}


tagVECTOR CVector::GetVector()
{
	return m_vector;
}

void CVector::SetVector(tagVECTOR vIn)
{
	m_vector.x = vIn.x;
	m_vector.y = vIn.y;
	m_vector.z = vIn.z;
	m_vector.w = vIn.w;
}

int CVector::GetDepth()
{
	return m_iDepth;
}

void CVector::SetDepth(int iDepth)
{
	m_iDepth = iDepth;
}

double CVector::Dot( tagVECTOR vIn )
{
 /* Purpose 
	calculate the dot (inner) product of X && Y
    Inputs
	X, Y are the input vectors of length m_iDepth
 */
	double *X = (double *)&m_vector;
	double *Y = (double *)&vIn;

	int i;
	double M;
  
	M = 0.0;;
	for ( i=0; i<m_iDepth; i++ ) M += X[i] * Y[i];
	return M;
}
/*-----------------------------------------------------------------------*/
tagVECTOR CVector::Cross( tagVECTOR vIn )
{
  /*
    Purpose
	    calculate the vector cross product z of x && y.
    Inputs
	    m_vector && vIn of length m_iDepth
    Output
	    Z of length m_iDepth
  */
	tagVECTOR vector = {};
	double *X = (double *)&m_vector;
	double *Y = (double *)&vIn;
	double *Z = (double *)&vector;
	
	int j,k,m;
	for( m = 0; m < m_iDepth ; m++ ) {
		j = (m+1)%m_iDepth; k = (m+2)%m_iDepth;
		Z[m] = X[j] * Y[k] - X[k] * Y[j];
	}
	return vector;
}
/*-----------------------------------------------------------------------*/
double CVector::Mag()
{
/*  
   Purpose
	   calculate the magnitude of the vector X of length m_iDepth
   Input 
	   X
   returns the magnitude of X
*/
	return sqrt( Dot( m_vector ));
}
/*------------------------------------------------------------------------*/
tagVECTOR CVector::Unit()
{
  /*
     Purpose
	     calculate a unit vector in the direction of the input
	     vector
     Input 
	     X, a vector
	     m_iDepth  the length of X && Y
     Output
	     Y, a unit vector in the direction of X
  */
	static tagVECTOR vector;
	double *X = (double *)&m_vector;
	double *Y = (double *)&vector;
	
	int i;
	double M;

	M = Mag( );
	if (M == 0.0) for (i=0;i<m_iDepth;i++) Y[i] = 0.0;
	else          for (i=0;i<m_iDepth;i++) Y[i] = X[i]/M;
	return vector;
}
/*-----------------------------------------------------------------------*/
tagVECTOR CVector::Diff(tagVECTOR vIn )
{
/*  
   Purpose
	    Calculate the vector difference of the input vectors
   Input
	    X, Y are vectors of length m_iDepth
   Output  
	    Z = X - Y
*/
	static tagVECTOR vector;
	double *X = (double *)&m_vector;
	double *Y = (double *)&vIn;
	double *Z = (double *)&vector;
	
	int i;
	for (i=0;i<m_iDepth;i++) Z[i] = X[i] - Y[i];
	return vector;
}
/*-----------------------------------------------------------------------*/
tagVECTOR CVector::Sum(tagVECTOR vIn )
{
  /* purpose sum of two vectors */
  /* input X && Y are vectors of length m_iDepth
     output Z another vector of length m_iDepth
  */
	static tagVECTOR vector;
	double *X = (double *)&m_vector;
	double *Y = (double *)&vIn;
	double *Z = (double *)&vector;
	
	int i;
	for (i=0;i<m_iDepth;i++) Z[i] = X[i] + Y[i];
	return vector;
}
/*-----------------------------------------------------------------------*/
tagVECTOR CVector::Scale( double u )
{
/*
  Purpose
	  scale a vector
  Input
	  u: a scalar
	  X: the vector to be scaled 
	  m_iDepth: the length of X
  Output
	  Y: the scaled vector
*/
	static tagVECTOR vector;
	double *X = (double *)&m_vector;
	double *Y = (double *)&vector;
	
	int i;
	for(i=0;i<m_iDepth;i++)Y[i] = X[i] * u;
	return vector;
}


void Sgp4CalcMethod::CalculateLatLonAlt(double jdTime)
{
	m_vLLA = CalculateLatLonAlt(m_vPOS, jdTime);
	m_bLatLonAlt = true;
}

tagVECTOR Sgp4CalcMethod::CalculateLatLonAlt(tagVECTOR vPOS, double time)
{
	// Reference:  The 1992 Astronomical Almanac, page K12. 
	static tagVECTOR vLLA;
	double lat, lon, alt;
	double theta, r, e2, phi, c;
	double arg1, arg2;

	vLLA.x = vLLA.y = vLLA.z = vLLA.w = 0.0;
	lat = lon = alt = 0.0;
	theta = r = e2 = phi = c = 0.0;

	//	theta = atan2(vPOS.y,vPOS.x);
	theta = AcTan(vPOS.y, vPOS.x);

	arg1 = ThetaG(time);
	arg1 = theta - arg1;
	arg2 = 2.0 * PI;

	//	lon = Modulus(theta - ThetaG(time),2.0*PI);
	lon = Modulus(arg1, arg2);

	r = sqrt(sqr(vPOS.x) + sqr(vPOS.y));
	e2 = f * (2.0 - f);
	lat = AcTan(vPOS.z, r);
	do {
		phi = lat;
		c = 1.0 / sqrt(1.0 - e2 * sqr(sin(phi)));
		lat = AcTan(vPOS.z + xkmper * c * e2 * sin(phi), r);
	} while (fabs(lat - phi) > 1E-10);//1E-7); For speeding up calculation 7 digit
	//is exact enough (123.45
	alt = r / cos(lat) - xkmper * c;

	vLLA.x = lat * 180.0 / PI;   // radians
	vLLA.y = lon * 180.0 / PI;   // radians
	vLLA.z = alt;			// kilometers
	vLLA.w = theta * 180.0 / PI; // radians
	return vLLA;
}


