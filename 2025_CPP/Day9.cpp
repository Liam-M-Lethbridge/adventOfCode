#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;
ifstream input("theFile.txt");

// part two is crazy.
// however I think there is a possibility of an efficient algorithm.
// If I can figure out how to determine if a vertex is concave or convex?
// a vertex is concave if it "turns inwards". A rectangle has 4 concave vertices. 
// with every new convex edge, a concave edge would be needed to eventually complete the shape and vice vera.
// We can figure out the distinction between convex and concave because one turns clockwise and the other anticlockwise.
// Hence there will be 4 more concave edges and we can determine which vertices are concave and which are convex.
// A rectangle is invalid if:
    // the rectangle contains any points strictly inside it
    // the rectangle contains any concave vertices on the edge of it but not on the corner.
    // the points defining the corners of the shape are orinetataed in correctly with respect to the rectangle.
    // If any of the edges of the rectangle intersect an edge of the shape.
    // if an edge of the shape completely bisects the rectangle. (edge case when the points of the edge lie on the edges of the rectangle)
    
    
long calcSize(vector<long> pointA, vector<long> pointB){
    /**
     * Calculates the size of the rectangle defined by the two points.
     * 
     * @param pointA - the first coordinate.
     * @param pointB - the second coordinate.
     * 
     * @returns the size of the rectangle.
     */
    return (abs(pointA[0] - pointB[0])+1) * (abs(pointA[1] - pointB[1])+1);
}


int calcCrossProduct(vector<long> x, vector<long> y, vector<long> z){
    /**
     * Calculates the cross product of the two line vectors AB and BC.
     * 
     * @param pointA - point A.
     * @param pointB - point B.
     * @param pointB - point C.
     * 
     * @returns the cross product.
     */

    // x and y will share a coordinate so we can add the differences for each axis and get the distance between them regardless
    // same with y and z
    if((x[0]-y[0])*(z[1]-y[1])-(x[1]-y[1])*(z[0]-y[0]) > 0 ){
        return 1;
    }else{
        return -1;
    }
}

bool checkStrictlyInBox(int minX, int maxX, int minY, int maxY, vector<long> point){
    /**
     * Checks if a point lies strictly within the rectangle.
     * 
     * @param minX - the minimum x value of the rectangle.
     * @param maxX - the maximum x value of the rectangle.
     * @param minY - the minimum y value of the rectangle.
     * @param maxY - the maximum y value of the rectangle.
     * @param point - the point being checked.
     * 
     * @returns true if the point lies within the rectangle. False otheriwse.
     */

    
    // checks if point lies strictly within the bounding box
    if(point[0] > minX && point[0] < maxX && point[1] > minY && point[1] < maxY){
        return true;
    }
    return false;

}

bool checkStrictlyOnEdges(int minX, int maxX, int minY, int maxY, vector<long> point){
    /**
     * Checks if a point lies strictly on the edges of the rectangle but not the corners.
     * 
     * @param minX - the minimum x value of the rectangle.
     * @param maxX - the maximum x value of the rectangle.
     * @param minY - the minimum y value of the rectangle.
     * @param maxY - the maximum y value of the rectangle.
     * @param point - the point being checked.
     * 
     * @returns true if the point lies within the edges of the rectangle. False otheriwse.
     */

    // to lie on an edge, the line must either equal minX or maxX and lie in the range (minY, maxY) or vice versa
    if((point[0] == minX || point[0] == maxX) && (point[1] > minY && point[1] < maxY)){
        return true;
    }
    if((point[1] == minY || point[1] == maxY) && (point[0] > minX && point[0] < maxX)){
        return true;
    }
    return false;
}


bool checkIntersection(long minX, long maxX, long minY, long maxY, vector<long> linePoint1, vector<long> linePoint2){
    /**
     * Checks if a line defined by two points intersects any of the edges of the rectangle.
     * 
     * @param minX - the minimum x value of the rectangle.
     * @param maxX - the maximum x value of the rectangle.
     * @param minY - the minimum y value of the rectangle.
     * @param maxY - the maximum y value of the rectangle.
     * @param point1 - the first point of the line.
     * @param point1 - the second point of the line.
     * 
     * @returns true if any of the edges intersect the lines. False otherwise.
     */

    // we can guarantee that the lines are either horizontal or vertical by the problem's restrictions
    long maxLine, minLine;
    // if line is vertical we want to check if it intersects the horizontal lines of the rectangle 
    if(linePoint1[0] == linePoint2[0]){
        
        if(linePoint1[1] > linePoint2[1]){
            minLine = linePoint2[1];
            maxLine = linePoint1[1];
        }else{
            minLine = linePoint1[1];
            maxLine = linePoint2[1];
        }

        // the lines will intersect if the x coordinate of the line lies between the x coordinates of the edge and if the y coordinate of the edge lies between the x coordinates of the line
        if((linePoint1[0] > minX && linePoint1[0] < maxX) && ((minY <  maxLine && minY > minLine) || (maxY <  maxLine && maxY > minLine))){
            return true;
        }

    }
    // else if it's horizontal we do similar.
    else{
                
        if(linePoint1[0] > linePoint2[0]){
            minLine = linePoint2[0];
            maxLine = linePoint1[0];
        }else{
            minLine = linePoint1[0];
            maxLine = linePoint2[0];
        }

        // the lines will intersect if the x coordinate of the line lies between the x coordinates of the edge and if the y coordinate of the edge lies between the x coordinates of the line
        if((linePoint1[1] > minY && linePoint1[1] < maxY) && ((minX <  maxLine && minX > minLine) || (maxX <  maxLine && maxX > minLine))){
            return true;
        }

    }
    return false;

}

