#pragma once
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <functional>

enum class Axis { X, Y };

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
    
    // Добавляем оператор присваивания
    Point& operator=(const Point& other) = default;
};

template<typename T>
class Writer {
    T value;
    std::vector<std::string> logs;
public:
    Writer(T v, std::vector<std::string> lgs = {}) 
        : value(v), logs(lgs) {}
    
    Writer<T> bind(std::function<Writer<T>(T)> f) {
        auto newWriter = f(value);
        std::vector<std::string> newLogs = logs;
        newLogs.insert(newLogs.end(), newWriter.logs.begin(), newWriter.logs.end());
        return Writer<T>(newWriter.value, newLogs);
    }
    
    T getValue() const { return value; }
    std::vector<std::string> getLogs() const { return logs; }
};

class KdTree {
public:
    struct Node {
        Point point;
        Axis axis;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(Point p, Axis a) : point(p), axis(a), left(nullptr), right(nullptr) {}
    };

    static std::shared_ptr<Node> build(const std::vector<Point>& points, Axis axis = Axis::X);
    static Writer<std::vector<Point>> searchRange(
        const Point& lower, 
        const Point& upper, 
        const std::shared_ptr<Node>& root
    );
};