#include <unordered_map>
#include <string>

#include "NoCopy.h"

class Accounts : private NoCopy
{
private:
	typedef std::unordered_map<std::string, std::string> accounts_t;

	static accounts_t accounts;

public:
	Accounts();
	virtual ~Accounts();

	std::string get_by_key(std::string key);
};

