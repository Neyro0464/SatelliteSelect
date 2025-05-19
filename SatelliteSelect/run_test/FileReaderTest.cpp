// #include <iostream>

// #include "FileTxtReader.h"
// #include "IReader.h"


// int main(){

//     IReader* a = new FileTxtReader("C:\\Qt\\Projects\\SatelliteSelect\\SatelliteSelect\\run_test\\testTLE.txt");
//     auto&& b = a->GetFrame();
//     if(b != std::nullopt)
//     {
//         std::cout << std::get<0>(b.value()) << "\n" <<std::get<1>(b.value()) << "\n" << std::get<2>(b.value());
//     }
//     auto c = a->GetFewFrames(10);
//     for (const auto& item : c) {
//         std::cout << std::get<0>(item) << "\n" << std::get<1>(item) << "\n" << std::get<2>(item) << "\n";
//     }
//     return 1;
// }
