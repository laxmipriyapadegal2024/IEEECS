// Cache with Expiry - Time-Based Cache

#include <iostream>
#include <unordered_map>
#include <queue>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Cache {
    unordered_map<int, pair<string, long long>> data;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> expiryQueue;

    void cleanUp() {
        long long now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        while (!expiryQueue.empty() && expiryQueue.top().first <= now) {
            int key = expiryQueue.top().second;
            expiryQueue.pop();
            if (data.count(key) && data[key].second <= now) {
                data.erase(key);
            }
        }
    }

public:
    void set(int key, string value, int expirySeconds) {
        long long expiryTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() + expirySeconds * 1000;
        data[key] = {value, expiryTime};
        expiryQueue.push({expiryTime, key});
    }

    string get(int key) {
        cleanUp();
        return data.count(key) ? data[key].first : "Key not found";
    }
};

int main() {
    Cache cache;
    cache.set(1, "Hello", 3);
    cache.set(2, "World", 5);

    cout << cache.get(1) << endl;

    cout << "Enter any key to continue (simulating delay)..." << endl;
    string pause;
    cin >> pause;

    cout << cache.get(1) << endl;
    cout << cache.get(2) << endl;

    return 0;
}
