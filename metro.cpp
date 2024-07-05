#include <iostream>
#include <fstream>  
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <tuple>
#include <climits>

using namespace std;

class Graph {
    unordered_map<string, vector<pair<string, int>>> adjList;
public:
    void addVertex(const string& vertex) {
        adjList[vertex] = vector<pair<string, int>>();
    }

    void addEdge(const string& src, const string& dest, int fare) {
        adjList[src].push_back(make_pair(dest, fare));
    }

    pair<int, vector<string>> findCheapestRoute(const string& src, const string& dest, int k) {
        // Min-heap to store (fare, station, stops, path)
        priority_queue<tuple<int, string, int, vector<string>>, vector<tuple<int, string, int, vector<string>>>> pq;
        pq.push(make_tuple(0, src, 0, vector<string>{src}));

        // Map to keep track of the minimum fare to each station with up to k stops
        unordered_map<string, unordered_map<int, int>> minFare;

        while (!pq.empty()) {
            auto top = pq.top();
            pq.pop();
            int currentFare = get<0>(top);
            string currentStation = get<1>(top);
            int stops = get<2>(top);
            vector<string> path = get<3>(top);

            // If we have reached the destination
            if (currentStation == dest) return {currentFare, path};

            // If we have reached the stop limit, skip further exploration
            if (stops > k) continue;

            for (auto& neighbor : adjList[currentStation]) {
                auto nextStation = neighbor.first;
                int fare = neighbor.second;
                int newFare = currentFare + fare;
                auto newPath = path;
                newPath.push_back(nextStation);

                // If the new fare is cheaper or we haven't visited this station with the current stops count
                if (minFare[nextStation].find(stops + 1) == minFare[nextStation].end() || newFare < minFare[nextStation][stops + 1]) {
                    minFare[nextStation][stops + 1] = newFare;
                    pq.push(make_tuple(newFare, nextStation, stops + 1, newPath));
                }
            }
        }

        // If no valid path is found
        return {-1, {}};
    }
};

int main() {
    Graph g;
    g.addVertex("Noida Sector 62~B");
    g.addVertex("Botanical Garden~B");
    g.addVertex("Yamuna Bank~B");
    g.addVertex("Rajiv Chowk~BY");
    g.addVertex("Vaishali~B");
    g.addVertex("Moti Nagar~B");
    g.addVertex("Janak Puri West~BO");
    g.addVertex("Dwarka Sector 21~B");
    g.addVertex("Huda City Center~Y");
    g.addVertex("Saket~Y");
    g.addVertex("Vishwavidyalaya~Y");
    g.addVertex("Chandni Chowk~Y");
    g.addVertex("New Delhi~YO");
    g.addVertex("AIIMS~Y");
    g.addVertex("Shivaji Stadium~O");
    g.addVertex("DDS Campus~O");
    g.addVertex("IGI Airport~O");
    g.addVertex("Rajouri Garden~BP");
    g.addVertex("Netaji Subhash Place~PR");
    g.addVertex("Punjabi Bagh West~P");

    g.addEdge("Noida Sector 62~B", "Botanical Garden~B", 8);
    g.addEdge("Botanical Garden~B", "Yamuna Bank~B", 10);
    g.addEdge("Yamuna Bank~B", "Vaishali~B", 8);
    g.addEdge("Yamuna Bank~B", "Rajiv Chowk~BY", 6);
    g.addEdge("Rajiv Chowk~BY", "Moti Nagar~B", 9);
    g.addEdge("Moti Nagar~B", "Janak Puri West~BO", 7);
    g.addEdge("Janak Puri West~BO", "Dwarka Sector 21~B", 6);
    g.addEdge("Huda City Center~Y", "Saket~Y", 15);
    g.addEdge("Saket~Y", "AIIMS~Y", 6);
    g.addEdge("AIIMS~Y", "Rajiv Chowk~BY", 7);
    g.addEdge("Rajiv Chowk~BY", "New Delhi~YO", 1);
    g.addEdge("New Delhi~YO", "Chandni Chowk~Y", 2);
    g.addEdge("Chandni Chowk~Y", "Vishwavidyalaya~Y", 5);
    g.addEdge("New Delhi~YO", "Shivaji Stadium~O", 2);
    g.addEdge("Shivaji Stadium~O", "DDS Campus~O", 7);
    g.addEdge("DDS Campus~O", "IGI Airport~O", 8);
    g.addEdge("Moti Nagar~B", "Rajouri Garden~BP", 2);
    g.addEdge("Punjabi Bagh West~P", "Rajouri Garden~BP", 2);
    g.addEdge("Punjabi Bagh West~P", "Netaji Subhash Place~PR", 3);

    string source, destination;
    int k;

    cout << "Enter the source station: ";
    getline(cin, source);
    cout << "Enter the destination station: ";
    getline(cin, destination);
    cout << "Enter the maximum number of stops (k): ";
    cin >> k;
    cin.ignore(); // to consume the newline character left by cin

    // Call findCheapestRoute and capture return values
    pair<int, vector<string>> result = g.findCheapestRoute(source, destination, k);

    // Unpack the result
    int cheapestFare = result.first;
    vector<string> path = result.second;

    // Open metro.txt for writing
    ofstream outputFile("metro.txt");
    if (outputFile.is_open()) {
        if (cheapestFare != -1) {
            outputFile << "The cheapest fare from " << source << " to " << destination << " with at most " << k << " stops is: " << cheapestFare << endl;
            outputFile << "The path is:" << endl;
            for (const auto& station : path) {
                outputFile << station << endl;
            }
        } else {
            outputFile << "No route found from " << source << " to " << destination << " with at most " << k << " stops." << endl;
        }
        outputFile.close();
        cout << "Output has been written to metro.txt" << endl;
    } else {
        cout << "Unable to open metro.txt for writing!" << endl;
    }

    return 0;
}

