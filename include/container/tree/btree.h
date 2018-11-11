#pragma once
#include <array>
#include <memory>
namespace container::tree {
template <class data_type>
class btree {
 public:
  explicit btree(const data_type& value) : m_value{value} {}
  explicit btree(data_type&& value) : m_value{std::move(value)} {}
  btree(const btree&) = delete;
  btree(btree&& rhs) {}
  btree& operator=(const btree&) = delete;
  btree& operator=(btree&&) {}

 private:
  data_type m_value;
  std::unique_ptr<btree> lnode = {nullptr};
  std::unique_ptr<btree> rnode = {nullptr};
  btree* parent;
};

template <class data_type>
class btree_view {};

}  // namespace container::tree