#ifndef _EMPLOYEESYSTEM_H_
#define _EMPLOYEESYSTEM_H_

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

namespace application {
using ID = std::pair<std::string, std::string>; // EmployeeID and ID-Card Number
using Name = std::pair<std::string, std::string>;
using DateOfBirth = std::tuple<size_t, size_t, size_t>;
using Details = std::tuple<DateOfBirth, size_t, size_t, std::string>;
using Employee = std::map<Name, Details>;
using EmployeeType = Employee::value_type;

class EmployeeSystem {
public:
    EmployeeSystem() {
    }

    virtual ~EmployeeSystem() {
    }

    void entry(const EmployeeType& employee) {
        m_employee.emplace(employee);
    }

    void print() {
        DateOfBirth dob;
        std::string occupation;

        std::for_each(std::begin(m_employee), std::end(m_employee), [&](const std::pair<Name, Details>& kv) {
            std::tie(dob, std::ignore, std::ignore, occupation) = kv.second;
            std::cout << std::left << kv.first.first << " " << kv.first.second << ":"
            << std::setfill('0') << std::right
            << std::setw(2) << std::get<0>(dob) << "/"
            << std::setw(2) << std::get<1>(dob) << "/"
            << std::setw(4) << std::get<2>(dob) << " "
            << occupation << std::endl;
        });
    }

    template<typename Compare>
    void sort_print(Compare comp) {
        std::vector<const EmployeeType*> folks;
        for (const auto& pr : m_employee)
            folks.push_back(&pr);

        auto ptr_comp = [&comp](const EmployeeType* ptr1, const EmployeeType* ptr2) -> bool {
            return comp(ptr1, ptr2);
        };
        std::sort(std::begin(folks), std::end(folks), ptr_comp);

        DateOfBirth dob;
        std::string occupation;
        size_t height, weight;
        for (const auto& kv : folks) {
            std::tie(dob, height, weight, occupation) = kv->second;
            std::cout << std::left << kv->first.first << " " << kv->first.second << ":" << std::setfill('0') << std::right << std::setw(2) << std::get<0>(dob) << "/" << std::setw(2)
                    << std::get<1>(dob) << "/" << std::setw(4) << std::get<2>(dob) << " " << height << " " << weight << " " << occupation << std::endl;
        };
    }

private:
    Employee m_employee;
};

void test_employeesystem() {
    EmployeeSystem es;
    es.entry(std::make_pair(std::make_pair("B", "B"), std::make_tuple(std::make_tuple(8, 10, 1987), 181, 62, "Senior Software Engineer")));
    es.entry(std::make_pair(std::make_pair("A", "A"), std::make_tuple(std::make_tuple(8, 10, 1987), 172, 62, "Senior Software Manager")));
    es.entry(std::make_pair(std::make_pair("C", "C"), std::make_tuple(std::make_tuple(8, 10, 1987), 183, 72, "Senior Hardware Engineer")));
    es.print();

    auto fun = [](const EmployeeType* ptr1, const EmployeeType* ptr2) -> bool {
        return std::get<1>(ptr1->second) < std::get<1>(ptr2->second);
    };
    es.sort_print(fun);
}
} /* namespace application */

#endif /* _EMPLOYEESYSTEM_H_ */