bool checkIfValid(vector <vector <long> > points,vector <int> vertexTypes, vector<vector<int> > vertexDirs, int pointIndex1,int pointIndex2){
    /**
     * Checks if the rectangle defined by two points is a valid rectangle.
     * 
     * @param points - the points.
     * @param vertexTypes - a vector correlating to the type of each vertex, -1 for concave and 1 for convex.
     * @param vertexDirs - a vector correlating to the direction of each angle (e.g. the top left corner of a square would have the value "(1,-1)").
     * @param pointIndex1 - the index of the first point.
     * @param pointIndex1 - the index of the second point.
     * 
     * @returns true if the rectangle is valid. False otherwise.
     */

    vector <long> point1 = points[pointIndex1];
    vector <long> point2 = points[pointIndex2];
    int point1Type = vertexTypes[pointIndex1];
    int point2Type = vertexTypes[pointIndex2];
    vector <int> point1Dir = vertexDirs[pointIndex1];
    vector <int> point2Dir = vertexDirs[pointIndex2];
    
    // first find the bounding box
    long minX, minY, maxX, maxY;

    // in theis segment, we are checking the direction of each corner. 
    // for a concave vertex, only 90º around the point lies within the shape and so the orientation must line up with where it lies on the rectangle created.

    if(point1[0] < point2[0]){
        minX = point1[0];
        maxX = point2[0];
        // if the vertex is concave it must point to the right
        if((point1Dir[0] == 1 && point1Type != 1)){

            return false;
        }
        // if the vertex is concave it must point to the left
        if((point2Dir[0] == -1 && point2Type != 1)){

            return false;
        }

    }else if(point1[0] > point2[0]){
        maxX = point1[0];
        minX = point2[0];

        // if the vertex is concave it must point to the left
        if((point1Dir[0] == -1 && point1Type != 1)){

            return false;
        }
        // if the vertex is concave it must point to the right
        if((point2Dir[0] == 1 && point2Type != 1)){

            return false;
        }
    }else{
        maxX = point1[0];
        minX = point2[0];
    }

    if(point1[1] < point2[1]){

        minY = point1[1];
        maxY = point2[1];
        // if the vertex is concave it must point up
        if((point1Dir[1] == 1 && point1Type != 1)){

            return false;
        }
        // if the vertex is concave it must point down
        if((point2Dir[1] == -1 && point2Type != 1)){

            return false;
        }

    }else if (point1[1] > point2[1]){
        maxY = point1[1];
        minY = point2[1];
        // if the vertex is concave it must point down
        if((point1Dir[1] == -1 && point1Type != 1)){

            return false;
        }
        // if the vertex is concave it must point up
        if((point2Dir[1] == 1 && point2Type != 1)){

            return false;
        }

    }else{
        maxY = point1[1];
        minY = point2[1];
    }

    
    // for a convex vertex, 90º around the point lies outside the shape 
    // in this case, we need these convex edges to not point in a specific direction. If they do then we return false
    if(point1Type == 1){
        if( point1[0] == minX && point1[1] == minY && point1Dir[0] == 1 && point1Dir[1] == 1){
            // bottom left corner
            return false;
        }
        if( point1[0] == maxX && point1[1] == minY && point1Dir[0] == -1 && point1Dir[1] == 1){
            // bottom right corner
            return false;
        }
        if( point1[0] == minX && point1[1] == maxY && point1Dir[0] == 1 && point1Dir[1] == -1){
            // top left corner
            return false;
        }
        if( point1[0] == maxX && point1[1] == maxY && point1Dir[0] == -1 && point1Dir[1] == -1){
            // bottom left corner
            return false;
        }
    }
    if(point2Type == 1 ){
        if( point2[0] > point1[0] && point2[1] < point1[1] && point2Dir[0] == 1 && point2Dir[1] == 1){
            // bottom left corner
            return false;
        }
        if( point2[0] > point1[0] && point2[1] < point1[1] && point2Dir[0] == -1 && point2Dir[1] == 1){
            // bottom right corner
            return false;
        }
        if( point2[0] < point1[0] && point2[1] > point1[1] && point2Dir[0] == 1 && point2Dir[1] == -1){
            // top left corner
            return false;
        }
        if( point2[0] > point1[0] && point2[1] > point1[1] && point2Dir[0] == -1 && point2Dir[1] == -1){

            // top right corner
            return false;
        }
    }

    for(int i = 0; i < points.size(); i++){
        // invalid if any points are strictly inside the bounding box
        if(checkStrictlyInBox(minX, maxX, minY, maxY, points[i])){

            return false;
        }

        // also invalid if any CONCAVE points lie on the edges but NOT the corners
        // We can prove this by defining concave points as points where 90º around the point lie within the shape and 
        // defining convex points as points where 270º around the point lie within the shape
        // SO if a line of the rectangle passes through the concave point then obviously since only 90º of around that point lie within the shape
        // and we need at least 180º around the line to lie within the shape, it is impossible for that to be the case.

        // if concave, check if STRICTLY on edge any edge defined
        if(vertexTypes[i] == -1){
            if(checkStrictlyOnEdges(minX, maxX, minY, maxY, points[i])){
                return false;
            }
        }
    }

    // next to determine if there's an intersection between the edges of the rectangle and the edges of the shape
    // if there is any intersection we can dismiss the rectangle.

    int linePoint2;
    linePoint2 = points.size()-1;
    for(int linePoint1 = 0; linePoint1 < points.size(); linePoint1++){
        if(checkIntersection(minX, maxX, minY, maxY, points[linePoint1], points[linePoint2])){
            return false;
        }
        linePoint2 = linePoint1;
    }

    
    return true;

}

