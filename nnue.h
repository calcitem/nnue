// Copyright 2021 by Jon Dart. All Rights Reserved.
#ifndef _NNUE_H
#define _NNUE_H

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>
#ifdef __cpp_lib_bitops
#include <bit>
#endif

#ifdef SIMD
#include "simd.h"
#endif

namespace nnue {
#include "network.h"
#include "evaluate.h"
}

#endif
