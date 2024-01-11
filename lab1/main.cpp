#include <iostream>
#include <functional>
#include <utility>
#include <cmath>
#include <vector>
#include <random>
#include <map>
#include <fstream>
#include <iterator>

typedef std::vector<std::pair<double, double>> pvector;

// TASK 1

std::pair<pvector, pvector> check_area(pvector points, std::function<bool(std::pair<double, double>)> area) {
  pvector in_area = {};
  pvector not_in_area = {};

  for (auto p : points) {
    if (area(p)) {
      in_area.push_back(p);
    } else {
      not_in_area.push_back(p);
    }
  }

  std::pair<pvector, pvector> result = {in_area, not_in_area};
  return result;
}

const bool area_a(std::pair<double, double> P) {
  double a = 1.0;
  return (P.first + P.second <= 5 * a / 2) && (P.first * P.second >= a*a);
}

const bool area_pq(std::pair<double, double> P) {
  double p = 1.0;
  double q = 1.0;
  return (P.second *P.second >= 2*p*P.first + p*p) && (P.second <= -2*q*P.first + q*q);
}

// TASK 2
void number_conversion(int n) {
  // допустимые границы определены в stdint.h / cstdint

  std::cout << "UINT64_T MAX = " << UINT64_MAX << std::endl; // 2^64 - 1, потолок
  // передать можно сколь угодно большое, но оно будет закодировано как максимально возможное для UINT64
  std::cout << "UINT32_T MAX = " << UINT32_MAX << std::endl;
  std::cout << "INT64_T MAX = " << INT64_MAX << std::endl;
  std::cout << "INT32_T MAX = " << INT32_MAX << std::endl << std::endl;

  std::cout << "given number (int) = " << n << std::endl;
  std::cout << "UINT64_T number = " << (uint64_t) n << std::endl;
  std::cout << "UINT32_T number = " << (uint32_t) n << std::endl;
  std::cout << "INT64_T number = " << (int64_t) n << std::endl;
  std::cout << "INT32_T number = " << (int32_t) n << std::endl;
}

// TASK 3
void generate_binomial() {
  std::default_random_engine rnd;

  std::binomial_distribution<int> brand(INT32_MAX, 0.5);
  std::vector<int> v;

  std::cout << "Generating numbers...";  
  
  for (uint64_t n = 0; n < INT16_MAX; n++) {
    v.push_back(brand(rnd));
  }
  
  std::cout << " Finished." << std::endl;
  
  std::ofstream of("binomial.txt");
  std::ostream_iterator<int> out_it(of, "\n");
  std::copy(v.begin(), v.end(), out_it);

  std::system("python3 graph.py");
}

// TASK 4
void generate_poisson() {
  std::default_random_engine rnd;

  std::poisson_distribution<int> prand(4);
  std::map<int, int> hist;
  
  for (int n = 0; n != 10000; ++n)
    ++hist[prand(rnd)];
 
  for (auto const& [x, y] : hist)
    std::cout << x << ' ' << std::string(y / 100, '*') << std::endl;
}

// TASK 5
void manipulate_vector() {
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 0, 0, -1, -2, -3, -4, -5, -6, -7};
  std::cout << "Initial vector: ";
  for (auto i : v) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  std::cout << "Removing zeros: ";
  v.erase(std::remove(v.begin(), v.end(), 0), v.end());
  for (auto i : v) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  std::cout << "Removing negative values: ";
  v.erase(std::remove_if(v.begin(), v.end(), [](int i){return i < 0;}), v.end());
  for (auto i : v) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  std::cout << "Removing positive odd values: ";
  v.erase(std::remove_if(v.begin(), v.end(), [](int i){return i > 0 && i % 2 == 1;}), v.end());
  for (auto i : v) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
}


int main(int argc, char* argv[]) {
  // task 1
  std::cout << "------" << std::endl << "TASK 1" << std::endl << "------" << std::endl;

  // generate vector of 10 points for area_a()
  std::default_random_engine rnd;
  std::uniform_real_distribution<double> urand(-1.0, 2.0);
  pvector points = {};
  for (int i = 0; i < 10; i++) {
    points.push_back({urand(rnd), urand(rnd)});
  }

  std::pair<pvector, pvector> v1 = check_area(points, area_a);
  std::pair<pvector, pvector> v2 = check_area(points, area_pq);

  std::cout << "IN area_a: ";
  for (auto p : v1.first) {
  	std::cout << "(" << p.first << ", " << p.second << "); ";
  }
  std::cout << std::endl << "NOT IN area_a: ";
  for (auto p : v1.second) {
        std::cout << "(" << p.first << ", " << p.second << "); ";
  }
  std::cout << std::endl << std::endl;

  std::cout << "IN area_pq: ";
  for (auto p : v2.first) {
  	std::cout << "(" << p.first << ", " << p.second << "); ";
  }
  std::cout << std::endl << "NOT IN area_pq: ";
  for (auto p : v2.second) {
        std::cout << "(" << p.first << ", " << p.second << "); ";
  }
  std::cout << std::endl;

  // task 2
  if (argc > 1) {
    std::cout << "------" << std::endl << "TASK 2" << std::endl << "------" << std::endl;
    number_conversion(std::atoi(argv[1]));
  }

  // task 3
  std::cout << "------" << std::endl << "TASK 3" << std::endl << "------" << std::endl;
  generate_binomial();

  // task 4
  std::cout << "------" << std::endl << "TASK 4" << std::endl << "------" << std::endl;
  generate_poisson();

  // task 5
  std::cout << "------" << std::endl << "TASK 5" << std::endl << "------" << std::endl;
  manipulate_vector();

  return 0;
}