vector<int> findPointDirection(vector<long> point1, vector<long> point2, vector<long> point3){
    /**
     * Calculates the direction of the corner defined by two lines AB and BC.
     * 
     * @param point1 - point A.
     * @param point2 - point B.
     * @param point3 - point C.
     * 
     * @returns the direction of the corner.
     */

    vector<int> direction;
    if((point2[0]- point3[0]) + (point2[0]-point1[0]) < 0){
        direction.push_back(-1);
    }
    else{
        direction.push_back(1);
    }
    if((point2[1]- point3[1]) + (point2[1] - point1[1]) < 0){
        direction.push_back(-1);
    }
    else{
        direction.push_back(1);
    }
    return direction;
}

int main(){
    vector <vector <long> > points;   

    for(string line; getline(input, line);){
        vector <long> row;
        string workingString = "";
        for(int i = 0; i < line.length(); i++){
            if(line[i] == ',' or i == line.length()-1){
                if(i == line.length()-1){
                    workingString.append(string() + line[i]);
                }
                row.push_back(stoi(workingString));
                workingString = "";
            }
            else{
                workingString.append(string() + line[i]);
            }
        }
        points.push_back(row);
    }

    long maxVal = 0;
    long currentVal;
    for(int i = 0; i < points.size();i++){
        vector<float> distances;
        for(int j = i+1; j < points.size(); j++){
            currentVal = calcSize(points[i], points[j]);
            if(currentVal > maxVal){
                maxVal = currentVal;
            }
        }
    }

    cout << "Part one output:\n";
    cout << maxVal;
    cout << "\n";

    // split the nodes into concave and convex
    vector <int> vertexTypes;
    vector<vector<int> > vertexDirs;
    int prevPos = points.size()-1;
    int currentPos = 0;
    int nextPos = 1;
    for(int i = 0; i < points.size()-1;i++){
        
        vertexTypes.push_back(calcCrossProduct(points[prevPos], points[currentPos], points[nextPos]));
        vertexDirs.push_back(findPointDirection(points[prevPos], points[currentPos], points[nextPos]));

        prevPos = currentPos;
        currentPos = nextPos;
        nextPos++;
    }
    prevPos = points.size()-2;
    currentPos = points.size()-1;
    nextPos = 0;
    vertexTypes.push_back(calcCrossProduct(points[prevPos], points[currentPos], points[nextPos]));
    vertexDirs.push_back(findPointDirection(points[prevPos], points[currentPos], points[nextPos]));

    // cout << vertexTypes[currentPos];
    int convOrConc = 0;
    for(int i = 0; i < vertexTypes.size(); i++){
        convOrConc += vertexTypes[i];
        cout << i;
        cout << " (";
        cout << vertexDirs[i][0];
        cout << ", ";
        cout << vertexDirs[i][1];
        cout << ")\n";
    }
    convOrConc = convOrConc/4;

    // we want convex to be positive and concave to be negative
    if(convOrConc > 0){
        for(int i = 0; i < vertexTypes.size(); i++){
            vertexTypes[i] *=-1;
        }
    }

    // now we look for the max value;
    maxVal = 0;
    int nValid = 0;
    for(int i = 0; i < points.size();i++){
        vector<float> distances;
        for(int j = i+1; j < points.size(); j++){

            // check if the pair is valid
            if(checkIfValid(points, vertexTypes, vertexDirs, i, j)){
                currentVal = calcSize(points[i], points[j]);
                nValid ++;
                if(currentVal > maxVal){
                    maxVal = currentVal;
                }
            }
        }
    }
    cout << "\n";
    cout << nValid;
    cout << "\nPart 2 output:\n";
    cout << maxVal;
}

