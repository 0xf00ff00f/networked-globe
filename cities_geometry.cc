#include "cities_geometry.h"

#include "geometry.h"
#include "cities.h"

#include <algorithm>
#include <cmath>

namespace
{
glm::vec3 to_position(float latitude, float longitude)
{
    const auto y = std::sin(latitude);
    const auto r = std::cos(latitude);
    const auto z = r * std::sin(longitude);
    const auto x = - r * std::cos(longitude);
    return {x, y, z};
}
}

std::unique_ptr<geometry> build_cities_geometry()
{
    const auto &cities = get_cities();
    std::vector<std::tuple<glm::vec3>> verts;
    verts.reserve(std::size(cities));
    std::transform(std::begin(cities), std::end(cities), std::back_inserter(verts), [](const auto &city) {
        return to_position(city.latitude, city.longitude);
    });
    auto g = std::make_unique<geometry>();
    g->set_data(verts);
    return g;
}
