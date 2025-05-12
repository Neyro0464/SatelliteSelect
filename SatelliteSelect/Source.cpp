#include<string>
#include <vector>
#include <iostream>

#include "SatelliteSelect.h"


int main() {
    FileTxtReader Reader;
    StationClass::StationParams st;
    st.geo.Lat = 56.14717;
    st.geo.Lon = 37.76275;
    st.geo.Alt = 100;

    st.lim.minAzm = 120.0;
    st.lim.maxAzm = 200.0;
    st.lim.minElv = 10.0;
    st.lim.maxElv = 80.0;
    st.lim.timeMinObserveSec = 5;

    SatelliteSelect a (st);
    const std::string file = "C:\\Qt\\Projects\\untitled\\TLE.txt";
    a.SetReadModel(&Reader, file);
    a.DataPrepare();
    a.Solve();
    auto b = a.GetSatArray();
    a.ShowResult(b);
    std::cout << "\n\n\nResult 2 :: \n";
    a.Solve();
    a.ShowResult();

    return 1;
}
