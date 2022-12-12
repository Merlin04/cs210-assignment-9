//
// Created by me on 12/11/22.
//

#ifndef INC_1_VEHICLES_CAR_H
#define INC_1_VEHICLES_CAR_H

#include <cmath>
#include <iostream>

/*
 * Write the code for a Car class. It should represent the car objects that
 * move from location to location using fuel from their gas tank. Cars
 * should have variables representing their position in a two-dimensional
 * space, their fuel efficiency in miles per gallon, the size of their fuel
 * tank, and how much fuel they currently have. Cars should also have a
 * method move_to that tries to move to new coordinates. If the car has
 * enough fuel to drive to the new coordinates in a straight line, it does
 * so and returns true. If not, the car does not move and returns false.
 */

class Car {
public:
    double x;
    double y;
    double mpg;
    double tank_size;
    double fuel;

    Car() : Car(0, 0, 0, 0, 0) {}
    Car(double mpg, double tank_size, double fuel) : Car(0, 0, mpg, tank_size, fuel) {}
    Car(double x, double y, double mpg, double tank_size, double fuel) : x(x), y(y), mpg(mpg), tank_size(tank_size), fuel(fuel) {}

    virtual bool move_to(double new_x, double new_y) {
        double distance = sqrt(pow(new_x - this->x, 2) + pow(new_y - this->y, 2));
        if(distance > this->fuel * this->mpg) {
            return false;
        }
        this->x = new_x;
        this->y = new_y;
        this->fuel -= distance / this->mpg;
        return true;
    }

    friend std::ostream& operator<<(std::ostream &out, const Car &car) {
        out << "Car at (" << car.x << ", " << car.y << "), fuel: " << car.fuel << " (MPG: " << car.mpg << ", tank size: " << car.tank_size << ")";
        return out;
    }
};

#endif //INC_1_VEHICLES_CAR_H
