// Basic type traits, needed for algorithm implementation.

namespace type_traits {

template<typename ValueT>
struct RemoveReference {
  typedef ValueT Type;
};

template<typename ValueT>
struct RemoveReference<ValueT &> {
  typedef ValueT Type;
};

// Need remove_const/remove_reference here?
template <typename ValueT>
constexpr auto move(ValueT &&v) {
  using VValueT = typename RemoveReference<ValueT>::Type;
  return static_cast<VValueT &&>(v);
}

}  // namespace type_traits
