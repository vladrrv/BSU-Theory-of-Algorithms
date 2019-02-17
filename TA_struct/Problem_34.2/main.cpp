#include <fstream>
#include <queue>
using namespace std;
/*
#include <chrono>
#include <iostream>
class Timer {
public:
    Timer() : beg_(clock_::now()) {}
    void reset() {
        beg_ = clock_::now();
    }
    double elapsed() const {
        return chrono::duration_cast<second_>
                (clock_::now() - beg_).count();
    }
private:
    typedef chrono::high_resolution_clock clock_;
    typedef chrono::duration<double, ratio<1> > second_;
    chrono::time_point<clock_> beg_;
};
*/
int* nums;
int* indices;
int sz = 0;
struct gr {
    bool operator()(const int& i, const int& j) const {
        int si = nums[i] + nums[indices[i]];
        int sj = nums[j] + nums[indices[j]];
        return si >= sj;
    }
};
int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");
    int n = 0, x0 = 0, x = 0, ni = 0;
    infile >> n >> x0;
    nums = new int[n];
    indices = new int[n];
    priority_queue< int, vector<int>, gr > q, w;
    x0 /= 2;
    nums[sz++] = x0;
    outfile << x0;
    infile >> x;
    indices[0] = 1;
    q.push(0);
    while (sz < n) {
        if (q.empty() || indices[q.top()] >= sz || nums[q.top()]+nums[indices[q.top()]] > x) {
            ni = x - x0;
            nums[sz++] = ni;
            indices[sz-1] = 1;
            outfile << endl << ni;
            q.push(sz-1);
            while (!w.empty()) {
                int p = w.top();
                w.pop();
                q.push(p);
            }
        } else {
            int p = q.top();
            q.pop();
            ++indices[p];
            if (indices[p] < sz)
                q.push(p);
            else
                w.push(p);
        }
        infile >> x;
    }
    outfile.close();
    return 0;
}
