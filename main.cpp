#include <iostream>
#include <list>
#include <stack>

#include "graph.h"

namespace {
    template <typename T>
    std::ostream& operator <<(std::ostream& os, const std::list<T>& object) {
        for (const auto& it : object) {
            os << it << " ";
        }

        return os;
    }

    template <typename N>
    void dfs(const Graph<N>& g, std::vector<bool>& visited, size_t i) {
        //std::cout << i << " ";

        visited[i] = true;
        for (const auto& it : g[i]) {
            if (!visited[static_cast<size_t>(it)])
                dfs(g, visited, static_cast<size_t>(it));
        }
    }

    template <typename N>
    bool checkEulerPathUndirected(const UndirectedGraph<N>& g) {
        if (g.empty())
            return true;

        size_t oddVertex = 0;
        for (const auto& it : g)
            if (it.size() % 2 == 1)
                ++oddVertex;
        if (oddVertex > 2)
            return false;

        std::vector<bool> visited(g.number_of_verteces(), false);
        for (size_t i = 0, end_ = g.number_of_verteces(); i < end_; ++i) {
            if (!g[i].empty()) {
                dfs(g, visited, i);
                break;
            }
        }
        for (size_t i = 0, end_ = g.number_of_verteces(); i < end_; ++i)
            if (!g[i].empty() && !visited[i])
                return false;

        return true;
    }

    template <typename N>
    bool checkEulerPathDirected(const DirectedGraph<N>& g) {
        if (g.empty())
            return true;

        size_t incorrectness = 0;
        for (size_t i = 0, end_ = g.number_of_verteces(); i < end_; ++i) {
            int inDegree = 0;
            for (size_t j = 0; j < end_; ++j) {
                inDegree += std::count_if(g[j].begin(), g[j].end(), [i](const N& node) {
                    return i == node;
                });
            }

            if (std::abs(inDegree - static_cast<int>(g[i].size())) == 1)
                ++incorrectness;
            if (std::abs(inDegree - static_cast<int>(g[i].size())) > 1 || incorrectness > 2)
                return false;
        }

        std::vector<bool> visited(g.number_of_verteces(), false);
        for (size_t i = 0, end_ = g.number_of_verteces(); i < end_; ++i) {
            if (!g[i].empty()) {
                dfs(g, visited, i);
                break;
            }
        }
        for (size_t i = 0, end_ = g.number_of_verteces(); i < end_; ++i)
            if (!g[i].empty() && !visited[i])
                return false;

        return true;
    }

    template <typename N>
    std::vector<size_t> UndirectedEulerPath(UndirectedGraph<N> g) {
        size_t start = 0;
        for (size_t i = 0, end_ = g.number_of_verteces(); i < end_; ++i)
            if (g[i].size() % 2 == 1) {
                start = i;
                break;
            }

        std::vector<size_t> result;

        std::stack<size_t> stack;
        stack.push(start);
        while (!stack.empty()) {
            auto vert = stack.top();
            if (!g[vert].empty()) {
                stack.push(static_cast<size_t>(g[vert][0]));
                auto second_arg = g[vert][0];               // necessary because of changed state of g[vert][0] after first
                                                            // erase in remove_edge function, which receives the second argument
                                                            // as const reference
                g.remove_edge(vert, second_arg);
            }

            if (vert == stack.top()) {
                stack.pop();
                result.push_back(vert);
            }
        }

        return result;
    }

    template <typename N>
    std::list<size_t> DirectedEulerPath(DirectedGraph<N> g) {
        size_t start = 0;
        for (size_t i = 0, end_ = g.number_of_verteces(); i < end_; ++i) {
            size_t inDegree = 0;
            for (size_t j = 0; j < end_; ++j) {
                inDegree += std::count_if(g[j].begin(), g[j].end(), [i](const N& node) {
                    return i == node;
                });
            }

            if (g[i].size() - inDegree == 1) {
                start = i;
                break;
            }
        }


        std::list<size_t> result;

        std::stack<size_t> stack;
        stack.push(start);
        while (!stack.empty()) {
            auto vert = stack.top();
            if (!g[vert].empty()) {
                stack.push(static_cast<size_t>(g[vert][0]));
                auto second_arg = g[vert][0];               // necessary because of changed state of g[vert][0] after first
                                                            // erase in remove_edge function, which receives the second argument
                                                            // as const reference
                g.remove_edge(vert, second_arg);
            }

            if (vert == stack.top()) {
                stack.pop();
                result.push_front(vert);
            }
        }

        return result;
    }
}

int main() {
    std::cout << "Undirected graph" << std::endl;
    UndirectedGraph<size_t> undirected_g = generate_undirected_graph(4, 5);
    while (!checkEulerPathUndirected(undirected_g)) {
        undirected_g = generate_undirected_graph(4, 5);
    }
    std::cout << undirected_g << std::endl;
    std::cout << "The Eulerian path is: " << UndirectedEulerPath(undirected_g) << std::endl;

    std::cout << "Directed graph" << std::endl;
    DirectedGraph<size_t> directed_g = generate_directed_graph(4, 6);
    while (!checkEulerPathDirected(directed_g)) {
        directed_g = generate_directed_graph(4, 6);
    }
    std::cout << directed_g << std::endl;
    std::cout << "The Eulerian path is: " << DirectedEulerPath(directed_g) << std::endl;

    return 0;
}
