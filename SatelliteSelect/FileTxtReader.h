#ifndef FILETXTREADER_H
#define FILETXTREADER_H

#include <string>
#include <fstream>
#include <vector>


#include "IReader.h"



class FileTxtReader : public IReader {
public:
    // FileTxtReader() = default;
    explicit FileTxtReader(const std::string& src);
    ~FileTxtReader() = default;

    bool SetReaderSrc(const std::string&) override;
    dataFrame GetFrame() override;


    //Prototype
    std::vector<dataFrame> GetFewFrames(const std::size_t frameQuantity) override;

private:
	std::ifstream file;
};

#endif /*FILETXTREADER_H*/
