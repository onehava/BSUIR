#include <unordered_map>
#include <string>
#include <regex>
#include <iostream>
#include <algorithm>

int main(){
    std::string content(
        R"(Li Europan lingues es membres del sam familie.)"
        R"(Lor separat existentie es un myth.)"
        R"(Por scientie, musica, sport, litot Europa usa li sam vocabular.)"
        R"(Li lingues differe solmen in li grammatica, li pronunciation e li plu commun vocabules.)"
        R"(Omnicos directe al desirabilite de un nov lingua franca:)"
        R"(On refusa continuar payar custosi traductores.)"
        R"(At solmen va esser necessi far uniform grammatica, pronunciation e plu sommun paroles.)"
    );

    std::regex expr("\\w+");

    std::regex_token_iterator<std::string::iterator> begin(content.begin(), content.end(), expr);
    std::regex_token_iterator<std::string::iterator> end;

    std::unordered_map<std::string, std::size_t> counter;

    std::for_each(begin, end, [&counter](const std::string& word){
        ++counter[word];
    });

    auto max_frequent = std::max_element(
        counter.begin(),
        counter.end(),
        [](const std::pair<std::string, std::size_t>& a, const std::pair<std::string, std::size_t>& b){
            return a.second < b.second;
        }
    );

    std::cout << "Max frequent word is `" << max_frequent->first << "` with frequency " << max_frequent->second << std::endl;
}