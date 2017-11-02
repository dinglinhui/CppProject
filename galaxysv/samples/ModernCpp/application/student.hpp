#ifndef SRC_STUDENT_HPP_
#define SRC_STUDENT_HPP_

#include <iostream>
#include <valarray>
#include <string>

namespace application {

class Student: private std::string, private std::valarray<double> {
private:
    typedef std::valarray<double> ArrayDb;
    std::ostream & arr_out(std::ostream & os) const;

public:
    Student() :
            std::string("Null Student"), ArrayDb() {
    }

    Student(const std::string & s) :
            std::string(s), ArrayDb() {
    }

    Student(int n) :
            std::string("Nully"), ArrayDb(n) {
    }

    Student(const std::string & s, int n) :
            std::string(s), ArrayDb(n) {
    }

    Student(const std::string & s, const ArrayDb & a) :
            std::string(s), ArrayDb(a) {
    }

    Student(const char * str, const double * pd, int n) :
            std::string(str), ArrayDb(pd, n) {
    }

    ~Student() {
    }

    double Average() const;
    double & operator[](int i);
    double operator[](int i) const;
    const std::string & Name() const;

    friend std::istream & operator>>(std::istream & is, Student & stu);
    friend std::istream & getline(std::istream & is, Student & stu);
    friend std::ostream & operator<<(std::ostream & os, const Student & stu);
};

double Student::Average() const {
    if (ArrayDb::size() > 0)
        return ArrayDb::sum() / ArrayDb::size();
    else
        return 0;
}

const std::string & Student::Name() const {
    return (const std::string &) *this;
}

double & Student::operator[](int i) {

    return ArrayDb::operator[](i);
}

double Student::operator[](int i) const {
    return ArrayDb::operator[](i);
}

std::ostream & Student::arr_out(std::ostream & os) const {
    size_t i;
    size_t lim = ArrayDb::size();
    if (lim > 0) {
        for (i = 0; i < lim; i++) {
            os << ArrayDb::operator[](i) << " ";
            if (i % 5 == 4)
                os << std::endl;
        }
        if (i % 5 != 0)
            os << std::endl;
    } else
        os << " empty array";
    return os;
}

std::istream & operator>>(std::istream & is, Student & stu) {
    is >> (std::string &) stu;
    return is;
}

std::istream & getline(std::istream & is, Student & stu) {
    getline(is, (std::string &) stu);
    return is;
}

std::ostream & operator<<(std::ostream & os, const Student & stu) {
    os << "Scores for " << (const std::string &) stu << ":\n";
    stu.arr_out(os);
    return os;
}

void set(Student & sa, int n) {
    std::cout << "Please enter the student's name: ";
    getline(std::cin, sa);
    std::cout << "Please enter " << n << " quiz scores: \n";
    for (int i = 0; i < n; i++)
        std::cin >> sa[i];
    while (std::cin.get() != '\n')
        continue;
}

const int pupils = 3;
const int quizzes = 5;

void test_student() {
    Student ada[pupils] = { Student(quizzes), Student(quizzes), Student(quizzes) };
    int i;
    for (i = 0; i < pupils; i++)
        set(ada[i], quizzes);
    std::cout << "\nStudent List: \n ";
    for (i = 0; i < pupils; ++i)
        std::cout << ada[i].Name() << std::endl;
    std::cout << "\nResults :";
    for (i = 0; i < pupils; i++) {
        std::cout << std::endl << ada[i];
        std::cout << "average : " << ada[i].Average() << std::endl;
    }
}
}
#endif /* SRC_STUDENT_HPP_ */
