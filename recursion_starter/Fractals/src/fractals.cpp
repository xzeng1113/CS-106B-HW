/*
 * File: fractals.cpp
 * --------------------------
 * Name: Xiaoxi Zeng
 * Section leader: Semir Shafi
 * This program drawas Sierpinski Triangle and Mandelbrot Set
 * using recursive function.
 */


#include "fractals.h"
#include <cmath>
#include "complex.h"

using namespace std;

const double COS_60 = 0.5;
const double SIN_60 = sqrt(3) * 0.5;
const int LEAF_COLOR = 0x2e8b57;   /* Color of all leaves of recursive tree (level 1) */
const int BRANCH_COLOR = 0x8b7765; /* Color of all branches of recursive tree (level >=2) */

/**
 * Draws a Sierpinski triangle of the specified size and order, placing its
 * top-left corner at position (x, y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Sierpinski triangle.
 * @param x - The x coordinate of the top-left corner of the triangle.
 * @param y - The y coordinate of the top-left corner of the triangle.
 * @param size - The length of one side of the triangle.
 * @param order - The order of the fractal.
 */
void drawSierpinskiTriangle(GWindow& gw, double x, double y, double size, int order) {
    //Throw an exception for invalid input
    if(x < 0 || y < 0 || size < 0 || order < 0){
        throw("Invalid input.");
    }
    //If the order passed is 0, then draw nothing in the window
    if(order == 0){
        return;
    }
    //Base case
    if(order == 1){
        gw.drawLine(x, y, (x + size), y);
        gw.drawLine(x, y, x + size * COS_60, y + size * SIN_60);
        gw.drawLine((x + size), y, x + size * COS_60, y + size * SIN_60);
    }else{
        //Recursive call
        size /= 2;
        drawSierpinskiTriangle(gw, x, y, size, order - 1);
        x += size;
        drawSierpinskiTriangle(gw, x, y, size, order - 1);
        x -= size * COS_60;
        y += size * SIN_60;
        drawSierpinskiTriangle(gw, x, y, size, order - 1);
    }
}

/**
 * Draws a recursive tree fractal image of the specified size and order,
 * placing the bounding box's top-left corner at position (x,y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the recursive tree.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 * @param order - The order of the fractal.
 */
void drawTree(GWindow& gw, double x, double y, double size, int order) {
    // TODO: Write this function
}

/**
 * Draws a Mandelbrot Set in the graphical window give, with maxIterations
 * (size in GUI) and in a given color (zero for palette)
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Mandelbrot set.
 * @param minX - left-most column of grid
 * @param incX - increment value of columns of grid
 * @param minY - top-most row of grid
 * @param incY - increment value of rows of grid
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @param color - The color of the fractal; zero if palette is to be used
 */
void mandelbrotSet(GWindow& gw, double minX, double incX,
                   double minY, double incY, int maxIterations, int color) {

    // Creates palette of colors
    // To use palette:
    // pixels[r][c] = palette[numIterations % palette.size()];
    Vector<int> palette = setPalette();

    int width = gw.getCanvasWidth();
    int height = gw.getCanvasHeight();
    GBufferedImage image(width,height,0xffffff);
    gw.add(&image);
    Grid<int> pixels = image.toGrid(); // Convert image to grid

    //Define all the points to inspect for inclusion in the Mandelbrot Set
    for(int r = 0; r < pixels.numRows(); r ++){
        for(int c = 0; c < pixels.numCols(); c++){

            //Create complex numbers based on given increments
            Complex pt = Complex(minX + c * incX, minY + r * incY);
            int numIters = mandelbrotSetIterations(pt, maxIterations);

            //Determine the color of the picture
            if(color == 0){
                pixels[r][c] = palette[numIters % palette.size()];
            }else{
                if(numIters == maxIterations){
                    pixels[r][c] = color;
                }
            }
        }
    }
    image.fromGrid(pixels); // Converts and puts the grid back into the image
}

/**
 * Runs the Mandelbrot Set recursive formula on complex number c a maximum
 * of maxIterations times.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param c - Complex number to use for recursive formula.
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */
int mandelbrotSetIterations(Complex c, int maxIterations) {
    Complex z = Complex(0, 0);

    //Call helper function
    int remainings = mandelbrotSetIterations(z, c, maxIterations);
    if(remainings == 0){
        return maxIterations;
    }else{
        return (maxIterations - remainings + 1);
    }
}
/**
 * An iteration of the Mandelbrot Set recursive formula with given values z and c, to
 * run for a maximum of maxIterations.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param z - Complex number for a given number of iterations
 * @param c - Complex number to use for recursive formula.
 * @param remainingIterations - The remaining number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */
int mandelbrotSetIterations(Complex z, Complex c, int remainingIterations) {
    //Base case
    if(remainingIterations == 0){
        return remainingIterations;
    }
    //Recursive call
    z = z * z + c;
    if(z.abs() > 4){
        return remainingIterations;
    }else{
        return mandelbrotSetIterations(z, c, remainingIterations - 1);
    }
}

// Helper function to set the palette
Vector<int> setPalette() {
    Vector<int> colors;

    // Feel free to replace with any palette.
    // You can find palettes at:
    // http://www.colourlovers.com/palettes

    // Example palettes:
    // http://www.colourlovers.com/palette/4480793/in_the_middle
    // string colorSt = "#A0B965,#908F84,#BF3C43,#9D8E70,#C9BE91,#A0B965,#908F84,#BF3C43";

    // http://www.colourlovers.com/palette/4480786/Classy_Glass
    // string colorSt = "#9AB0E9,#C47624,#25269A,#B72202,#00002E,#9AB0E9,#C47624,#25269A";

    // The following is the "Hope" palette:
    // http://www.colourlovers.com/palette/524048/Hope
    string colorSt =  "#04182B,#5A8C8C,#F2D99D,#738585,#AB1111,#04182B,#5A8C8C,#F2D99D";
    Vector<string>colorsStrVec = stringSplit(colorSt,",");
    for (string color : colorsStrVec) {
        colors.add(convertColorToRGB(trim(color)));
    }
    return colors;
}
