#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <iostream>
#include <fstream>
#include <mutex>

namespace utils {

class Log {
    std::ofstream _ofs;
    std::string _filename;
    std::once_flag _flag;
public:
    Log(std::string filepath) : _filename(filepath) {
        std::call_once(_flag, [this, filepath]() {
            _ofs.open(filepath, std::ofstream::out);
            _ofs << "logging starting...";
        });
    }

    ~Log() {
        _ofs.close();
    }

    void log(const std::string msg) {
        _ofs << std::chrono::system_clock::now();
        _ofs << msg;
    }

    Log(const Log & copy) = delete;
    Log & operator= (const Log & copy) = delete;
};

} /* namespace utils */

#endif /* _LOG_HPP_ */
