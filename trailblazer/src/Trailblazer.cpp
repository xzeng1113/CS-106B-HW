/* Name: Xiaoxi Zeng
 * Section Leader: Semir Shafi
 * File: Trailblazer.cpp
 * -------------------------
 * This program implements four path-finding functions, including
 * breath-first search, Dijkstra, A*, and an alternative route finding
 * algorithm.
*/

#include "Trailblazer.h"
#include "queue.h"
#include "vector.h"
#include "pqueue.h"
using namespace std;

static const double SUFFICIENT_DIFFERENCE = 0.2;
/*Function Prototypes*/
Set<RoadNode*> pathSet(Path path);
Path shortestPathHelper(const RoadGraph& graph, RoadNode* start, RoadNode* end, double heuristic, RoadEdge* toIgnore);

/* Function: breadthFirstSearch
 * Purpose: implement breadth firt search to find the shortest hops from
 * start to the end
*/
Path breadthFirstSearch(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    Path path = {};
    Set<RoadNode*> visitedNodes;
    Queue<Path> paths; //Create a to-do list
    path.add(start);
    visitedNodes.add(start);
    paths.enqueue(path); // Enqueue the first element
    start->setColor(Color::YELLOW);
    while(!paths.isEmpty()){
        Path currPath = paths.dequeue();
        RoadNode* currNode = currPath[(currPath.size() - 1)];
        currNode->setColor(Color::GREEN);
        if(currNode == end){
            return currPath;
        }else{
            for(RoadNode* node: graph.neighborsOf(currNode)){
                if(!visitedNodes.contains(node)){
                    visitedNodes.add(node);
                    Path newPath = currPath;
                    newPath += node;
                    paths.enqueue(newPath);
                    node->setColor(Color::YELLOW);
                }
            }
        }
    }
    return path;
}

/* Function: dijktraAlgorithm
 * Purpose: implement Dijkstra Algorithm to find the shortest
 * path from start to the end
*/
Path dijkstrasAlgorithm(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    return shortestPathHelper(graph, start, end, 0, nullptr);
}

/* Function: astarAlgorithm
 * Purpose: implement A* Algorithm to find the shortest
 * path from start to the end
*/
Path aStar(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    double heuristic = graph.crowFlyDistanceBetween(start, end) / graph.maxRoadSpeed();
    return shortestPathHelper(graph, start, end, heuristic, nullptr);
}

/* Function: alternativeRoute
 * Purpose: find the shortest alternative route which replaces one
 * edge at a time in the best route
*/
Path alternativeRoute(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    Path alternativePath = {};
    PriorityQueue<Path> alterPaths;
    Path bestPath = aStar(graph, start, end);
    Set<RoadNode*> bestPathNodes = pathSet(bestPath);
    for(int i = 0; i < bestPath.size() - 1; i ++){
        RoadEdge* toIgnore = graph.edgeBetween(bestPath[i], bestPath[i + 1]);
        double heuristic = graph.crowFlyDistanceBetween(start, end) / graph.maxRoadSpeed();
        Path potentialAlterPath = shortestPathHelper(graph, start, end, heuristic, toIgnore);
        Set<RoadNode*> potentialAlterNodes = pathSet(potentialAlterPath);
        Set<RoadNode*> diffNodes = potentialAlterNodes - bestPathNodes;
        double diff = (double)diffNodes.size() / bestPathNodes.size();
        if(diff > SUFFICIENT_DIFFERENCE){
            double pathCost = 0;
            for(int i = 0; i < potentialAlterPath.size() - 1; i ++){
                RoadEdge* currEdge = graph.edgeBetween(potentialAlterPath[i], potentialAlterPath[i + 1]);
                pathCost += currEdge->cost();
            }
            alterPaths.enqueue(potentialAlterPath, pathCost);
        }
    }
    alternativePath = alterPaths.dequeue();
    return alternativePath;
}

/* Function: shortestPathHelper
 * Purpose: a combined helper function to implement Dijkstra, A* and alternative route
 * based on different heuristic scenario and the value of toIgnore; when heristic equals
 * 0, implement Dijkstra; otherwise implement A*.
*/
Path shortestPathHelper(const RoadGraph& graph, RoadNode* start, RoadNode* end, double heuristic, RoadEdge* toIgnore){
    Path path = {};
    PriorityQueue<Path> paths; // Create a to-do list
    Set<RoadNode*> visitedNodes;
    Map<Path, double> pathCost;
    path.add(start);
    pathCost[path] = 0 + heuristic; // For Dijkstra, the heuristic is zero
    paths.enqueue(path, pathCost[path]);
    start->setColor(Color::YELLOW);
    while(!paths.isEmpty()){
        Path currPath = paths.dequeue();
        RoadNode* currNode = currPath[(currPath.size() - 1)];
        if(!visitedNodes.contains(currNode)){
            visitedNodes.add(currNode);
        }
        currNode->setColor(Color::GREEN);
        if(currNode == end){
            return currPath;
        }else{
            for(RoadNode* node: graph.neighborsOf(currNode)){
                if(!visitedNodes.contains(node)){
                    if(toIgnore != nullptr && graph.edgeBetween(currNode, node) == toIgnore){
                        continue;
                    }
                    double edgeCost = graph.edgeBetween(currNode, node)->cost();
                    Path newPath = currPath;
                    newPath += node;
                    if(heuristic != 0.0){ // Implement either Dijkstra or A* based on different heuristic value
                        double newHeuristic = graph.crowFlyDistanceBetween(node, end) / graph.maxRoadSpeed() -
                                graph.crowFlyDistanceBetween(currNode, end) / graph.maxRoadSpeed(); // The previous heuristic should be reset
                        double newCost = pathCost[currPath] + edgeCost + newHeuristic;
                        pathCost[newPath] = newCost;
                    }else{
                        double newCost = pathCost[currPath] + edgeCost;
                        pathCost[newPath] = newCost;
                    }
                    paths.enqueue(newPath, pathCost[newPath]);
                    node->setColor(Color::YELLOW);
                }
            }
        }
    }
    return path;
}

/* Function: pathSet
 * Purpose: return a set of roadnodes that are in a path
*/
Set<RoadNode*> pathSet(Path path){
    Set<RoadNode*> toReturn;
    for(RoadNode* node: path){
        toReturn.add(node);
    }
    return toReturn;
}
