#include <libtcod.hpp>

#include <vector>
#include <iostream>
#include <cmath>

#include "ColorScheme.hpp"
#include "rll/BMP/BMP.hpp"

using namespace std;

int xAreaSize = 0;
int yAreaSize = 0;
size_t particleCount = 0;

const string imageFile = "Quake.bmp";

const float acceleration = 10;
const float accelerationPerPixel = 0.0025;

const float stopVel = 0.05;

const float deccelerationCoeff = 0.99;

TCODRandom rnd;

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
    Particle(const Point< float >& startPoint) : coordinate(startPoint), gravitation(false) {
//        home.x() = rnd.getFloat(0, xAreaSize);
//        home.y() = rnd.getFloat(0, yAreaSize);
    }

    void SetDestinationPoint(const Point< float >& destinationPoint) {
        destination = destinationPoint;
        gravitation = true;
    }

    void setHome(Point<float> newHome) {
        home = newHome;
    }

    void Update(float elapsedTime) {
        if(gravitation) {
            Point< float > distance = destination - coordinate;

            float rad = sqrt(pow(fabs(distance.x()),2) + pow(fabs(distance.y()),2));
            float maxRad = sqrt(pow(xAreaSize,2) + pow(yAreaSize,2));

            velocity.x() +=  (acceleration - (accelerationPerPixel*(maxRad - rad))) * ( fabs(distance.x()) / ( fabs(distance.x()) + fabs(distance.y()) ) ) * (distance.x() > 0 ? -1 : 1) * elapsedTime;
            velocity.y() +=  (acceleration - (accelerationPerPixel*(maxRad - rad))) * ( fabs(distance.y()) / ( fabs(distance.x()) + fabs(distance.y()) ) ) * (distance.y() > 0 ? -1 : 1) * elapsedTime;

            coordinate.x() += velocity.x();
            coordinate.y() += velocity.y();

        } else {
            if(velocity != Point<float>::MakePoint(0, 0)) {
                Point< float > distance = home - coordinate;

                float rad = sqrt(pow(fabs(distance.x()),2) + pow(fabs(distance.y()),2));

                velocity.x() += ( accelerationPerPixel / (1/rad) ) * (distance.x() > 0 ? 1 : -1) * elapsedTime;
                velocity.y() += ( accelerationPerPixel / (1/rad) ) * (distance.y() > 0 ? 1 : -1) * elapsedTime;

                velocity.x() *= deccelerationCoeff;
                velocity.y() *= deccelerationCoeff;

                if(velocity.x() < stopVel && fabs(coordinate.x()-home.x()) < 1) {
                    velocity.x() = 0;
                }
                if(velocity.y() < stopVel && fabs(coordinate.y()-home.y()) < 1) {
                    velocity.y() = 0;
                }

                coordinate.x() += velocity.x();
                coordinate.y() += velocity.y();
            }
        }

        Reflection();

        gravitation = false;
    }


    Point< float > coordinate;
    Point< float > velocity;

private:
    void Reflection() {
        if(coordinate.x() < 0 || coordinate.x() > xAreaSize) {
            velocity.x() = -velocity.x();
        }

        if(coordinate.y() < 0 || coordinate.y() > xAreaSize) {
            velocity.y() = -velocity.y();
        }
    }


    Point< float > destination;
    Point< float > home;
    bool gravitation;
};

class ParticleEnsemble : public vector< Particle > {
public:
    ParticleEnsemble(size_t particleCount) {
        for(size_t i= 0; i < particleCount; i++) {
            Add(GenerateParticle());
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
        return Particle(Point< float >::MakePoint(rnd.getInt(0, xAreaSize), rnd.getInt(0, yAreaSize)/*xAreaSize/2,yAreaSize/2*/));
    }
};

int main()
{
    rnd.setDistribution(TCOD_DISTRIBUTION_LINEAR);

    rll::Bitmap::QDBMPWrapper bmp(imageFile);

    xAreaSize = bmp.GetWidth();
    yAreaSize = bmp.GetHeight();

    cout << "Area Size : " << xAreaSize << " " << yAreaSize << endl;

    rll::Bitmap::ColorMapping mapping;

    mapping.AddMapping(rll::Bitmap::RGBColor(0, 0, 0), 0);
    mapping.AddMapping(rll::Bitmap::RGBColor(255, 255, 255), 1);

    rll::Space::NumField tr =  bmp.Transform(mapping);

    for(size_t y = 0; y < tr.GetYSize(); y++) {
        for(size_t x = 0; x < tr.GetXSize(); x++) {
            if(tr.CompareElement(x,y, 0)) {
                particleCount++;
            }
        }
    }
    cout << "Particle Count : " << particleCount << endl;

    ParticleEnsemble pe(particleCount);

    size_t particleIterator = 0;
    for(size_t y = 0; y < tr.GetYSize(); y++) {
        for(size_t x = 0; x < tr.GetXSize(); x++) {
            if(tr.CompareElement(x,y, 0)) {
                pe[particleIterator++].setHome(Point<float>::MakePoint(x,y));
            }
        }
    }

    TCODConsole::initRoot(xAreaSize, yAreaSize, "Particles Gravity");
    ColorSchemeI *scheme = ColorSchemeFactory::getScheme("cold");

    TCOD_key_t key;
    TCOD_mouse_t mouse;

    TCODMouse::showCursor(true);

    while(!TCODConsole::isWindowClosed()) {
        TCODSystem::checkForEvent(TCOD_EVENT_ANY, &key, &mouse);
        if(mouse.lbutton) {
            pe.SetDestinationPoint(Point<float>::MakePoint(mouse.cx, mouse.cy));
        } else if(key.c == 'q') {
            break;
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
            if(minVel == maxVel) { maxVel++; }
            color_t data(sqrt(pow(particle.velocity.x(),2) + pow(particle.velocity.y(),2)));
            scheme->writeColor(data, minVel, maxVel);

            TCODConsole::root->putCharEx(particle.coordinate.x(), particle.coordinate.y(), ' ', TCODColor::black, TCODColor(data.rgba.r,data.rgba.g,data.rgba.b));
        }

        TCODConsole::flush();
    }
    return 0;
}

