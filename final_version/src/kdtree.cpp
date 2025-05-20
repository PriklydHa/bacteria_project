#include "kdtree.h"
#include <algorithm>

using namespace std;

shared_ptr<KdTree::Node> KdTree::build(const vector<Point>& points, Axis axis) {
    if (points.empty()) return nullptr; //проверка на пустоту

    // Создаем копию вектора для сортировки
    vector<Point> sorted_points = points;
    
    auto comparator = [axis](const Point& a, const Point& b) {
        return (axis == Axis::X) ? (a.x < b.x) : (a.y < b.y);//сравнение
    };
    sort(sorted_points.begin(), sorted_points.end(), comparator);//сортировка по текущей оси

    size_t median = sorted_points.size() / 2;//находим медиану
    auto node = make_shared<KdTree::Node>(sorted_points[median], axis);//создали узел в медиане с разделениемпо текущей оси

    vector<Point> left_points(sorted_points.begin(), sorted_points.begin() + median);//левое поддерево-список
    vector<Point> right_points(sorted_points.begin() + median + 1, sorted_points.end());//правое поддерево- список

    node->left = build(left_points, (axis == Axis::X) ? Axis::Y : Axis::X);
    node->right = build(right_points, (axis == Axis::X) ? Axis::Y : Axis::X);

    return node;//указатель на корень дерева
}

Writer<vector<Point>> KdTree::searchRange(
    const Point& lower, 
    const Point& upper, 
    const shared_ptr<Node>& root
) {
    vector<Point> result;
    vector<string> logs;

    function<void(const shared_ptr<Node>&)> search = [&](const shared_ptr<Node>& node) {//рекурс.функция поиска
        if (!node) return;//завершение если узел пуст

        logs.push_back("Visiting node: (" + to_string(node->point.x) + ", " + to_string(node->point.y) + ")");
//добавили инфу о текущем узле

        if (node->point.x >= lower.x && node->point.x <= upper.x &&
            node->point.y >= lower.y && node->point.y <= upper.y) {
            result.push_back(node->point);//попадает ли точка в диапазон-да -добавляет в резалт
        }

        if (node->axis == Axis::X) {
            if (node->point.x >= lower.x) search(node->left);
            if (node->point.x <= upper.x) search(node->right);
        } else {
            if (node->point.y >= lower.y) search(node->left);
            if (node->point.y <= upper.y) search(node->right);//какие деревья нужны, куда идти
        }
    };

    search(root);
    return Writer<vector<Point>>(result, logs);//вернули обьект с точками и логами
}