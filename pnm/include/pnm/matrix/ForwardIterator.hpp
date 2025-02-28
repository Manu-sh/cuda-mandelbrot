#pragma once
#include <iterator>

template <typename Matrix1D>
struct ForwardIterator:
        public std::iterator<
            std::forward_iterator_tag,                      // iterator_category
            typename Matrix1D::value_type,                  // value_type
            typename Matrix1D::difference_type,             // difference_type
            typename Matrix1D::pointer,                     // pointer
            typename Matrix1D::reference                    // reference
> {

};