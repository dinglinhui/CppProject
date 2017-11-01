#ifndef SRC_APPLICATION_UTILS_HPP_
#define SRC_APPLICATION_UTILS_HPP_
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <string>
#include <regex>
#include <boost/algorithm/string/split.hpp>

namespace utils {

class StringUtils {
public:
    static std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

    template<typename T>
    static std::string to_string(T nVal) {
        std::ostringstream os;
        os << nVal;
        return os.str();
    }

private:
    StringUtils() {
    }

    template<typename Out>
    static void split(const std::string &s, char delim, Out result) {
        std::stringstream ss;
        ss.str(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            *(result++) = item;
        }
    }
};

std::string largestNumber(std::vector<int> &num) {
    std::vector<std::string> rec;

    for (auto i : num)
        rec.push_back(StringUtils::to_string<int>(i));
//            rec.push_back(std::to_string(i));

    std::sort(std::begin(rec), std::end(rec), [](std::string a, std::string b) {
        return a + b > b + a;
    });

    return std::accumulate(std::begin(rec), std::end(rec), std::string(""));
}

void test_split() {
    std::string res("Rat,Ox,Tiger,Rabbit,Dragon,Snake,Horse,Goat,Monkey,Rooster,Dog,Pig");

    std::vector<std::string> x = StringUtils::split(res, ',');
    for (auto i : x)
        std::cout << i << " ";

    std::vector<std::string> results;
    boost::split(results, res, [](char c) {return c == ',';});
    for (auto i : results)
        std::cout << i << " ";

    std::regex sep("[ ,.]+");
    std::sregex_token_iterator tokens(res.cbegin(), res.cend(), sep, -1);
    std::sregex_token_iterator end;
    for (; tokens != end; ++tokens) {
        std::cout << *tokens << "\n";
    }
}

void test_utils() {
    std::vector<int> b;
    for (auto it : { 122, 21, 344, 47, 50, 69, 97, 86 }) {
        b.push_back(it);
    }
    std::cout << largestNumber(b) << std::endl;
    std::cout << std::to_string(1 + 2 + 3 + 4 + 5) << std::endl;

}
}/*namespace utils*/

#endif /* SRC_APPLICATION_UTILS_HPP_ */
