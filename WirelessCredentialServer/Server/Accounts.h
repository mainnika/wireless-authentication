#include <unordered_map>
#include <string>

#include "NoCopy.h"

class Accounts : private NoCopy
{
private:
	typedef std::unordered_map<std::string, std::string> accounts_t;

	accounts_t accounts;
	accounts_t unregistered;

	void add_unregistered(std::string key);

public:
	Accounts();
	virtual ~Accounts();

	void register_key_for_account(std::string key, std::string account);

	std::string get_by_key(std::string key);
};

