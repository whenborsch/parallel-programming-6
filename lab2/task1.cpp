#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

int main() {
  // Выясняем количество потоков выполнения, поддерживаемых процессором
  auto n_threads = std::thread::hardware_concurrency();

  // Создаем n потоков
  std::vector<std::thread> threads(n_threads);
  for (int i = 0; i < n_threads; i++) {
    threads[i] = std::thread([i] {
      auto start = std::chrono::high_resolution_clock::now();
      // Каждому потоку при создании присваивается номер i от 1 до n
      std::cout << "Поток №" << i + 1 << " начал работу\n";
      // Каждый поток после запуска должен ждать i секунд
      std::this_thread::sleep_for(std::chrono::seconds(i + 1));
      // Выводим в консоль время работы потока
      auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start);
      auto diff_min = std::chrono::duration_cast<std::chrono::minutes>(diff_sec);
      std::cout << "Поток №" << i + 1 << " завершил работу за: " << diff_min.count() << "m " << diff_sec.count() << "s\n";
    });
  }

  // Ждем завершения работы всех потоков
  for (auto& thread : threads) {
    thread.join();
  }

  return 0;
}
