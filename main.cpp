#include <cstddef>
#include <future>
#include <iostream>
#include <vector>

#include "clicker.hpp"

using data_t = std::vector<unsigned long long>;
using value_t = data_t::value_type;

value_t sumBlock(const data_t & value, size_t begin, size_t end)
{
  value_t sum = 0;
  for (size_t i = begin; i < end; ++i)
  {
    sum+= value[i];
  }
  return sum;
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " <number_of_threads>\n";
    return 1;
  }

  size_t threads = 0;

  try
  {
    threads = std::stoull(argv[1]);
  }
  catch(...)
  {
    std::cout << "Error: Threads count must be a positive number.\n";
    return 1;
  }

  if (threads == 0)
  {
    std::cout << "Invalid threads\n";
    return 1;
  }

  constexpr size_t size{1'000'000'000};
  double init{0}, total{0};
  value_t sum{0};
  {
    Clicker cl;
    data_t values(size, 1);
    init = cl.millisec();

    std::vector<std::future<value_t>> futures;

    for (size_t i = 0; i < threads; ++i)
    {
      size_t begin = i * size / threads;
      size_t end = (i + 1) * size /threads;

      futures.push_back(std::async(std::launch::async, sumBlock, std::cref(values), begin, end));
    }

    for (size_t i = 0; i < threads; ++i)
    {
      sum += futures[i].get();
    }

    total = cl.millisec();
  }

  std::cout << "Init: " << init << "\n";
  std::cout << "Total: " << total << "\n";
  std::cout << "Total - Init: " << total - init << "\n";
}