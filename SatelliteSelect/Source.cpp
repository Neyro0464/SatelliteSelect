
// #include<string>
// #include <vector>
#include <iostream>

#include "StationClass.h"


int main() {
    // FileTxtReader Reader("C:\\Qt\\Projects\\SatelliteSelect\\SatelliteSelect\\testTLE.txt");
    StationClass::StationVision vis{};
    StationClass station(0.0, 0.0, 0.0, vis);
    return 1;
}

