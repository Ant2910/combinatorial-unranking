/*
g++ -std=c++23 urn.cpp ranges_and_views.cpp -o ranges_test

*/
# include "urn.hpp"
# include <iostream>
# include <cassert>
# include <algorithm>
# include <vector>
# include <ranges>

int main()
{   
    urn::UrnR urn {6, 9};
    std::cout << "Anwendungsbeispiel 1: Welten mit nur Berge- und Schneegebieten" << std::endl;
    auto only_mountain_and_snow = urn | std::views::filter([](const std::vector<uint>& draw) {
        return std::ranges::all_of(draw, [](int value) {
            return value == 4 || value == 5;
        });
    });

    for (auto const& draw : only_mountain_and_snow)
        std::cout << urn::to_string(draw) << std::endl;
    std::cout << "Anzahl der only_mountain_and_snow: " << std::ranges::distance(only_mountain_and_snow) << std::endl;

    std::cout << "Anwendungsbeispiel 2: Welten mit mindestens einem Meer- und maximal zwei Wüstengebieten" << std::endl;
    auto constrained_worlds =
    urn | std::views::filter([](const std::vector<uint>& draw) {
        const auto sea_count =
            std::ranges::count(draw, 2);

        const auto desert_count =
            std::ranges::count(draw, 0);

        return sea_count >= 1 && desert_count <= 2;
    });

    for (auto const& draw : constrained_worlds)
        std::cout << urn::to_string(draw) << std::endl;
    std::cout << "Anzahl der constrained_worlds: " << std::ranges::distance(constrained_worlds) << std::endl;
    return 0;
}