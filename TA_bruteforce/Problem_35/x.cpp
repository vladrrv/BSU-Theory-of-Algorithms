#include <fstream>
#include <cmath>
using namespace std;

double **arr;
bool *rem_r, *rem_c;
int **rem;
int n;
double cost;

bool check(int i, int j) {
    return rem[i][j] == 0 && !rem_r[i] && !rem_c[j];
}
void remove_row_col(int row, int col, int m, int lower_bound) {
    rem_r[row] = true;
    rem_c[col] = true;
	for (int i = 0; i < n; ++i) {
		bool fr = true, fc = true, f = false;
		for (int j = 0; j < n; ++j) {
			if (rem_r[i] || rem_c[j]) continue;
			else f = true;
			if (fr && rem[i][j] == -1) {
				fr = false;
			}
		}
		if (f && fr) {
			row = i;
		}
		f = false;
		for (int j = 0; j < n; ++j) {
			if (rem_r[j] || rem_c[i]) continue;
			else f = true;
			if (fc && rem[j][i] == -1) {
				fc = false;
			}
		}
		if (f && fc) {
			col = i;
		}
	}
	rem[row][col] = -1;
    tsp(m-1, lower_bound);
    rem_r[row] = false;
    rem_c[col] = false;
    rem[row][col] = 0;
}

double min_row_col(int row, int col) {
	double r_min = -1, c_min = -1;
	for (int i = 0; i < n; ++i) {
		if ((col != i) && check(row,i) && (arr[row][i] < r_min || r_min < 0)) {
			r_min = arr[row][i];
		}
		if ((row != i) && check(i,col) && (arr[i][col] < c_min || c_min < 0)) {
			c_min = arr[i][col];
		}
	}
	return r_min + c_min;
}

void tsp(int m, double lower_bound) {
    double *row_mins = new double[n];
    double *col_mins = new double[n];
    fill_n(row_mins, n, 0);
    fill_n(col_mins, n, 0);
	for (int i = 0; i < n; ++i) {
		double min_row = -1;
		for (int j = 0; j < n; ++j) {
			if (check(i, j) && (arr[i][j] < min_row || min_row < 0)) {
				min_row = arr[i][j];
			}
		}
		if (min_row < 0) continue;
        row_mins[i] = min_row;
		lower_bound += min_row;
		for (int j = 0; j < n; ++j) {
			if (check(i, j)) {
				arr[i][j] -= min_row;
			}
		}
	}
	for (int j = 0; j < n; ++j) {
		double min_col = -1;
		for (int i = 0; i < n; ++i) {
			if (check(i, j) && (arr[i][j] < min_col || min_col < 0)) {
				min_col = arr[i][j];
			}
		}
		if (min_col < 0) continue;
        col_mins[j] = min_col;
		lower_bound += min_col;
		for (int i = 0; i < n; ++i) {
			if (check(i, j)) {
				arr[i][j] -= min_col;
			}
		}
	}
	if (cost > 0 && lower_bound >= cost) {
        for (int j = 0; j < n; ++j) {
            double cmin = col_mins[j];
            for (int i = 0; i < n; ++i) {
                if (check(i, j)) arr[i][j] += cmin;
            }
        }
        for (int i = 0; i < n; ++i) {
            double rmin = row_mins[i];
            for (int j = 0; j < n; ++j) {
                if (check(i, j)) arr[i][j] += rmin;
            }
        }
	    return;
	}
	if (m == 2) {
        for (int j = 0; j < n; ++j) {
            double cmin = col_mins[j];
            for (int i = 0; i < n; ++i) {
                if (check(i, j)) arr[i][j] += cmin;
            }
        }
        for (int i = 0; i < n; ++i) {
            double rmin = row_mins[i];
            for (int j = 0; j < n; ++j) {
                if (check(i, j)) arr[i][j] += rmin;
            }
        }
		cost = lower_bound;
		return;
	}
	double max_coef = -1; int c = -1, r = -1;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (check(i, j) && arr[i][j] == 0) {
				double cur = min_row_col(i, j);
				if (cur > max_coef) {
					max_coef = cur;
					r = i; c = j;
				}
			}
		}
	}
	remove_row_col(r, c, m, lower_bound);
	rem[r][c] = -2;
	tsp(m, lower_bound);
    rem[r][c] = 0;
    for (int j = 0; j < n; ++j) {
        double cmin = col_mins[j];
        for (int i = 0; i < n; ++i) {
            if (check(i, j)) arr[i][j] += cmin;
        }
    }
    for (int i = 0; i < n; ++i) {
        double rmin = row_mins[i];
        for (int j = 0; j < n; ++j) {
            if (check(i, j)) arr[i][j] += rmin;
        }
    }
}

struct point {
	double x, y;
};

int main() {
	ifstream infile("input.txt");
	ofstream outfile("output.txt");
	infile >> n;
	point*pts = new point[n];
	cost = 0;
	arr = new double*[n];
    rem = new int*[n];
	for (int i = 0; i < n; ++i) {
		arr[i] = new double[n];
		rem[i] = new int[n];
		fill_n(arr[i], n, 0.0);
        fill_n(rem[i], n, 0);
		double x, y;
		infile >> x >> y;
		pts[i] = {x, y};
	}
	for (int i = 0; i < n; ++i) {
		rem[i][i] = -1;
		for (int j = i+1; j < n; ++j) {
			double
					x = pts[i].x-pts[j].x,
					y = pts[i].y-pts[j].y,
					d = sqrt(x*x+y*y);
			arr[i][j] = d;
			arr[j][i] = d;
		}
	}
	tsp(n, 0);
	outfile << fixed << cost;
	return 0;
}
