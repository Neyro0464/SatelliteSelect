
// #include<string>
// #include <vector>
#include <iostream>

#include "FileTxtReader.h"


int main() {
    FileTxtReader Reader("C:\\Qt\\Projects\\SatelliteSelect\\SatelliteSelect\\testTLE.txt");
    auto&& [line0, line1, line2] = Reader.GetFrame();
    std::cout<<line0 << "\n" << line1 << "\n" << line2 << "\n";

    return 1;
}

