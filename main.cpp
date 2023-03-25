/*
 * | ds/dt = v
 * | dv/dt = a
 */

// Fo = -qV, q in [0..1]

// 1) f(x0 + del_x) = f(x0) + f`(x0) * del_x
// 2) f(x0 + del x) = f(x0) + f`(x0 + del_x/2) * del_x

#include <iostream>
#include <array>
#include <vector>
#include <numeric>
#include <utility>

class Point {
    int x;
    int y;
    int mass;
public:
    Point(int x, int y, int mass) {
        this->x = x;
        this->y = y;
        this->mass = mass;
    }

    int get_x() const {
        return x;
    }

    int get_y() const {
        return y;
    }

    int get_mass() const {
        return mass;
    }

    void update() {

    }

private:
    std::pair<int, int> net_force(const std::vector<std::pair<int,int>> &forces) {
        std::pair<int,int> force = std::make_pair(0,0);
        std::for_each(forces.begin(), forces.end(), [&force] (auto a) -> void {
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
