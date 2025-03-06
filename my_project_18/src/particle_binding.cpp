#include <pybind11/pybind11.h>
#include <pybind11/operators.h> // Для привязки операторов
#include "particle.h"

namespace py = pybind11;

// Трамплинный класс для Particle
class PyParticle : public Particle {
public:
    // Наследуем конструктор базового класса
    using Particle::Particle;

    // Переопределяем чисто виртуальный метод move
    void move(double orientation, double speed) override {
        PYBIND11_OVERRIDE_PURE(
            void,        // Тип возвращаемого значения
            Particle,    // Базовый класс
            move,        // Имя метода
            orientation, // Аргументы
            speed
        );
    }

    // Переопределяем чисто виртуальный метод clone
    Particle* clone() const override {
        PYBIND11_OVERRIDE_PURE(
            Particle*,   // Тип возвращаемого значения
            Particle,    // Базовый класс
            clone
        );
    }
};

PYBIND11_MODULE(particle, m) {
    m.doc() = "Python bindings for Particle class";

    // Привязка класса Particle с указанием трамплинного класса PyParticle
    py::class_<Particle, PyParticle>(m, "Particle")
        // Исправление 1: Конструктор с double аргументами (было int)
        .def(py::init<double, double, double>(), 
             py::arg("x"), py::arg("y"), py::arg("size"))
        // Свойства для доступа к полям
        .def_property("x", &Particle::getX, &Particle::setX)
        .def_property("y", &Particle::getY, &Particle::setY)
        .def_property("size", &Particle::getSize, &Particle::setSize)
        // Методы
        .def("move", &Particle::move)
        .def("clone", &Particle::clone)
        // Исправление 2: Привязка оператора << через __repr__
        .def("__repr__", [](const Particle& p) {
            std::ostringstream oss;
            oss << p;
            return oss.str();
        });
}