#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <sstream>  // Добавлен заголовок для std::ostringstream
#include "../include/particle.h"

namespace py = pybind11;

// Трамплинный класс для Particle
class PyParticle : public Particle {
public:
    using Particle::Particle;

    void move(double orientation, double speed) override {
        PYBIND11_OVERRIDE_PURE(
            void, Particle, move, orientation, speed
        );
    }

    Particle* clone() const override {
        PYBIND11_OVERRIDE_PURE(
            Particle*, Particle, clone
        );
    }
};

// Перегрузка оператора << для Particle (если требуется)
std::ostream& operator<<(std::ostream& os, const Particle& p) {
    os << "Particle(x=" << p.getX() 
       << ", y=" << p.getY() 
       << ", size=" << p.getSize() << ")";
    return os;
}

PYBIND11_MODULE(particle, m) {
    m.doc() = "Python bindings for Particle class";

    py::class_<Particle, PyParticle>(m, "Particle")
        .def(py::init<double, double, double>(), 
            py::arg("x"), py::arg("y"), py::arg("size"))
        .def_property("x", &Particle::getX, &Particle::setX)
        .def_property("y", &Particle::getY, &Particle::setY)
        .def_property("size", &Particle::getSize, &Particle::setSize)
        .def("move", &Particle::move)
        .def("clone", &Particle::clone)
        // Убрана лишняя точка с запятой и добавлен оператор <<
        .def("__repr__", [](const Particle& p) {
            std::ostringstream oss;
            oss << p;  // Использует перегруженный оператор <<
            return oss.str();
        });
}