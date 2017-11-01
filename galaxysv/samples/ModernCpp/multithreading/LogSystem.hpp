#ifndef SRC_MULTITHREADING_LOGSYSTEM_HPP_
#define SRC_MULTITHREADING_LOGSYSTEM_HPP_

#include <iostream>
#include <fstream>
#include <mutex>

namespace application {

class LogSystem {
    std::ofstream _ofs;
    std::string _filename;
    std::once_flag _flag;
public:
    LogSystem(std::string filepath) : _filename(filepath) {
        std::call_once(_flag, [this, filepath]() {
            _ofs.open(filepath, std::ofstream::out);
            _ofs << "logging starting...";
        });
    }

    ~LogSystem() {
        _ofs.close();
    }

    void log(const std::string msg) {
        _ofs << std::chrono::system_clock::now();
        _ofs << msg;
    }

    LogSystem(const LogSystem & copy) = delete;
    LogSystem & operator= (const LogSystem & copy) = delete;
};

} /* namespace application */

#endif /* SRC_MULTITHREADING_LOGSYSTEM_HPP_ */
