#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <limits>
#include <string>

using namespace std;

// Custom hash functor for pair<int, int>
struct PairHash {
    size_t operator()(const pair<int, int>& p) const {
        return hash<int>()(p.first) ^ hash<int>()(p.second);
    }
};

// Global Variables
int n; // number of attractions
vector<string> attractionNames;
vector<int> attractionDurations;
unordered_map<pair<int, int>, int, PairHash> travelDurations;
unordered_map<pair<int, int>, int, PairHash> memo;

// Recursive function to find shortest path using backtracking
int tspBacktrack(int curr, int mask) {
    if (mask == (1 << n) - 1) { // all attractions visited
        return travelDurations[{curr, 0}] + attractionDurations[0]; // return to starting point
    }

    if (memo.find({curr, mask}) != memo.end()) {
        return memo[{curr, mask}];
    }

    int minDuration = numeric_limits<int>::max();

    for (int next = 0; next < n; next++) {
        if (!(mask & (1 << next))) { // next attraction not visited
            int newMask = mask | (1 << next);
            int duration = travelDurations[{curr, next}] + attractionDurations[next] + tspBacktrack(next, newMask);

            minDuration = min(minDuration, duration);
        }
    }

    memo[{curr, mask}] = minDuration;
    return minDuration;
}

// Recursive function to print itinerary
void printItinerary(int curr, int mask) {
    if (mask == (1 << n) - 1) { // all attractions visited
        cout << "- " << attractionNames[0] << " (" << attractionDurations[0] << " minutes) -> ";
        return;
    }

    for (int next = 0; next < n; next++) {
        if (!(mask & (1 << next))) { // next attraction not visited
            int newMask = mask | (1 << next);
            int duration = travelDurations[{curr, next}] + attractionDurations[next];

            if (tspBacktrack(curr, mask) == duration + tspBacktrack(next, newMask)) {
                cout << "- " << attractionNames[next] << " (" << attractionDurations[next] << " minutes) -> ";
                printItinerary(next, newMask);
                break;
            }
        }
    }
}

int main() {
    cout << "Enter the number of attractions: ";
    cin >> n;

    attractionNames.resize(n);
    attractionDurations.resize(n);

    cout << "Enter attraction names and durations (in minutes):\n";
    for (int i = 0; i < n; i++) {
        cout << "Attraction " << i + 1 << " name: ";
        cin.ignore();
        getline(cin, attractionNames[i]);
        cout << "Attraction " << i + 1 << " duration (minutes): ";
        cin >> attractionDurations[i];
    }

    cout << "Enter travel durations between attractions:\n";
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) { // only need to input for one direction
            cout << "Duration from " << attractionNames[i] << " to " << attractionNames[j] << ": ";
            int duration;
            cin >> duration;
            travelDurations[{i, j}] = duration;
            travelDurations[{j, i}] = duration; // symmetric
        }
    }

    int minDuration = tspBacktrack(0, 1); // start from first attraction (0) and mask 1 for visited

    cout << "\nOptimized Itinerary:\n";
    printItinerary(0, 1); // start from first attraction (0) and mask 1 for visited
    cout << "\nTotal Duration: " << minDuration << " minutes\n";

    return 0;
}
