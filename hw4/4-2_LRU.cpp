#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int num_block, k, num_data, cache_miss = 0;
    cin >> num_block >> k >> num_data;
    int num_set = num_block / k;

    vector<list<int>> cache(num_set);

    for (int i = 0; i < num_data; i++) {
        int data;
        cin >> data;

        int set;
        bool found = false;

        for (int j = 0; j < num_set; j++) {
            if (find(cache[j].begin(), cache[j].end(), data) != cache[j].end()) {
                found = true;
                set = j;
                break;
            }
        }

        if (!found) {
            cache_miss++;
            if (cache[i % num_set].size() == k)
                cache[i % num_set].pop_front();
            cache[i % num_set].push_back(data);
        }
        else {
            cache[set].remove(data);
            cache[set].push_back(data);
        }
    }

    cout << "Total Cache Misses:" << cache_miss << endl;

    return 0;
}