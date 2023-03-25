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
#include <string>

const double G = 9.8; // standard gravity

auto euler_method(const double &f, const std::function<double(double)> &f_derivative, const double &delta_x) -> double;

auto euler_method(const double &f, const double &f_derivative, const double &delta_x) -> double;

auto improved_euler(const double &f, const std::function<double(double)> &f_derivative, const double &delta_x) -> double;

auto improved_euler(const double &f, const double &f_derivative, const double &delta_x) -> double;

class Point {
    const double delta = 0.1;
    const double q = 0.1;
    double x;
    double y;
    double mass;
    double v_x = 0;
    double v_y = 0;
    double a_x = 0;
    double a_y = 0;
    std::vector<std::pair<double, double>> forces;
public:
    enum class SolvingMethod {EULER, IMPROVED_EULER};

    Point(double x, double y, double mass) {
        this->x = x;
        this->y = y;
        this->mass = mass;
        forces.emplace_back(0.0,-G * mass);
        forces.emplace_back(0.0, -q * v_y);
    }

    std::pair<double,double> get_position() const {
        return std::make_pair(x,y);
    }

    double get_mass() const {
        return mass;
    }

    void update(const SolvingMethod method) {
        auto force = net_force_calc(forces);
        std::pair<double,double> acceleration;
        std::pair<double,double> velocity;
        std::pair<double,double> s;

        if (method == SolvingMethod::EULER) {
            acceleration = std::make_pair(
            euler_method(a_x, [force,this](auto val) -> double {return force.first/mass;}, delta),
            euler_method(a_y, [force,this](auto val) -> double {return force.second/mass;}, delta)
            );
            velocity = std::make_pair(
            euler_method(v_x,acceleration.first, delta),
            euler_method(v_y,acceleration.second,delta)
            );
            s = std::make_pair(
            euler_method(x,velocity.first,delta),
            euler_method(y,velocity.second,delta)
            );
        } else {

        }
        x = s.first;
        y = s.second;
        v_x = velocity.first;
        v_y = velocity.second;
        a_x = acceleration.first;
        a_y = acceleration.second;
    }

private:
    static auto net_force_calc(const std::vector<std::pair<double,double>> &forces) -> std::pair<double, double> {
        std::pair<double,double> force = std::make_pair(0,0);
        std::for_each(forces.begin(), forces.end(), [&force] (const auto &a) -> void {
            force.first += a.first;
            force.second += a.second;
        });
        return force;
    }

    auto force_apply(const std::pair<double, double> &force) -> void {
        forces.push_back(force);
    }

    /*auto velocity() -> double {

    }

    auto acceleration(double prev) -> double {

    }*/
};

int main() {
    Point point(12.5, 34.3, 100);
    auto pair_to_string = [] (const std::pair<double, double> &a) -> std::string {
        return "x:\t" + std::to_string(a.first) + ", y:\t" + std::to_string(a.second);
    };

    std::cout << pair_to_string(point.get_position()) << std::endl;
    std::cout << "mass: " << std::to_string(point.get_mass()) << '\n';

    while (point.get_position().second > 0) {
        point.update(Point::SolvingMethod::EULER);
        //point.update(Point::SolvingMethod::IMPROVED_EULER);
        std::cout << pair_to_string(point.get_position()) << '\n';
    }

    return 0;
}

auto euler_method(const double &f, const std::function<double(double)> &f_derivative, const double &delta_x) -> double {
    return f + f_derivative(f) * delta_x;
}

auto euler_method(const double &f, const double &f_derivative, const double &delta_x) -> double {
    return f + f_derivative * delta_x;
}

auto improved_euler(const double &f, const std::function<double(double)> &f_derivative, const double &delta_x) -> double {
    return f + f_derivative(f) * delta_x;
}

auto improved_euler(const double &f, const double &f_derivative, const double &delta_x) -> double {
    return f + f_derivative * delta_x;
}


// remove later:
auto
improved_euler(const std::function<double(double)> &f, const std::function<double(double)> &f_derivative,
               double x0, double delta_x)
-> double {
    return f(x0) + f_derivative(x0 + delta_x/2.0) * delta_x;
}