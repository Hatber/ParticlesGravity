#include <libtcod.hpp>

#include <vector>
#include <iostream>

#include "ColorScheme.hpp"

using namespace std;

const int xAreaSize = 300;
const int yAreaSize = 300;

const float acceleration = 3;
const float accelerationPerPixel = 0.025;
const int fps = 30;

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

            velocity.x() = acceleration * elapsedTime;
            velocity.y() = acceleration * elapsedTime;

            if(distance.x() > 0) {
                coordinate.x() += velocity.x();
            } else {
                    coordinate.x() -= velocity.x();
            }

            if(distance.y() > 0) {
                coordinate.y() += velocity.y();
            } else {
                    coordinate.y() -= velocity.y();
            }

        } else {
            Deceleration();
        }

        Reflection();

        RecalcPosition();

        gravitation = false;
    }

//private:
    void Deceleration() {}
    void Reflection() {}
    void RecalcPosition() {}

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
    ColorSchemeI *scheme = ColorSchemeFactory::getScheme("soft");

    TCOD_key_t key;
    TCOD_mouse_t mouse;

    float vel = accelerationPerPixel;
    float elapsedTime = 0;

    ParticleEnsemble pe(10000);

    while(!TCODConsole::isWindowClosed()) {
        TCODSystem::checkForEvent(TCOD_EVENT_ANY, &key, &mouse);
        if(mouse.lbutton) {
            pe.SetDestinationPoint(Point<float>::MakePoint(mouse.x, mouse.y));
        }

        pe.Update(TCODSystem::getLastFrameLength());


        TCODConsole::root->clear();
        for(auto particle : pe) {
            TCODConsole::root->putCharEx(particle.coordinate.x(), particle.coordinate.y(), ' ', TCODColor::black, TCODColor(255,255,255));
        }

        TCODConsole::flush();
    }
    return 0;
}

