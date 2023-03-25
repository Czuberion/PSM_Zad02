/*
 * | ds/dt = v
 * | dv/dt = a
 */

// Fo = -qV, q in [0..1]

// 1) f(x0 + del_x) = f(x0) + f`(x0) * del_x
// 2) f(x0 + del x) = f(x0) + f`(x0 + del_x/2) * del_x

#include <iostream>
#include <vector>
#include <utility>
#include <functional>
#include <string>

const double G = 9.8; // standard gravity

auto euler_method(const double &f, const std::function<double(double)> &f_derivative, const double &delta_x) -> double;

auto euler_method(const double &f, const double &f_derivative, const double &delta_x) -> double;

/*auto improved_euler(const double &f, const std::function<double(double)> &f_derivative, const double &delta_x) -> double;

auto improved_euler(const double &f, const double &f_derivative, const double &delta_x) -> double;*/

class Point {
    const double delta = 0.1;
    const double q = 0.1;
    double x;
    double y;
    double mass;
    std::pair<double, double> v;
    std::pair<double, double> a;
    std::vector<std::pair<double, double>> forces;
public:
    enum class SolvingMethod {EULER, IMPROVED_EULER};

    Point(double x, double y, double mass) {
        this->x = x;
        this->y = y;
        this->mass = mass;
        this->v.first = 0.0;
        this->v.second = 0.0;
        this->a.first = 0.0;
        this->a.second = 0.0;
        forces.emplace_back(0.0,-G * mass); // apply gravitation
        forces.emplace_back(0.0, -q * v.second); // apply resistance
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
            euler_method(a.first, [force,this](auto val) -> double {return force.first/mass;}, delta),
            euler_method(a.second, [force,this](auto val) -> double {return force.second/mass;}, delta)
            );
            velocity = std::make_pair(
            euler_method(v.first,acceleration.first, delta),
            euler_method(v.second,acceleration.second,delta)
            );
            s = std::make_pair(
            euler_method(x,velocity.first,delta),
            euler_method(y,velocity.second,delta)
            );
        } else {
            auto half_accel = std::make_pair(
            euler_method(a.first, [force,this](auto val) -> double {return force.first/mass;}, delta/2.0),
            euler_method(a.second, [force,this](auto val) -> double {return force.second/mass;}, delta/2.0)
            );
            acceleration = std::make_pair(
            euler_method(half_accel.first, [force,this](auto val) -> double {return force.first/mass;}, delta),
            euler_method(half_accel.second, [force,this](auto val) -> double {return force.second/mass;}, delta)
            );

            auto half_vel = std::make_pair(
            euler_method(v.first,acceleration.first, delta/2.0),
            euler_method(v.second,acceleration.second,delta/2.0)
            );
            /*velocity = std::make_pair(
            improved_euler(v_x,acceleration.first, delta),
            improved_euler(v_y,acceleration.second,delta)
            );*/
            velocity = std::make_pair(
            euler_method(half_vel.first,acceleration.first, delta),
            euler_method(half_vel.second,acceleration.second,delta)
            );
            auto half_s = std::make_pair(
            euler_method(x,velocity.first,delta),
            euler_method(y,velocity.second,delta)
            );
            /*s = std::make_pair(
            improved_euler(x,velocity.first,delta),
            improved_euler(y,velocity.second,delta)
            );*/
            s = std::make_pair(
            euler_method(half_s.first,velocity.first,delta),
            euler_method(half_s.second,velocity.second,delta)
            );
        }
        x = s.first;
        y = s.second;
        v = velocity;
        a = acceleration;
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

};

int main() {
    Point point(12.5, 34.3, 100);
    auto pair_to_string = [] (const std::pair<double, double> &a) -> std::string {
        return "x:\t" + std::to_string(a.first) + ", y:\t" + std::to_string(a.second);
    };

    std::cout << pair_to_string(point.get_position()) << std::endl;
    std::cout << "mass: " << std::to_string(point.get_mass()) << '\n';

    while (point.get_position().second > 0) {
        //point.update(Point::SolvingMethod::EULER);
        point.update(Point::SolvingMethod::IMPROVED_EULER);
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

/*auto improved_euler(const double &f, const std::function<double(double)> &f_derivative, const double &delta_x) -> double {
    return f + euler_method(f, f_derivative, delta_x/2.0) * delta_x;
}

auto improved_euler(const double &f, const double &f_derivative, const double &delta_x) -> double {
    return f + euler_method(f, f_derivative, delta_x/2.0) * delta_x;
}*/
