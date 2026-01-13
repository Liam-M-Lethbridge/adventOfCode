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
// a vertex is concave if it "turns inwards". A square has 4 concave vertices. 
// with every new convex edge, a concave edge would be needed to eventually complete the shape and vice vera.
// We can figure out the distinction between convex and concave because one turns clockwise and the other anticlockwise.
// Hence there will be 4 more concave edges.
// A square is invalid if:
    // the square contains any verticies not on the edge of it.
    // the square contains any convex vertices on the edge of it but not on the corner.
    // the square contains any concave vertices on the corner of it.
    
    
// How do we find which vertices are convex and which are concave? With any shape, there will be 4 more concave vertices than convex vertices
long calcSize(vector<long> pointA, vector<long> pointB){

    return (abs(pointA[0] - pointB[0])+1) * (abs(pointA[1] - pointB[1])+1);
}


int calcCrossProduct(vector<long> x, vector<long> y, vector<long> z){

    // x and y will share a coordinate so we can add the differences for each axis and get the distance between them regardless
    // same with y and z
    if((x[0]-y[0])*(z[1]-y[1])-(x[1]-y[1])*(z[0]-y[0]) > 0 ){
        return 1;
    }else{
        return -1;
    }
}

bool checkStrictlyInBox(int minX, int maxX, int minY, int maxY, vector<long> point){
    // checks if point lies strictly within the bounding box
    if(point[0] > minX && point[0] < maxX && point[1] > minY && point[1] < maxY){
        return true;
    }
    return false;

}

bool checkOnCorners(int minX, int maxX, int minY, int maxY, vector<long> point){
    if((point[0] == minX || point[0] == maxX) && (point[1] == minY || point[1] == maxY)){
        return true;
    }
    return false;
}

bool checkStrictlyOnEdges(int minX, int maxX, int minY, int maxY, vector<long> point){

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

        // there is also an edge case where a line lies between two opposite edges:
        //  #XO OX#    #XO OX#
        //  XXX XXX    XXO OXX
        //  XXXXXXX    XX---XX
        //  XXXXXXX -> XXOOOXX
        //  XXXXXXX    XXOOOXX
        //  #XOXOX#    #XOOOX#
        //    XXX        XXX
        //    #X#        #X#
        // the line --- lies between two edges and we end up with invalid rectangle

        // to determine this line, we check if shared coordinate lies in the min and max, and that the end points of the line equal the edge
        // we check that the ends of the line meet the two perpendicular edges of the rectangle and lie between the parallel edges of the rectangle
        if((linePoint1[0] > minX && linePoint1[0] < maxX)&&(minY == minLine && maxY == maxLine)){
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
        if((linePoint1[1] > minY && linePoint1[1] < maxY)&&(minX == minLine && maxX == maxLine)){
            return true;
        }
    }
    return false;

}

bool checkIfValid(vector <vector <long> > points,vector <int> vertexTypes, vector<vector<int> > vertexDirs, int pointIndex1,int pointIndex2){

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
    


    // really the only reliable way to check is to determine if there's an intersection between the edges of the rectangle and the edges of the shape
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
        // cout << prevPos;
        // cout << " ";
        // cout << currentPos;
        // cout << " ";
        // cout << nextPos;
        // cout << " ";


        
        vertexTypes.push_back(calcCrossProduct(points[prevPos], points[currentPos], points[nextPos]));
        vertexDirs.push_back(findPointDirection(points[prevPos], points[currentPos], points[nextPos]));
        // cout << vertexTypes[currentPos];
        // cout << "\n";
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
    for(int i = 0; i < points.size();i++){
        vector<float> distances;
        for(int j = i+1; j < points.size(); j++){

            // check if the pair is valid
            if(checkIfValid(points, vertexTypes, vertexDirs, i, j)){
                currentVal = calcSize(points[i], points[j]);

                cout << i;
                cout << " and ";
                cout << j;
                cout << " make a valid rectangle.";
                cout << "\n";

                if(currentVal > maxVal){
                    maxVal = currentVal;
                }
            }
        }
    }
    cout << "Part 2 output:\n";
    cout << maxVal;


}

