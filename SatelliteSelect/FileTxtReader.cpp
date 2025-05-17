#include "FileTxtReader.h"

FileTxtReader::FileTxtReader(const std::string& src){
    if(src.substr(src.size() - 3) != "txt"){
        throw std::invalid_argument("Wrong file extension");
    }
    file = std::ifstream(src);
    if(!file.is_open()){
        file.clear();
        throw std::runtime_error("Could not open file");
    }
}

bool FileTxtReader::SetReaderSrc(const std::string& src)
{
    if(src.substr(src.size() - 3) != "txt"){
        return false;
    }
    file = std::ifstream(src);
    if (!file.is_open()) {
        file.clear();
        return false;
    }
    return true;
}

FileTxtReader::dataFrame FileTxtReader::GetFrame(){
    std::string name, param1, param2;
    
    std::getline(file, name);
    std::getline(file, param1);
    std::getline(file, param2);
    
    return dataFrame{ std::move(name), std::move(param1), std::move(param2) };
}

// Prototype

std::vector<FileTxtReader::dataFrame> FileTxtReader::GetFewFrames(const std::size_t frameQuantity){
    std::vector<dataFrame> result;
    for(std::size_t i = 0; i < frameQuantity; i++)
        // if({GetFrame()})
        result.push_back({GetFrame()});
    return result;

}


