#include "costs.h"
#include "trailblazer.h"

using namespace std;

Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;

    if (isPathExistDFS(start, end, path)) return path;

    path.clear();
    return path;
}

Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    Queue<Vertex*> queue;
    Vector<Vertex*> path;
    Map<Vertex*, Vector<Vertex*>> map;

    if (start != NULL) {
        queue.enqueue(start);
        Vector<Vertex*> vec(1, start);
        map[start] = vec;
    }

    while (!queue.isEmpty()) {
        Vertex* vertex = queue.dequeue();
        vertex->setColor(GREEN);
        vertex->visited = true;
        if (vertex == end) return map[vertex];

        for (Edge* edge : vertex->edges) {
            if (!map.containsKey(edge->finish)) {
                queue.enqueue(edge->finish);
                edge->finish->setColor(YELLOW);

                Vector<Vertex*> vec = map[vertex];
                vec.add(edge->finish);
                map[edge->finish] = vec;
            }
        }
    }
    return path;
}

Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    PriorityQueue<Vertex*> pqueue;

    start->setColor(YELLOW);
    start->previous = NULL;
    start->cost = 0;
    pqueue.enqueue(start, start->cost);

    while (!pqueue.isEmpty()) {

        start = pqueue.dequeue();
        start->setColor(GREEN);
        if (start == end) return returnPath(end);

        for (Edge* edge : start->edges) {

            if (edge->finish->getColor() == 0) {

                edge->finish->setColor(YELLOW);
                edge->finish->cost = start->cost + edge->cost;
                edge->finish->previous = start;
                pqueue.enqueue(edge->finish, edge->finish->cost);

            } else if ((edge->finish->getColor() == YELLOW) && (edge->finish->cost > (start->cost + edge->cost))) {

                edge->finish->cost = start->cost + edge->cost;
                edge->finish->previous = start;
                pqueue.changePriority(edge->finish, edge->finish->cost);
            }
        }
    }
    Vector<Vertex*> path;
    return path;
}

Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    PriorityQueue<Vertex*> pqueue;

    start->setColor(YELLOW);
    start->previous = NULL;
    start->cost = 0;
    pqueue.enqueue(start, start->cost + heuristicFunction(start, end));

    while (!pqueue.isEmpty()) {

        start = pqueue.dequeue();
        start->setColor(GREEN);
        if (start == end) return returnPath(end);

        for (Edge* edge : start->edges) {

            if (edge->finish->getColor() == 0) {

                edge->finish->setColor(YELLOW);
                edge->finish->cost = start->cost + edge->cost;
                edge->finish->previous = start;
                pqueue.enqueue(edge->finish, edge->finish->cost + heuristicFunction(edge->finish, end));

            } else if ((edge->finish->getColor() == YELLOW) && (edge->finish->cost > (start->cost + edge->cost))) {

                edge->finish->cost = start->cost + edge->cost;
                edge->finish->previous = start;
                pqueue.changePriority(edge->finish, edge->finish->cost + heuristicFunction(edge->finish, end));
            }
        }
    }
    Vector<Vertex*> path;
    return path;
}

Set<Edge*> kruskal(BasicGraph& graph) {

    Set<Edge*> mst;
    PriorityQueue<Edge*> pqueue;
    Vector<Set<Vertex*>> vec;

    for (Vertex* node : graph.getNodeSet()) {
        Set<Vertex*> newset;
        newset.add(node);
        vec.add(newset);
    }

    for (Edge* edge : graph.getEdgeSet()) pqueue.enqueue(edge, edge->cost);

    while (vec.size() > 1) {
        Edge* edge = pqueue.dequeue();
        int start_cluster = -1, finish_cluster = -1;

        for (int i = 0; i < vec.size(); i++) {
            if ((start_cluster == -1) && (vec[i].contains(edge->start))) {
                start_cluster = i;
                if (finish_cluster != -1) break;
            }
            if ((finish_cluster == -1) && (vec[i].contains(edge->finish))) {
                finish_cluster = i;
                if (start_cluster != -1) break;
            }
        }

        if (start_cluster != finish_cluster) {
            vec[min(start_cluster, finish_cluster)] += vec[max(start_cluster, finish_cluster)];
            vec.remove(max(start_cluster, finish_cluster));
            mst.add(edge);
        }
    }
    return mst;
}

bool isPathExistDFS(Vertex*& start, Vertex*& finish, Vector<Vertex*>& visited) {

    if (start->visited) return false;

    if (start == finish) {
        visited.add(finish);
        finish->visited = true;
        return true;
    }

    visited.add(start);
    start->setColor(GREEN);
    start->visited = true;

    for (Edge* edge : start->edges) {

        if (isPathExistDFS(edge->finish, finish, visited)) return true;
    }

    visited.remove(visited.size() - 1);
    start->setColor(GRAY);

    return false;
}

Vector<Vertex*> returnPath(Vertex* end) {

    Vector<Vertex*> path;

    do {
        path.add(end);
        end = end->previous;
    } while (end != NULL);

    Vector<Vertex*> reversed;

    for (int i = path.size() - 1; i >= 0; i--) reversed.add(path[i]);

    return reversed;
}

