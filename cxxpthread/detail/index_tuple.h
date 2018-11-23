// index_tuple.h
#ifndef CXXPTHREAD_DETAIL_INDEX_TUPLE_H
#define CXXPTHREAD_DETAIL_INDEX_TUPLE_H

#include <type_traits>

namespace cxxpthread {

namespace detail {

template <std::size_t... indexes>
struct IndexTuple {};

template <std::size_t ib, typename Tuple, std::size_t ie>
struct MakeIndexTupleImp;

template <std::size_t ib, std::size_t... indexes, std::size_t ie>
struct MakeIndexTupleImp<ib, IndexTuple<indexes...>, ie> {
  using type =
      typename MakeIndexTupleImp<ib + 1, IndexTuple<indexes..., ib>, ie>::type;
};

template <std::size_t ie, std::size_t... indexes>
struct MakeIndexTupleImp<ie, IndexTuple<indexes...>, ie> {
  using type = IndexTuple<indexes..., ie>;
};

// MakeIndexTuple<1, 4>::type
// = MakeIndexTupleImp<1, IndexTuple<>, 3>::type
// = MakeIndexTupleImp<2, IndexTuple<1>, 3>::type
// = MakeIndexTupleImp<3, IndexTuple<1,2>, 3>::type
// = IndexTuple<1, 2, 3>
template <std::size_t ib, std::size_t ie>
struct MakeIndexTuple {
  static_assert(ib < ie, "MakeIndexTuple<ib, ie>: ib must < ie");
  using type = typename MakeIndexTupleImp<ib, IndexTuple<>, ie - 1>::type;
};

// MakeIndexTuple<1, 1>::type
// = IndexTuple<>
template <std::size_t i>
struct MakeIndexTuple<i, i> {
  using type = IndexTuple<>;
};

}  // namespace cxxpthread::detail

}  // namespace cxxpthread

#endif
