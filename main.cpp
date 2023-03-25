/*
 * | ds/dt = v
 * | dv/dt = a
 */

// Fo = -qV, q in [0..1]

// 1) f(x0 + del_x) = f(x0) + f`(x0) * del_x
// 2) f(x0 + del x) = f(x0) + f`(x0 + del_x/2) * del_x

#include <iostream>
//#include <array>
#include <vector>
//#include <numeric>
#include <utility>
#include <functional>

auto
euler_method(const std::function<double(double)> &f, const std::function<double(double)> &f_derivative,
             double x0, double delta_x)
-> double;
auto
improved_euler(const std::function<double(double)> &f, const std::function<double(double)> &f_derivative,
               double x0, double delta_x)
-> double;

class Point {
    double x;
    double y;
    double mass;
public:
    Point(double x, double y, double mass) {
        this->x = x;
        this->y = y;
        this->mass = mass;
    }

    std::pair<double,double> get_position() const {
        return std::make_pair(x,y);
    }

    double get_mass() const {
        return mass;
    }

    void update() {

    }

private:
    auto net_force(const std::vector<std::pair<double,double>> &forces) -> std::pair<double, double> {
        std::pair<double,double> force = std::make_pair(0,0);
        std::for_each(forces.begin(), forces.end(), [&force] (const auto &a) -> void {
            force.first += a.first;
            force.second += a.second;
        });
        return force;
    }
};

int main() {
    Point point(12, 34, 100);

    //std::cout << point.get_mass() << '\n';

    return 0;
}

auto
euler_method(const std::function<double(double)> &f, const std::function<double(double)> &f_derivative,
             double x0, double delta_x)
-> double {
    return f(x0) + f_derivative(x0) * delta_x;
}

auto
improved_euler(const std::function<double(double)> &f, const std::function<double(double)> &f_derivative,
               double x0, double delta_x)
-> double {
    return f(x0) + f_derivative(x0 + delta_x/2.0) * delta_x;
}