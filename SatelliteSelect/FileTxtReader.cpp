#include "FileTxtReader.h"


int FileTxtReader::read(){
	return 1;
}

bool FileTxtReader::SetReaderSrc(const std::string& src)
{ 
    file = std::ifstream(src);
    if (!file.is_open()) {
        return false;
    }
    return true;
}


FileTxtReader::dataFrame FileTxtReader::GetNextData(){
    std::string name, param1, param2;
    
    std::getline(file, name);
    std::getline(file, param1);
    std::getline(file, param2);
    
    return dataFrame{ std::move(name), std::move(param1), std::move(param2) };
}


