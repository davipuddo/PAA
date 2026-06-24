#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>

#define LIMIT 1000;

struct Point {
    
    int x;
    int y;

    Point(bool r = false) {
        this->x = -1;   
        this->y = -1;
        if (r == true) {
            this->x = (int) rand() % LIMIT;
            this->y = (int) rand() % LIMIT;
        }
    }

    Point(int x, int y) {
        this->x = x;   
        this->y = y;
    }

    ~Point() = default;

    void print() {
        std::cout << "(" << x << ", " << y << ")";
        std::fflush(stdout);
    }

    double dist(Point& other) {
        using namespace std;
        return sqrt( pow(this->x - other.x, 2) + pow(this->y - other.y, 2) );
    }
};

struct Points {
    std::vector<Point> list;

    Points() {
        this->list = std::vector<Point>();
    }

    Points(int n) {
        this->list = std::vector<Point>(n);
    }

    ~Points() = default;

    Point &operator[] (int pos) {
        return list[pos];
    }

    void rand(int n) {
        for (int i = 0; i < n; i++) {

            Point p = Point(true);

            bool repeat = false;

            int j = 0;
            while (j < (int)list.size() && !repeat) {
                if (p.x == list[j].x || p.y == list[j].y) {
                    repeat = true;
                }
                j++;
            }

            if (!repeat) {
                push(p);           
            } else {
                i--;
            }
        }
    }

    void push(Point x) {
        list.push_back(x);
    }

    Points clone() {
        int n = (int)list.size();
        Points other(n);
        for (int i = 0; i < n; i++) {
            other.list[i] = this->list[i];
        }
        return other;
    }

    void sortX() {
        sort(list.begin(), list.end(), 
                [](const Point& a, const Point& b) {
                    return a.x < b.x;
                }
            );
    }

    void sortY() {
        sort(list.begin(), list.end(), 
                [](const Point& a, const Point& b) {
                    return a.y < b.y;
                }
            );
    }

    void print() {
        int n = (int)list.size();
        if (n > 0) {
            std::cout << "[";
            for (int i = 0; i < n-1; i++) {
                list[i].print();
                std::cout << "; ";
            }
            list[n-1].print();
            std::cout << "]" << std::endl;
        }
    }
};

Points inter(Points Px, Points Py, int L, int M, int R, double delta) {
    Points Sy = Points();
    Sy.print();

    int n = Py.list.size();
    for (int i = 0; i < n; i++) {
        Point p = Py[i];
        if (std::abs(p.x - Px[M].x) < delta) { // Point is within a distance delta from the line
            Sy.push(p);
        }
    }

    n = Sy.list.size();

    Points buf;
    for (int i = 0; i < n; i++) {

        Point s1 = Sy[i];

        int j = i+1;
        while (j < n && (j-i) < 8) {

            Point s2 = Sy[j];
            double d = s1.dist(s2);

            if (d < delta) {
                buf.list.clear();
                buf.push(s1);
                buf.push(Sy[j]);
                delta = d;
            }
            j++;
        }
    }
    return buf;
}

Points closest(Points Px, Points Py, int L, int R) {

    Points res = Points();

    if ((R - L - 1) <= 3) {

        int n = R - L;

        double d = MAXFLOAT;

        for (int i = 0; i < n; i++) {
            Point p = Px[i + L];
            for (int j = i+1; j < n; j++) {
                double buf = p.dist(Px[j + L]);
                if (buf < d) {
                    res.list.clear();
                    res.push(p);
                    res.push(Px[j + L]);
                }
            }
        }

    } else {

        int M = (int)((double)(R+L)/2.0);

        Points p1 = closest(Px, Py, L, M);
        Points p2 = closest(Px, Py, M, R);
        
        double delta1 = p1[0].dist(p1[1]);
        double delta2 = p2[0].dist(p2[1]);

        double delta = std::min(delta1, delta2);

        Points pk = inter(Px, Py, L, M, R, delta);

        double deltak = MAXFLOAT;

        if ((int)pk.list.size() > 0) { 
            deltak = pk[0].dist(pk[1]);
        }

        double sm = std::min(delta, deltak);
        if (sm == deltak) {
            res = pk;
        } else if (sm == delta1) {
            res = p1;
        } else {
            res = p2;
        }
    }
    return res;
}

Points closest(Points P) {

    Points Px = P.clone();
    Px.sortX();

    Points Py = P.clone();
    Py.sortY();

    return closest(Px, Py, 0, (int)Px.list.size()-1);
}

Points optimal(Points P) {

    int n = (int)P.list.size();

    Points res;
    
    double d = MAXFLOAT;
    for (int i = 0; i < n; i++) {
        Point p = P[i];
        for (int j = i+1; j < n; j++) {
            double D = p.dist(P[j]);
            if (D < d) {
                res.list.clear();
                res.push(p);
                res.push(P[j]);
                d = D;
            }
        }
    }
    return res;
}

int main (void) {
    
    srand(time(NULL));

    Points P = Points();
    P.rand(500);

    std::cout << "Points: ";
    P.print();

    std::cout << "\nOptimal: ";

    clock_t start = clock();
    Points op = optimal(P);
    clock_t end = clock();

    std::cout << op[0].dist(op[1]) << std::endl;
    op.print();

    double time = (double)(end-start)/CLOCKS_PER_SEC;
    std::cout << "\nruntime: " << time << std::endl;

    std::cout << "\nDC: ";

    start = clock();
    Points dc = closest(P);
    end = clock();

    std::cout << dc[0].dist(dc[1]) << std::endl;
    dc.print();

    time = (double)(end-start)/CLOCKS_PER_SEC;
    std::cout << "\nruntime: " << time << std::endl;


    return(0);
}
