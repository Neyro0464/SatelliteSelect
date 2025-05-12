#ifndef FILETXTREADER_H
#define FILETXTREADER_H

#include "IReader.h"
#include <string>
#include <fstream>



class FileTxtReader : public IReader {
public:
	FileTxtReader() = default;
    ~FileTxtReader() = default;

    dataFrame GetNextData() override;

private:
    bool SetReaderSrc(const std::string&) override;
	int read() override;
	std::ifstream file;
};

#endif /*FILETXTREADER_H*/
