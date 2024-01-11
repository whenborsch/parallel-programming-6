#include <vector>
#include <thread>
#include <random>
#include <iostream>
#include <fstream>
#include <functional>
#include <mutex>

using namespace std;

const auto N_PROC = 8;
const int Total_points = 100000;
const int a = 5;
struct point_t{
    double x, y;
};
vector<point_t> in;
vector<point_t> out;

mutex vectorMutex;

void gen_test_Point(int num_points, const function<bool(double, double)>& area) {
    int local_in = 0;
    int local_out = 0;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distribX((sqrt(3)/2)*a, a*sqrt(2));
    uniform_real_distribution<> distribY(-static_cast<double>(a)/2, static_cast<double>(a)/2);

    for (int i = 0; i < num_points; ++i) {
        point_t p;

        p.x = distribX(gen) * pow(-1, i);
        p.y = distribY(gen);
        std::lock_guard<std::mutex> guard(vectorMutex);

        if (area(p.x, p.y)) {
            in.push_back(p);
        } else {
            out.push_back(p);
        }
    }

}

bool func(double i, double j) {
    if (i < a) {
        return (i*i+j*j >= a*a) && (pow(i*i+j*j, 2) <= 2*a*a*(i*i-j*j));
    } else {
        return (pow(i * i + j * j, 2) <= 2 * a * a * (i * i - j * j));
    }
}

int main() {
    int num_points = Total_points / N_PROC;
    vector<thread> threads;
    threads.reserve(N_PROC);

    const auto t1 = chrono::steady_clock::now();

    // Создание и запуск потоков
    for (auto n_th = 0; n_th < N_PROC; n_th++) {
        threads.emplace_back(gen_test_Point, num_points, func);
    }

    // Ожидание завершения всех потоков
    for (auto& th : threads) {
        th.join();
    }


    const auto t2 = chrono::steady_clock::now();

    ofstream of;
    of.open("data.csv");
    of << "x,y,mask" << endl;

    for (const auto &cur: in) {
        of << cur.x << ',' << cur.y << ',' << true << endl;
    }

    for (const auto &cur: out) {
        of << cur.x << ',' << cur.y << ',' << false << endl;
    }

    size_t cnt1 = in.size();
    size_t cnt2 = out.size();
    double square = ((a*sqrt(2)) - ((sqrt(3)/2)*a)) * (static_cast<double>(a)/2);

    cout << "Total area of data generation   " << square << endl;
    cout << "Square counted   " << 4*square*(static_cast<double>(cnt1)/(cnt1+cnt2)) << endl;
    cout << "Theoretical value   " << a*a*((3*sqrt(3) - 3.14159)/3) << endl;
    cout << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;

    std::system("python3 graph.py");

    return 0;
}