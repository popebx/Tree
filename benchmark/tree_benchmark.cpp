#define _CRT_SECURE_NO_WARNINGS 1
#include <benchmark/benchmark.h>
#include <container/tree/sorted_btree.h>
#include <container/tree/tree.h>
constexpr int64_t range_start = 8;
constexpr int64_t range_end = 8 << 14;

using namespace container::tree;

#pragma region creation

static void benchmark_tree_creation(benchmark::State& state) {
  tree<int64_t> root{0};
  for (auto _ : state) {
    for (int64_t i = 1; i < state.range(0); i++) {
      root.add_child(i);
    }
  }
}
BENCHMARK(benchmark_tree_creation)->Range(range_start, range_end);

static void benchmark_std_vector_creation(benchmark::State& state) {
  std::vector<int64_t> root{0};
  root.reserve(state.range(0));
  for (auto _ : state) {
    for (int64_t i = 1; i < state.range(0); i++) {
      root.emplace_back(i);
    }
  }
}
BENCHMARK(benchmark_std_vector_creation)->Range(range_start, range_end);

static void benchmark_sorted_btree_creation(benchmark::State& state) {
  sorted_btree<int64_t> root{state.range(0) / 2};
  for (auto _ : state) {
    for (int64_t i = 1; i < state.range(0); i++) {
      root.insert(i);
    }
  }
}
BENCHMARK(benchmark_sorted_btree_creation)->Range(range_start, range_end);

#pragma endregion

#pragma region search

static void benchmark_tree_search(benchmark::State& state) {
  tree<int64_t> root{0};
  for (int64_t i = 1; i < state.range(0); i++) {
    root.add_child(i);
  }
  int64_t search_value = state.range(0) - 1;
  for (auto _ : state) {
    auto children = root.children();
    auto found = std::find_if(children.begin(), children.end(),
                 [&search_value](const tree_node_view<int64_t>& entry) -> bool { return entry.value() == search_value; });
    benchmark::DoNotOptimize(found);
  }
}
BENCHMARK(benchmark_tree_search)->Range(range_start, range_end);

static void benchmark_std_vector_search(benchmark::State& state) {
  std::vector<int64_t> root {};
  root.reserve(state.range(0));
  for (int64_t i = 1; i < state.range(0); i++) {
    root.emplace_back(i);
  }
  int64_t search_value = state.range(0) - 1;
  for (auto _ : state) {
    for (int64_t i = 1; i < state.range(0); i++) {
      auto found = std::find_if(root.begin(), root.end(), [&search_value](const int64_t& value) { return value == search_value; });
      benchmark::DoNotOptimize(found);
    }
  }
}
BENCHMARK(benchmark_std_vector_search)->Range(range_start, range_end);

static void benchmark_sorted_btree_search(benchmark::State& state) {
  sorted_btree<int64_t> root{state.range(0) / 2};
  for (int64_t i = 1; i < state.range(0); i++) {
    root.insert(i);
  }
  int64_t search_value = state.range(0) - 1;
  for (auto _ : state) {
    auto found = root.find_if(search_value);
    benchmark::DoNotOptimize(found);
  }
}
BENCHMARK(benchmark_sorted_btree_search)->Range(range_start, range_end);

#pragma endregion