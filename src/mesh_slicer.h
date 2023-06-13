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
#include <CGAL/draw_polyhedron.h>

#include <CGAL/Polygon_mesh_slicer.h>
#include <CGAL/Surface_mesh.h>

#include <vector>

#ifdef ENABLE_BOOST_PYTHON
#include <boost/python.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
#include <boost/python/numpy.hpp>
namespace py = boost::python;
namespace np = boost::python::numpy;
#endif // ENABLE_BOOST_PYTHON

typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
typedef CGAL::Surface_mesh<K::Point_3>                        Mesh;
typedef std::vector<K::Point_3>                               Polyline_type;
typedef std::list<Polyline_type>                              Polylines;

class mesh_slicer {
private:
    std::string filename_;
    Mesh mesh_;
public:
    using plane_type = std::array<double, 4>;
    mesh_slicer(const std::string& filename);

    /* Note: This will clear old things in `result_ref` */
    void get_intersection_polylines(const plane_type& plane_func, Polylines& result_ref); /* plane: ax+by+cz+d=0 */

    void save_intersection_polylines(const plane_type &plane_func,
                                     const std::string &filename,
                                     const std::string &seperator = ","); /* plane: ax+by+cz+d=0 */

#ifdef ENABLE_BOOST_PYTHON
    // 获取截面所截的等高线组,返回的list中, 可能有多个元素, 每个元素表示一段等高线, 是一个np.ndarray,
    // 每个np.ndarray是一个n*3的二维数组, 其中n为这一段等高线的点的数目
    py::list get_intersection_polylines_py(double a, double b, double c, double d); /* plane: ax+by+cz+d=0 */

    // 返回输入mesh的vertices点列, np.ndarray维度为n*2, 其中n为点的数量
    // 分别存储在np.ndarray第一维度的0~n-1的位置
    np::ndarray get_original_vertices_py() const;

    // 返回输入mesh的faces序列, 每一行是每个面所对应的0~n-1的vertices的序号
    np::ndarray get_original_faces_py() const;

    void save_intersection_polylines_py(double a, double b, double c, double d,
                                        const std::string &filename,
                                        const std::string &seperator = ",");
#endif // ENABLE_BOOST_PYTHON
};