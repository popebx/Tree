#pragma warning(push)
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#pragma warning(disable : 5275)
#include <gtest/gtest.h>
#pragma warning(pop)
#include <container/tree/sorted_btree.h>
#include <string>

using namespace container::tree;

template <class data_type>
void test_ordering(const sorted_btree_view<data_type>& view) {
  if (view.has_left()) {
    ASSERT_LT(view.left().value(), view.value());
    test_ordering(view.left());
  }
  if (view.has_right()) {
    ASSERT_LT(view.value(), view.right().value());
    test_ordering(view.right());
  }
}

TEST(sorted_btree, deconstructor) {
  auto root = std::make_unique<sorted_btree<int>>(5000);
  for (int i = 0; i < 10000; i++) {
    root->insert(i);
  }
  root.reset();
  ASSERT_TRUE(true);
}

TEST(sorted_btree, creation) {
  sorted_btree<int> root{50};
  for (int i = 0; i < 101; i++) {
    root.insert(i);
  }
  test_ordering<int>(root);
}

TEST(sorted_btree, print_in_order) {
  sorted_btree<int> root{0};
  for (int i = -5; i < 6; i++) {
    root.insert(i);
  }
  sorted_btree_util::print<int, sorted_btree_util::order::IN_ORDER>(root,
                                                                    [](const int& value) -> std::string { return std::to_string(value) + " "; });
}

TEST(sorted_btree, find_value) {
  sorted_btree<int> root{50};
  for (int i = 0; i < 101; i++) {
    root.insert(i);
  }

  ASSERT_NE(root.find_if(75), std::nullopt);
  ASSERT_NE(root.find_if(0), std::nullopt);
  ASSERT_EQ(root.find_if(101), std::nullopt);
}

TEST(sorted_btree, size) {
  sorted_btree<int> root{50};
  for (int i = 1; i <= 100; i++) {
    root.insert(i);
  }
  ASSERT_EQ(sorted_btree_util::size(root), 100);
}