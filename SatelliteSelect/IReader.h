#ifndef IREADER_H
#define IREADER_H

#include <string>
#include <tuple>


class IReader{
public:
	using dataFrame = std::tuple<std::string, std::string, std::string>;

	virtual int read() = 0;
	virtual dataFrame GetNextData() = 0;
	virtual bool SetReaderSrc(const std::string&) = 0;
	virtual ~IReader() = default;
};
#endif /*IREADER_H*/
