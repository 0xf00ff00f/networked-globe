#include "globe_geometry.h"

#include "geometry.h"
#include "pixmap.h"

#include <glm/glm.hpp>

#include <sstream>
#include <map>
#include <algorithm>

namespace glm
{
bool operator<(const vec3 &a, const vec3 &b)
{
    return std::tie(a.x, a.y, a.z) < std::tie(b.x, b.y, b.z);
}
}

class globe_geometry_builder
{
public:
    globe_geometry_builder(int max_subdivisions, const pixmap &pm)
        : max_subdivisions_(max_subdivisions)
        , pm_(pm)
    {
    }

    std::unique_ptr<geometry> build_geometry()
    {
        initialize_geometry();
        std::cout << (verts_.size() / 3) << " triangles\n";

        auto g = std::make_unique<geometry>();
        g->set_data(verts_);
        return g;
    }

private:
    void initialize_geometry()
    {
        static const glm::vec3 icosahedron_verts[] = {
            {0, -0.525731, 0.850651},  {0.850651, 0, 0.525731},   {0.850651, 0, -0.525731}, {-0.850651, 0, -0.525731},
            {-0.850651, 0, 0.525731},  {-0.525731, 0.850651, 0},  {0.525731, 0.850651, 0},  {0.525731, -0.850651, 0},
            {-0.525731, -0.850651, 0}, {0, -0.525731, -0.850651}, {0, 0.525731, -0.850651}, {0, 0.525731, 0.850651}};
        static const int icosahedron_tris[][3] = {{2, 3, 7},  {2, 8, 3},   {4, 5, 6},   {5, 4, 9},  {7, 6, 12},
                                                  {6, 7, 11}, {10, 11, 3}, {11, 10, 4}, {8, 9, 10}, {9, 8, 1},
                                                  {12, 1, 2}, {1, 12, 5},  {7, 3, 11},  {2, 7, 12}, {4, 6, 11},
                                                  {6, 5, 12}, {3, 8, 10},  {8, 2, 1},   {4, 10, 9}, {5, 9, 1}};

        for (const auto &vertex : icosahedron_verts)
        {
            maybe_add_vertex(vertex);
        }

        for (const auto &triangle : icosahedron_tris)
        {
            const auto i0 = triangle[0] - 1;
            const auto i1 = triangle[1] - 1;
            const auto i2 = triangle[2] - 1;
            subdivide_triangle(i0, i1, i2, 0);
        }

        initialize_dual();
    }

    void subdivide_triangle(int i0, int i1, int i2, int level)
    {
        if (level == max_subdivisions_)
        {
            source_verts_[i0].adjacent_triangles.push_back({i0, i1, i2});
            source_verts_[i1].adjacent_triangles.push_back({i0, i1, i2});
            source_verts_[i2].adjacent_triangles.push_back({i0, i1, i2});
        }
        else
        {
            const auto v0 = source_verts_[i0].position;
            const auto v1 = source_verts_[i1].position;
            const auto v2 = source_verts_[i2].position;

            const auto i01 = maybe_add_vertex(glm::normalize(0.5f * (v0 + v1)));
            const auto i12 = maybe_add_vertex(glm::normalize(0.5f * (v1 + v2)));
            const auto i20 = maybe_add_vertex(glm::normalize(0.5f * (v2 + v0)));

            subdivide_triangle(i0, i01, i20, level + 1);
            subdivide_triangle(i01, i1, i12, level + 1);
            subdivide_triangle(i20, i12, i2, level + 1);
            subdivide_triangle(i01, i12, i20, level + 1);
        }
    }

    int maybe_add_vertex(const glm::vec3 &v)
    {
        auto it = vert_indices_.find(v);
        if (it != vert_indices_.end())
            return it->second;
        auto index = source_verts_.size();
        source_verts_.push_back({v, {}});
        vert_indices_.insert(it, {v, index});
        return index;
    }

    void initialize_dual()
    {
        for (const auto &source_vert : source_verts_)
        {
            std::vector<glm::vec3> verts;
            auto center = glm::vec3(0);
            for (const auto &tri : source_vert.adjacent_triangles)
            {
                const auto vn =
                    glm::normalize((1.0f / 3) * (source_verts_[tri.i0].position + source_verts_[tri.i1].position +
                                                 source_verts_[tri.i2].position));
                center += vn;
                verts.push_back(vn);
            }
            for (size_t i = 1; i < verts.size() - 1; ++i)
            {
                auto it = std::min_element(std::next(verts.begin(), i), verts.end(),
                                           [o = verts[i - 1]](const auto &a, const auto &b) {
                                               return glm::length(a - o) < glm::length(b - o);
                                           });
                std::swap(verts[i], *it);
            }
            center *= (1.0f / verts.size());
            const auto normal = glm::normalize(center);

            // flip verts if wrong orientation
            const auto r = glm::cross(verts[0] - center, verts[1] - center);
            if (glm::dot(r, normal) < 0)
                std::reverse(verts.begin(), verts.end());

            if (map_at(center) == 0)
                continue;

            const float scale = 0.8f;

            for (size_t i = 0; i < verts.size(); ++i)
            {
                const auto &a = verts[i];
                const auto &b = verts[(i + 1) % verts.size()];
                const auto as = center + scale * (a - center);
                const auto bs = center + scale * (b - center);
                verts_.push_back({center, normal});
                verts_.push_back({as, normal});
                verts_.push_back({bs, normal});
            }
        }
    }

    float map_at(const glm::vec3 &v) const
    {
        const auto n = glm::normalize(v);

        const float longitude = (std::atan2(n.z, -n.x) + M_PI) / (2.0 * M_PI);

        const float d = sqrtf(n.x * n.x + n.z * n.z);
        const float latitude = (std::atan2(-n.y, d) + 0.5 * M_PI) / M_PI;

        const int c = static_cast<int>(longitude * pm_.width);
        const int r = static_cast<int>(latitude * pm_.height);

        return static_cast<float>(pm_.data[r * pm_.width + c]) / 255;
    }

    struct triangle
    {
        int i0, i1, i2;
    };
    struct vertex_triangles
    {
        glm::vec3 position;
        std::vector<triangle> adjacent_triangles;
    };
    std::vector<vertex_triangles> source_verts_;
    std::map<glm::vec3, std::size_t> vert_indices_;

    int max_subdivisions_;
    const pixmap &pm_;
    using vertex = std::tuple<glm::vec3, glm::vec3>;
    std::vector<vertex> verts_;
};

std::unique_ptr<geometry> build_globe_geometry(int max_subdivisions, const char *pixmap_path)
{
    auto pm = load_pixmap_from_png(pixmap_path);
    if (pm->type != pixmap::pixel_type::GRAY)
    {
        std::stringstream ss;
        ss << "invalid pixel type in " << pixmap_path;
        throw std::runtime_error(ss.str());
    }
    std::cout << pm->width << ' ' << pm->height << '\n';
    globe_geometry_builder builder(max_subdivisions, *pm);
    return builder.build_geometry();
}
