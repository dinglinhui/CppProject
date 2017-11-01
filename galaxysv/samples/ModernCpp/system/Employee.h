#ifndef SRC_SYSTEM_EMPLOYEE_H_
#define SRC_SYSTEM_EMPLOYEE_H_

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

namespace system {

class Employee {
    using ID = std::pair<std::string, std::string>; // EmployeeID and ID-Card Number
    using Name = std::tuple<std::string, std::string, std::string>; // First/Middle/Last Name
    using DateOfBirth = std::tuple<size_t, size_t, size_t>;

public:
    explicit Employee(ID id);
    virtual ~Employee();

private:
    ID _id;
    Name _name;
    DateOfBirth _dob;
};

} /* namespace system */

#endif /* SRC_SYSTEM_EMPLOYEE_H_ */
