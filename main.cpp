#include <libtcod.hpp>

#include <vector>
#include <iostream>

using namespace std;

const int xAreaSize = 100;
const int yAreaSize = 100;

const float accelerationPerPixel = 0.1;

template < typename T>
class Point {
public:
    Point() : Point(T(), T()) { }
    Point(const T& x, const T& y) : _x(x), _y(y) { }

    T x() const { return _x; }
    T y() const { return _y; }

    T& x() { return _x; }
    T& y() { return _y; }

    Point Shift(const T& x, const T& y) const { return Point(_x + x, _y + y); }

    bool operator == (const Point& p) { return x()==p.x() && y()==p.y(); }
    bool operator != (const Point& p) { return !(operator ==(p)); }

    const Point operator-(const Point& p) {
        return MakePoint(x() - p.x(), y() - p.y());
    }

    Point& operator = (const Point& right) {
        if (this == &right) {
            return *this;
        }

        x() = right.x();
        y() = right.y();

        return *this;
    }

    friend ostream& operator << (ostream& os, const Point& p) {
        os << "[" << p.x() << ", " << p.y() << "]";
        return os;
    }

    virtual ~Point() { }

    static Point MakePoint(const T& x, const T& y) { return Point(x, y); }
private:
    T _x;
    T _y;
};

class Particle {
    Particle(const Point< float >& startPoint) : coordinate(startPoint), gravitation(false) { }

    void SetDestinationPoint(const Point< float >& destinationPoint) {
        destination = destinationPoint;
        gravitation = true;
    }

    void Update(float elpsedTime) {
        if(gravitation) {
            Point< float > distance = destination - coordinate;

        } else {
            Deceleration();
        }

        Reflection();

        RecalcPosition();

        gravitation = false;
    }

private:
    void Deceleration() {}
    void Reflection() {}
    void RecalcPosition() {}

    Point< float > coordinate;
    Point< float > velocity;
    Point< float > destination;
    bool gravitation;
};

int main()
{

    return 0;
}

