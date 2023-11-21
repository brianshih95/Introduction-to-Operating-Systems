#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>
#include <cstring>

using namespace std;

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;

mutex count_mutex;

inline void solve(const int index, const uint64_t current, uint64_t &local_count) {
    if (index == m){
        if (current == (one << n) - 1)
            local_count++;
    }
    else {
        if (current)
            solve(index + 1, current, local_count);
        solve(index + 1, current | subsets[index], local_count);
    }
}

void solve_thread(const int &start, const int &end) {
    uint64_t local_count = 0;

    for (int i = start; i < end; ++i)
        solve(i, 0, local_count);

    lock_guard<mutex> lock(count_mutex);
    global_count += local_count;
}

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> m;

    subsets.resize(m);
    for (int i = 0; i < m; i++) {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++) {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }

    int num_threads = 1;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            num_threads = stoi(argv[i + 1]);
            break;
        }
    }

    thread threads[num_threads];
    int piece = int(pow(2, num_threads)) - 1;
    int range = max((m / piece), 1);

    int start, end = 0;
    for (int i = 0; i < num_threads; i++) {
        start = end;
        end = min(m, range * int(pow(2, i + 1)) - 1);
        threads[i] = thread(solve_thread, start, end);
    }

    for (auto &t : threads)
        t.join();

    cout << global_count << endl;
    
    return 0;
}
