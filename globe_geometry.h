#pragma once

#include <memory>

class geometry;
struct pixmap;

std::unique_ptr<geometry> build_globe_geometry(int max_subdivisions, const char *pixmap_path);
