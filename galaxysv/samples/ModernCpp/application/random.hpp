#ifndef RANDOM_HPP_
#define RANDOM_HPP_

#include <iostream>
#include <random>

void test_random() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 6);
    int dice_roll = distribution(generator);  // generates number in the range 1..6

    auto dice = std::bind(distribution, generator);
    int wisdom = dice() + dice() + dice();

}

#endif /* RANDOM_HPP_ */
