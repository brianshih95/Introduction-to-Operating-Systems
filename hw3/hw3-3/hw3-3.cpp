#include <iostream>
#include <thread>
#include <mutex>
#include <cstring>

using namespace std;

int n, global_count = 0;
mutex count_mutex;

inline bool is_prime(const int &num) {
    if (num == 1)
        return false;

    for (int i = 2; i * i <= num; ++i)
        if (num % i == 0)
            return false;
    return true;
}

void find_prime(const int &start1, const int &end1, const int &start2, const int &end2, const int &start3, const int &end3) {
    int count = 0;
    for (int i = start1; i <= end1; ++i)
        if (is_prime(i))
            ++count;

    for (int i = start2; i <= end2; ++i)
        if (is_prime(i))
            ++count;

    for (int i = start3; i >= end3; --i)
        if (is_prime(i))
            ++count;

    lock_guard<mutex> lock(count_mutex);
    global_count += count;
}

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    cin >> n;

    int num_threads = 1;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            num_threads = stoi(argv[i + 1]);
            break;
        }
    }

    int range = n / num_threads / 2;
    int last = num_threads * 2 * range;
    int remain = n - last;
    thread threads[num_threads];

    for (int i = 0; i < num_threads; ++i) {
        int start1 = i * range + 1;
        int end1 = (i + 1) * range;
        int start2 = (num_threads * 2 - i - 1) * range + 1;
        int end2 = (num_threads * 2 - i) * range;
        
        int start3 = 1, end3 = 1;
        if (remain > 0) {
            start3 = last + remain;
            end3 = max(last + 1, last + remain - 1);
            remain -= 2;
        }

        threads[i] = thread(find_prime, start1, end1, start2, end2, start3, end3);
    }

    for (auto &thread : threads)
        thread.join();

    cout << global_count << '\n';

    return 0;
}
