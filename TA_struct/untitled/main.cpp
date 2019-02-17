#include <fstream>
#include <vector>
using namespace std;

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

int main() {
    ofstream output("in.txt");
    output << "1000 1000\n1 5";
    for (int i = 0; i < 1000; ++i) {
        output << endl << "1000";
        for (int j = 0; j < 1000; ++j) {
            output << " " << (rand()%1000+1);
        }
    }
    Timer tmr;

    cout << tmr.elapsed() << endl;
    output.close();
    return 0;
}
