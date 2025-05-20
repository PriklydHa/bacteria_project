#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>
#include <sstream>
#include <iomanip>
#include <memory>
#include <functional>

#include "particle.h"
#include "bacteria.h"
#include "nutritionParticle.h"
#include "matrixParticle.h"
#include "map.h"
#include "kdtree.h"

namespace py = pybind11;

class PyParticle : public Particle {
public:
    using Particle::Particle;

    void move(double orientation, double speed) override {
        PYBIND11_OVERRIDE_PURE(
            void,
            Particle,
            move,
            orientation,
            speed
        );
    }

    Particle* clone() const override {
        PYBIND11_OVERRIDE_PURE(
            Particle*,
            Particle,
            clone
        );
    }
};

// Объявление класса Node до привязки KdTree
class KdTreeWrapper {
public:
    struct Node {
        Point point;
        Axis axis;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
    };
};

// Привязка для Node
PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

PYBIND11_MODULE(particle_sim, m) {
    py::add_ostream_redirect(m, "ostream_redirect");

    // Привязка для Particle
    py::class_<Particle, PyParticle, std::shared_ptr<Particle>>(m, "Particle", py::dynamic_attr())
        .def(py::init<double, double, double>())
        .def_property("x", &Particle::getX, &Particle::setX)
        .def_property("y", &Particle::getY, &Particle::setY)
        .def_property("size", &Particle::getSize, &Particle::setSize)
        .def("__repr__", [](const Particle& p) {
            std::ostringstream oss;
            oss << "Particle at (" << p.getX() << ", " << p.getY() << "), size=" << p.getSize();
            return oss.str();
        });

    // Привязка для Bacteria
    py::class_<Bacteria, Particle, std::shared_ptr<Bacteria>>(m, "Bacteria")
        .def(py::init<double, double, double, double, double, double>())
        .def_property("orientation", &Bacteria::getOrientation, &Bacteria::setOrientation)
        .def_property("speed", &Bacteria::getSpeed, &Bacteria::setSpeed)
        .def_property("nutrition", &Bacteria::getNutrition, &Bacteria::setNutrition)
        .def("move", &Bacteria::move)
        .def("eat", &Bacteria::eat)
        .def("excrete", &Bacteria::excrete);

    // Привязка для NutritionParticle
    py::class_<NutritionParticle, Particle, std::shared_ptr<NutritionParticle>>(m, "NutritionParticle")
        .def(py::init<double, double, double, double>())
        .def("move", &NutritionParticle::move)
        .def_property("nutrient_value",
            &NutritionParticle::getNutrientValue,
            &NutritionParticle::setNutrientValue)
        .def("__repr__", [](const NutritionParticle& np) {
            std::ostringstream oss;
            oss << "NutritionParticle(" << np.getX() << ", " << np.getY()
                << "), value=" << std::fixed << std::setprecision(1) << np.getNutrientValue();
            return oss.str();
        });

    // Привязка для MatrixParticle
    py::class_<MatrixParticle, Particle, std::shared_ptr<MatrixParticle>>(m, "MatrixParticle")
        .def(py::init<double, double, double>())
        .def("move", &MatrixParticle::move);

    // Привязка для Map
    py::class_<Map>(m, "Map")
        .def(py::init<double, double>())
        .def("add_object", &Map::addObject)
        .def("get_objects_near", &Map::getObjectsNear)
        .def("display", &Map::display);




    // Привязки для KD-дерева (исправленная версия)
    py::class_<Point>(m, "Point")
        .def(py::init<int, int>())
        .def_readwrite("x", &Point::x)
        .def_readwrite("y", &Point::y);
    
    py::enum_<Axis>(m, "Axis")
        .value("X", Axis::X)
        .value("Y", Axis::Y);

    // Привязка Node (используем явное указание типа)
    py::class_<KdTree::Node, std::shared_ptr<KdTree::Node>>(m, "KdTreeNode")
        .def_readwrite("point", &KdTree::Node::point)
        .def_readwrite("axis", &KdTree::Node::axis)
        .def_readwrite("left", &KdTree::Node::left)
        .def_readwrite("right", &KdTree::Node::right);

    // Привязка Writer (исправлен синтаксис)
    py::class_<Writer<std::vector<Point>>>(m, "KdWriter")
        .def("get_value", &Writer<std::vector<Point>>::getValue)
        .def("get_logs", &Writer<std::vector<Point>>::getLogs);

    // Привязка KdTree
    py::class_<KdTree>(m, "KdTree")
        .def_static("build", 
            [](const std::vector<Point>& points, Axis axis) {
                return KdTree::build(points, axis);
            }, 
            py::return_value_policy::reference
        )
        .def_static("search_range", &KdTree::searchRange);
}