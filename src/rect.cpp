#include "rect.h"

Rectangle::Rectangle(double w, double h) : w_(w), h_(h) {}

double Rectangle::area() const {
    return w_ * h_;
}

double Rectangle::perim() const {
    return 2 * (w_ + h_);
}