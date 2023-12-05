#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int num_block, num_data, cache_miss = 0;
    cin >> num_block >> num_data;

    vector<int> cache(num_block, -1);

    for (int i = 0; i < num_data; i++) {
        int data;
        cin >> data;
        if (find(cache.begin(), cache.end(), data) == cache.end()) {
            cache_miss++;
            cache[i % num_block] = data;
        }
    }

    cout << "Total Cache Misses:" << cache_miss << endl;

    return 0;
}