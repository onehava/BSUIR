#include "Registry.h"

Value &Registry::operator[](const std::string &ident){
    auto var = m_vars.find(ident);

    if(var == m_vars.end()){
        Value dv(0.0);
        m_vars.emplace(ident, std::move(dv));
    }

    return m_vars.at(ident);
}