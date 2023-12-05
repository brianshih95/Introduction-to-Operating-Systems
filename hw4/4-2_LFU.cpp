#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#define pii pair<int, int>

using namespace std;

struct Compare {
    int target;
    Compare(int data): target(data) {}
    bool operator()(const pii &element) {
        return element.first == target;
    }
};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int num_block, k, num_data, cache_miss = 0;
    cin >> num_block >> k >> num_data;
    int num_set = num_block / k;

    vector<vector<pii>> cache(num_set, vector<pii>(k, {-1, INT_MAX}));

    for (int i = 0; i < num_data; i++) {
        int data;
        cin >> data;

        vector<pii>::iterator it;
        bool found = false;

        for (int j = 0; j < num_set; j++) {
            auto tmp = find_if(cache[j].begin(), cache[j].end(), Compare(data));
            if (tmp != cache[j].end()) {
                found = true;
                it = tmp;
                break;
            }
        }

        if (!found) {
            cache_miss++;
            vector<pii> current_set = cache[i % num_set];
            if (find_if(current_set.begin(), current_set.end(), Compare(-1)) == current_set.end()) {
                int remove_data = -1, min_times = INT_MAX;
                for (auto &[data, times]: current_set) {
                    if (times < min_times) {
                        remove_data = data;
                        min_times = times;
                    }
                }
                auto removed = find_if(cache[i % num_set].begin(), cache[i % num_set].end(), Compare(remove_data));
                *removed = {-1, INT_MAX};
            }

            for (int j = 0; j < k; j++) {
                if (cache[i % num_set][j].first == -1) {
                    cache[i % num_set][j] = {data, 1};
                    break;
                }
            }
        }
        else
            it->second++;
    }

    cout << "Total Cache Misses:" << cache_miss << endl;

    return 0;
}