#include <libtcod.hpp>

#include <vector>
#include <iostream>
#include <cmath>

#include "ColorScheme.hpp"

using namespace std;

const int xAreaSize = 300;
const int yAreaSize = 300;

const float acceleration = 3;
const float accelerationPerPixel = 0.0025;
const int fps = 30;

const size_t particleCount = 1000;

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
public:
    Particle(const Point< float >& startPoint) : coordinate(startPoint), gravitation(false) { }

    void SetDestinationPoint(const Point< float >& destinationPoint) {
        destination = destinationPoint;
        gravitation = true;
    }

    void Update(float elapsedTime) {
        if(gravitation) {
            Point< float > distance = destination - coordinate;

            velocity.x() += accelerationPerPixel *  (xAreaSize - distance.x()) * (distance.x() > 0 ? 1 : -1) * elapsedTime;
            velocity.y() += accelerationPerPixel *  (yAreaSize - distance.y()) * (distance.y() > 0 ? 1 : -1) * elapsedTime;

            coordinate.x() += velocity.x();
            coordinate.y() += velocity.y();

//            if(distance.x() < 0) {
//                coordinate.x() += velocity.x();
//            } else {
//                coordinate.x() -= velocity.x();
//            }

//            if(distance.y() < 0) {
//                coordinate.y() += velocity.y();
//            } else {
//                coordinate.y() -= velocity.y();
//            }

        } else {
            velocity.x() *= 0.99;
            velocity.y() *= 0.99;

            coordinate.x() += velocity.x();
            coordinate.y() += velocity.y();
        }

        Reflection();

        gravitation = false;
    }

//private:
    void Deceleration() {

    }

    void Reflection() {
        if(coordinate.x() < 0 || coordinate.x() > xAreaSize) {
            velocity.x() = -velocity.x();
        }

        if(coordinate.y() < 0 || coordinate.y() > xAreaSize) {
            velocity.y() = -velocity.y();
        }
    }

    Point< float > coordinate;
    Point< float > velocity;
    Point< float > destination;
    bool gravitation;
};

class ParticleEnsemble : public vector< Particle > {
public:
    ParticleEnsemble(size_t particleCount) {
        for(size_t i= 0; i < particleCount; i++) {
            Add(GenerateParticle());
            SetDestinationPoint(Point<float>::MakePoint(0,0));
        }
    }

    void Add(const Particle& p) { push_back(p); }

    void SetDestinationPoint(const Point<float>& coord) {
        for(auto &i : *this) {
            i.SetDestinationPoint(coord);
        }
    }

    void Update(float elapsedTime) {
        for(auto &i : *this) {
            i.Update(elapsedTime);
        }
    }

private:
    Particle GenerateParticle() {
        return Particle(Point< float >::MakePoint(rand()%xAreaSize, rand()%yAreaSize));
    }
};

int main()
{
    TCODConsole::initRoot(xAreaSize, yAreaSize, "Particles Gravity");
    ColorSchemeI *scheme = ColorSchemeFactory::getScheme("cold");

    TCOD_key_t key;
    TCOD_mouse_t mouse;

    ParticleEnsemble pe(particleCount);

    while(!TCODConsole::isWindowClosed()) {
        TCODSystem::checkForEvent(TCOD_EVENT_ANY, &key, &mouse);
        if(mouse.lbutton) {
            cout << mouse.cx << " " << mouse.cy << " : " << pe[0].coordinate.x() << " " << pe[0].coordinate.y() << endl;
            pe.SetDestinationPoint(Point<float>::MakePoint(mouse.cx, mouse.cy));
        }

        pe.Update(TCODSystem::getLastFrameLength());


        TCODConsole::root->clear();


        float minVel = 100, maxVel = 0;
        for(auto particle : pe) {
            float vel = sqrt(pow(particle.velocity.x(),2) + pow(particle.velocity.y(),2));

            if(minVel > vel) { minVel = vel; }
            if(maxVel < vel) { maxVel = vel; }
        }

        for(auto particle : pe) {
            color_t data(sqrt(pow(particle.velocity.x(),2) + pow(particle.velocity.y(),2)));
            scheme->writeColor(data, minVel, maxVel);

            TCODConsole::root->putCharEx(particle.coordinate.x(), particle.coordinate.y(), ' ', TCODColor::black, TCODColor(data.rgba.r,data.rgba.g,data.rgba.b));
        }

        TCODConsole::flush();
    }
    return 0;
}

