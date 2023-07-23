#include "Value.h"
#include <string>
#include <unordered_map>

class Registry {
public:
    Value& operator[](const std::string& ident);

private:
    std::unordered_map<std::string, Value> m_vars;
};