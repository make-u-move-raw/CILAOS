#pragma once

/**
 * @namespace MyLib
 * @brief A custom namespace for demonstration purposes
 * 
 * This resumes the class "Rectangle"
 */
namespace MyLib{
/**
 * @class Rectangle
 * @brief Represents a rectangle
 * 
 * Can get perimeter and area of shape
 */
class Rectangle {
public:
    /**
     * @brief Constructor
     * @param w Width of rect
     * @param h Height of rect
     */
    Rectangle(double w, double h);

    /**
     * @brief Compute rect area
     * @return Area of rect
     */
    double area() const;

    /**
     * @brief Compute perimeter of rect
     * @return Perimeter of rect
     */
    double perim() const;

private:
    double w_;
    double h_;
};
}