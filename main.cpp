/*
 * | ds/dt = v
 * | dv/dt = a
 */

// Fo = -qV, q in [0..1]

// 1) f(x0 + del_x) = f(x0) + f`(x0) * del_x
// 2) f(x0 + del x) = f(x0) + f`(x0 + del_x/2) * del_x

#include <iostream>

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
};

int main() {
    Point point(12, 34, 100);

    //std::cout << point.get_mass() << '\n';

    return 0;
}
