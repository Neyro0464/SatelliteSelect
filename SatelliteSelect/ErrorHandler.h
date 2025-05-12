#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <string>

namespace ErrorHandler 
{
	enum Status {
		SUCCESS = 1,
		FILE_READER = -1,
		STATION = -2,
		CALC_METHOD = -3,
		SOLVER = -4
	};

	struct Error
	{
		Status s;
		std::string errorMessage = "";

		operator bool() const{
			return (s == SUCCESS);
		}
		bool operator!() const {
			return (s != SUCCESS);
		}
	};

}

#endif //ERRORHANDLER_H

