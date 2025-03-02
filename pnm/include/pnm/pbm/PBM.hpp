#pragma once
#include <type_traits>

#include <pnm/pnm.hpp>
#include <pnm/common.hpp>
#include <pnm/matrix/BitMatrix1D.hpp>

struct PBM: BitMatrix1D<> {

    using BitMatrix1D<>::BitMatrix1D;

};