#include <iostream>
#include <exception>
#include <cassert>
#include "mesh_slicer.h"

#ifdef ENABLE_FMT
#include <fmt/format.h>
#include <fmt/color.h>
#define GREEN_STYLE(x) fmt::styled(x, fmt::fg(fmt::color::green))
#define BLUE_STYLE(x) fmt::styled(x, fmt::fg(fmt::color::blue))
#define YELLOW_STYLE(x) fmt::styled(x, fmt::fg(fmt::color::yellow))
#define RED_STYLE(x) fmt::styled(x, fmt::fg(fmt::color::red))
#define TRUE_GREEN_OR_RED_STYLE(x) fmt::styled(x, fmt::fg(x ? fmt::color::green : fmt::color::red))
#endif // ENABLE_FMT

namespace PMP = CGAL::Polygon_mesh_processing;

#ifdef ENABLE_FMT
static constexpr const char *info_format = "faces: {}\n"
                                           "vertices: {}\n"
                                           "edges:{}, halfedges: {}\n"
                                           "valid: {},\n";
#endif // ENABLE_FMT

mesh_slicer::mesh_slicer(const std::string& filename) : filename_(filename) 
{
    
    if (!PMP::IO::read_polygon_mesh(filename_, mesh_) || CGAL::is_empty(mesh_) || !CGAL::is_triangle_mesh(mesh_))
    {
#ifdef ENABLE_FMT
        fmt::print(fmt::fg(fmt::color::red), "Invalid input file: {}", filename_);
#endif // ENABLE_FMT
        throw std::invalid_argument("Invalid input file:" + filename_);
    }
#ifdef ENABLE_FMT
    fmt::print(info_format, BLUE_STYLE(mesh_.num_faces()), 
                            BLUE_STYLE(mesh_.num_vertices()), 
                            BLUE_STYLE(mesh_.num_edges()), BLUE_STYLE(mesh_.num_halfedges()),
                            TRUE_GREEN_OR_RED_STYLE(mesh_.is_valid()));
#endif // ENABLE_FMT
}

void mesh_slicer::get_intersection_polylines(const plane_type &plane_func, Polylines &result_ref)
{
    CGAL::Polygon_mesh_slicer<Mesh, K> slicer(mesh_);
    result_ref.clear();
    slicer(K::Plane_3(plane_func[0], plane_func[1], plane_func[2], plane_func[3]), std::back_inserter(result_ref));
#ifdef ENABLE_FMT
    fmt::print("Plane Function: {}x + {}y + {}z + {} = 0\n", YELLOW_STYLE(plane_func[0]),
                                                             YELLOW_STYLE(plane_func[1]),
                                                             YELLOW_STYLE(plane_func[2]),
                                                             YELLOW_STYLE(plane_func[3]));
    fmt::print("the slicer intersects {} polyline(s).\n", TRUE_GREEN_OR_RED_STYLE(result_ref.size()));
#endif // ENABLE_FMT
}

void mesh_slicer::save_intersection_polylines(const plane_type &plane_func,
                                              const std::string &filename,
                                              const std::string &seperator)
{
    Polylines polylines;
    get_intersection_polylines(plane_func, polylines);

    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
#ifdef ENABLE_FMT
        fmt::print(stderr, "can not open {}\n", filename);
#endif // ENABLE_FMT
        return;
    }

    for (const auto& polyline : polylines) {
        std::size_t i = 0;
        ofs << '#' << i << '\n';
        for (const auto& point: polyline) {
            ofs << point.x() << seperator
                << point.y() << seperator
                << point.z() << '\n';
            ++i;
        }
    }

    ofs.close();
}

#ifdef ENABLE_BOOST_PYTHON
py::list mesh_slicer::get_intersection_polylines_py(double a, double b, double c, double d)
{
    Polylines polylines;
    get_intersection_polylines({a, b, c, d}, polylines);

    py::list py_list;

    np::initialize();
    
    for (const auto& polyline : polylines) {
        std::size_t i = 0;
        py::tuple shape = py::make_tuple(polyline.size(), 3);
        np::dtype type = np::dtype::get_builtin<double>();
        np::ndarray np_polyline = np::zeros(shape, type);
        for (const auto& point: polyline) {
            np_polyline[i][0] = point.x();
            np_polyline[i][1] = point.y();
            np_polyline[i][2] = point.z();
            ++i;
        }

        py_list.append(np_polyline);
    }
    polylines.clear();

    return py_list;
}

np::ndarray mesh_slicer::get_original_vertices_py() const
{
    np::initialize();

    py::tuple shape = py::make_tuple(mesh_.number_of_vertices(), 3);
    np::dtype type = np::dtype::get_builtin<float>();
    np::ndarray np_vertices = np::zeros(shape, type);

    for (const auto& vert_idx : mesh_.vertices()) {
        assert(vert_idx.idx >= 0 && vert_idx.idx < mesh_.number_of_vertices());
        const auto& point = mesh_.point(vert_idx);
        np_vertices[vert_idx.idx()][0] = point.x();
        np_vertices[vert_idx.idx()][1] = point.y();
        np_vertices[vert_idx.idx()][2] = point.z();
    }

    return np_vertices;
}

np::ndarray mesh_slicer::get_original_faces_py() const
{
    np::initialize();

    py::tuple shape = py::make_tuple(mesh_.number_of_faces(), 3);
    np::dtype type = np::dtype::get_builtin<uint32_t>();
    np::ndarray np_vertices = np::zeros(shape, type);

    for (auto& face_idx : mesh_.faces()) {
        assert(face_idx.idx >= 0 && face_idx.idx < mesh_.number_of_faces());
        auto hedge_idx = mesh_.halfedge(face_idx);
        auto vert_idxs = mesh_.vertices_around_face(hedge_idx);
        assert(vert_idxs.size() == 3);
        int i = 0;
        for (const auto& v_idx : vert_idxs) {
            np_vertices[face_idx.idx()][i++] = v_idx.idx();
        }

        // for (int i = 0; i < 3; ++i) {
        //     auto point_idx = mesh_.vertex(mesh_.edge(hedge_idx), 0);
        //     np_vertices[face_idx.idx()][i] = point_idx.idx();
        //     hedge_idx = mesh_.next(hedge_idx);
        // }
    }

    return np_vertices;
}

void mesh_slicer::save_intersection_polylines_py(double a, double b, double c, double d,
                                                 const std::string &filename,
                                                 const std::string &seperator)
{
    save_intersection_polylines({a, b, c, d}, filename, seperator);
}

#endif // ENABLE_BOOST_PYTHON

#ifdef ENABLE_BOOST_PYTHON
using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(save_intersection_polylines_py_ol, mesh_slicer::save_intersection_polylines_py, 5, 6)

BOOST_PYTHON_MODULE(libmesh_slicer)
{
    class_<mesh_slicer>("mesh_slicer", boost::python::init<const ::std::string &>())
        .def("get_intersection_polylines", &mesh_slicer::get_intersection_polylines_py)
        .def("get_original_vertices", &mesh_slicer::get_original_vertices_py)
        .def("get_original_faces", &mesh_slicer::get_original_faces_py)
        .def("save_intersection_polylines", &mesh_slicer::save_intersection_polylines_py);
}
#endif // ENABLE_BOOST_PYTHON