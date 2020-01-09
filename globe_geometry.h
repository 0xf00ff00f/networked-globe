#pragma once

#include <glm/vec3.hpp>
#include <memory>

#include "geometry.h"

struct pixmap;

using globe_vertex = std::tuple<glm::vec3, glm::vec3, float>;

std::unique_ptr<geometry<globe_vertex>> build_globe_geometry(int max_subdivisions, const char *pixmap_path);
