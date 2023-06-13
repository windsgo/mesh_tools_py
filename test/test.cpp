#include <iostream>
#ifdef ENABLE_FMT
#include <fmt/format.h>
#include <fmt/color.h>
#endif // ENABLE_FMT
#include <unordered_set>

#include <CGAL/HalfedgeDS_items_2.h>
#include <CGAL/HalfedgeDS_vector.h>
#include <CGAL/HalfedgeDS_default.h>
#include <CGAL/HalfedgeDS_decorator.h>

// #include <CGAL/IO/PLY.h>
// #include <CGAL/IO/STL.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/boost/graph/IO/polygon_mesh_io.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#ifdef ENABLE_CGAL_QT5
#include <CGAL/draw_polyhedron.h>
#endif // ENABLE_CGAL_QT5

#include <CGAL/Polygon_mesh_slicer.h>
#include <CGAL/Surface_mesh.h>

#include <cassert>
#include <vector>

#ifdef ENABLE_FMT
#define GREEN_STYLE(x) fmt::styled(x, fmt::fg(fmt::color::green))
#define BLUE_STYLE(x) fmt::styled(x, fmt::fg(fmt::color::blue))
#define YELLOW_STYLE(x) fmt::styled(x, fmt::fg(fmt::color::yellow))
#define RED_STYLE(x) fmt::styled(x, fmt::fg(fmt::color::red))
#define TRUE_GREEN_OR_RED_STYLE(x) fmt::styled(x, fmt::fg(x ? fmt::color::green : fmt::color::red))
#endif // ENABLE_FMT

// struct Traits { typedef int Point_3; };
// typedef CGAL::HalfedgeDS_default< Traits, CGAL::HalfedgeDS_items_3> HDS;
// typedef CGAL::HalfedgeDS_decorator<HDS>  Decorator;

const char* filename = "GDYS.STL";
typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron_3;

static void test_poly() {
#ifdef ENABLE_FMT
    constexpr const char *info_format = "faces: {}\n"
                                        "vertices: {}\n"
                                        "edges:{},         halfedges: {}\n"
                                        "border-edges: {}, border-halfedges: {}\n"
                                        "non-border halfedges: {}\n"
                                        "valid: {},        normalized_border_is_valid: {}\n";
#endif // ENABLE_FMT
    Polyhedron_3 poly;
    // CGAL::Polygon_mesh_processing::IO::read_polygon_mesh(filename, p); /* 此函数有矫正网格功能，但读取较慢 */
    CGAL::IO::read_polygon_mesh(filename, poly); /* boost/graph */
    poly.normalize_border(); /* 此函数可以计算并分类出border halfedges，在此前，这个数目是0 */
#ifdef ENABLE_FMT
    fmt::print(info_format, BLUE_STYLE(poly.size_of_facets()),
               BLUE_STYLE(poly.size_of_vertices()),
               BLUE_STYLE(poly.edges().size()), BLUE_STYLE(poly.size_of_halfedges()),
               YELLOW_STYLE(poly.size_of_border_edges()), YELLOW_STYLE(poly.size_of_border_halfedges()),
               GREEN_STYLE(poly.size_of_halfedges() - poly.size_of_border_halfedges()),
               TRUE_GREEN_OR_RED_STYLE(poly.is_valid()), TRUE_GREEN_OR_RED_STYLE(poly.normalized_border_is_valid()));

    fmt::print("{}\n", poly.vertices_begin()->point().x().exact().get_d());
#endif // ENABLE_FMT
#ifdef ENABLE_CGAL_QT5
    CGAL::draw(poly);
#endif // ENABLE_CGAL_QT5
}

typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
typedef CGAL::Surface_mesh<K::Point_3>                        Mesh;

typedef std::vector<K::Point_3>                               Polyline_type;
typedef std::list<Polyline_type>                              Polylines;

namespace PMP = CGAL::Polygon_mesh_processing;

static void test_mesh() {
#ifdef ENABLE_FMT
    constexpr const char *info_format = "faces: {}\n"
                                        "vertices: {}\n"
                                        "edges:{},         halfedges: {}\n"
                                        "valid: {},\n";
#endif // ENABLE_FMT
    Mesh mesh;
    if (!PMP::IO::read_polygon_mesh(filename, mesh) || CGAL::is_empty(mesh) || !CGAL::is_triangle_mesh(mesh))
    {
        std::cerr << "Invalid input." << std::endl;
        return;
    }
#ifdef ENABLE_FMT
    fmt::print(info_format, BLUE_STYLE(mesh.num_faces()), 
                            BLUE_STYLE(mesh.num_vertices()), 
                            BLUE_STYLE(mesh.num_edges()), BLUE_STYLE(mesh.num_halfedges()),
                            TRUE_GREEN_OR_RED_STYLE(mesh.is_valid()));
#endif // ENABLE_FMT
    // Slicer constructor from the mesh
    CGAL::Polygon_mesh_slicer<Mesh, K> slicer(mesh);
    Polylines polylines;
    slicer(K::Plane_3(0, 0, 1, -354), std::back_inserter(polylines));
    std::cout << "At z = P354, the slicer intersects "
              << polylines.size() << " polylines, fisrt has points: " << polylines.begin()->size() << std::endl;

    std::ofstream ofs("output.txt");

    int i = 0;
    for (auto& polyline : polylines) {
        ofs << '#' << i++ << '\n';
        for (auto& point : polyline) {
            ofs << point.x() << ','
                << point.y() << ','
                << point.z()
                << '\n';
        }
    }
    polylines.clear();

    ofs.close();
}

int main( [[maybe_unused]] int argc, char** argv) {

    test_poly();
    test_mesh();

    return 0;
}