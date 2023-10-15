#include <iostream>
#include <vector>
#include <queue>

#define pii pair<int, int>
#define six_int pair<pii, pair<pii, pii>>
#define endl '\n'
#define IOS()                     \
    ios_base::sync_with_stdio(0); \
    cin.tie(0)

using namespace std;

struct Process
{
    int arrival;
    int remaining;
    int finish;
};

class Compare
{
public:
    bool operator()(six_int a, six_int b)
    {
        if (a.second.first.second > b.second.first.second)  // queue_idx
            return true;
        else if (a.second.first.second == b.second.first.second)
        {
            if (a.second.second.first == 0 && b.second.second.first == 0)   // FCFS
            {
                if (a.second.second.second > b.second.second.second)    // queue_time
                    return true;
                return false;
            }
            if (a.second.second.first == 1 && b.second.second.first == 1)   // SRTF
            {
                if (a.first.first > b.first.first)  // remain
                    return true;
                else if (a.first.first == b.first.first && a.first.second > b.first.second) // arrival
                    return true;
                return false;
            }
            if (a.second.second.first == 2 && b.second.second.first == 2)   // RR
            {
                if (a.second.second.second > b.second.second.second)    // queue_time
                    return true;
                else if (a.second.second.second == b.second.second.second && a.first.second < b.first.second)   // arrival
                    return true;
                return false;
            }
        }
        return false;
    }
};

void add_to_queue(int n, int remain, int arrival, int idx, int queue_idx, int queue_type, int queue_time, vector<pii> &queue_info, priority_queue<six_int, vector<six_int>, Compare>& pq)
{
    if (queue_idx < n - 1) // go to the next level priority queue (if available)
    {
        queue_idx++;
        queue_type = queue_info[queue_idx].first;
    }
    pq.push({{remain, arrival}, {{idx, queue_idx}, {queue_type, queue_time}}});
}

int main()
{
    IOS();

    int n, m;
    cin >> n >> m;
    vector<pii> queue_info(n);
    vector<Process> process_info(m);
    priority_queue<six_int, vector<six_int>, Compare> pq;
    for (int i = 0; i < n; i++)
    {
        int mode, time_quantum;
        cin >> mode >> time_quantum;
        queue_info[i] = {mode, time_quantum};
    }
    
    for (int i = 0; i < m; i++)
    {
        int arrival_time, bust_time;
        cin >> arrival_time >> bust_time;
        process_info[i].arrival = arrival_time;
        process_info[i].remaining = bust_time;
    }

    int done = 0, begin = 0, current_time = 0;
    while (done < m)
    {
        for (int i = begin; i < m; i++)
        {
            if (process_info[i].arrival <= current_time)    // process arrived
            {
                pq.push({{process_info[i].remaining, process_info[i].arrival}, {{i, 0}, {queue_info[0].first, process_info[i].arrival}}});
                begin++;
            }
        }

        if (pq.empty())     // no process
        {
            current_time++;
            continue;
        }

        six_int cur = pq.top();
        pq.pop();

        int remain = cur.first.first, arrival = cur.first.second, idx = cur.second.first.first, queue_idx = cur.second.first.second, queue_type = cur.second.second.first, queue_time = cur.second.second.second;
        if (queue_type == 0)    // FCFS
        {
            if (queue_idx != 0) // may be preempted
            {
                bool flag = false;  // process finished
                while (1)
                {
                    current_time++;
                    remain--;
                    if (remain == 0)
                    {
                        done++;
                        flag = true;
                        process_info[idx].finish = current_time;
                        break;
                    }
                    if (begin < m && current_time >= process_info[begin].arrival)   // be preempted by higher priority queue process
                        break;
                }
                if (!flag)
                    add_to_queue(n, remain, arrival, idx, queue_idx, queue_type, current_time, queue_info, pq);
            }
            else
            {
                done++;
                current_time += remain;
                process_info[idx].finish = current_time;
            }
        }
        else if (queue_type == 1)   // SRTF
        {
            current_time++;
            remain--;
            if (remain == 0)
            {
                done++;
                process_info[idx].finish = current_time;
            }
            else
                add_to_queue(n, remain, arrival, idx, queue_idx, queue_type, current_time, queue_info, pq);
        }
        else if (queue_type == 2)   // RR
        {
            int time_quantum = queue_info[queue_idx].second;
            if (queue_idx != 0) // may be preempted
            {
                bool flag = false;
                for (int i = 0; i < time_quantum; i++)
                {
                    current_time++;
                    remain--;
                    if (remain == 0)
                    {
                        done++;
                        flag = true;
                        process_info[idx].finish = current_time;
                        break;
                    }
                    if (begin < m && current_time >= process_info[begin].arrival) // be preempted by higher priority queue process
                        break;
                }
                if (!flag)
                    add_to_queue(n, remain, arrival, idx, queue_idx, queue_type, current_time, queue_info, pq);
            }
            else
            {
                if (remain <= time_quantum)
                {
                    done++;
                    current_time += remain;
                    process_info[idx].finish = current_time;
                }
                else
                {
                    current_time += time_quantum;
                    remain -= time_quantum;
                    add_to_queue(n, remain, arrival, idx, queue_idx, queue_type, current_time, queue_info, pq);
                }
            }
        }
    }

    int total_wait = 0, total_turnaround = 0;
    for (const auto &p : process_info)
    {
        int wait = p.finish - p.remaining - p.arrival;
        int turnaround = p.finish - p.arrival;
        total_wait += wait;
        total_turnaround += turnaround;
        cout << wait << ' ' << turnaround << endl;
    }
    cout << total_wait << endl << total_turnaround << endl;

    return 0;
}