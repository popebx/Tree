#pragma warning(push)
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#pragma warning(disable : 5275)
#include <gtest/gtest.h>
#pragma warning(pop)
#include <container/tree/tree.h>
#include <iostream>

using namespace container::tree;

template <class data_type>
void print_rec(const tree_node_view<data_type>& view) {
  for (const auto& child : view.children()) {
    print_rec(child);
  }
    std::cout << view.value() << "\n";
}

TEST(tree, creation) {
  tree<int> root{5};
  root.add_child(1);
  root.add_child(5);
  root.add_child(9);	
  for (auto& child : root.children()) {
    auto another_child = child.add_child(9);
    another_child.add_child(2);
  }
  print_rec(static_cast<tree_node_view<int>>(root));
  ASSERT_EQ(root.value(), 5);
}
