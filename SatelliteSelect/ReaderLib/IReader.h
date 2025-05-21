#ifndef IREADER_H
#define IREADER_H

#include <string>
#include <vector>
#include <tuple>
#include <optional>


class IReader{
public:
	using dataFrame = std::tuple<std::string, std::string, std::string>;
    virtual std::optional<dataFrame> GetFrame() = 0;
    virtual std::vector<dataFrame> GetFewFrames(const std::size_t frameQuantity) = 0;
    virtual bool SetReaderSrc(const std::string&) = 0 ;
	virtual ~IReader() = default;
};
#endif /*IREADER_H*/
