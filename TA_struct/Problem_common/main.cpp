#include <iostream>
#include <fstream>
using namespace std;

bool is_bin_heap(int* arr, int size, int i) {
    return (2*i + 2 > size) ||
            ((2*i + 1 == size || arr[i] <= arr[2*i + 1]) &&
             (2*i + 2 == size || arr[i] <= arr[2*i + 2]) &&
             is_bin_heap(arr, size, 2*i + 1) &&
             is_bin_heap(arr, size, 2*i + 2));
}

int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");
    int n = 0;
    infile >> n;
    int* arr = new int[n];
    for (auto i = 0; i < n; i++) {
        infile >> arr[i];
    }
    outfile << (is_bin_heap(arr, n, 0)? "Yes" : "No");
    outfile.close();
    return 0;
}