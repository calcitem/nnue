// Copyright 2020, 2021 by Jon Dart. All Rights Reserved.
#ifndef _NNUE_HALF_KP_H
#define _NNUE_HALF_KP_H

#include "accum.h"
#include "nndefs.h"
#include "typed.h"

// Implements first layer of the neural network

template <typename InputType, typename WeightType, typename BiasType,
    typename OutputType, size_t inputSize, size_t outputSize,
    size_t alignment = DEFAULT_ALIGN>
class HalfKp : public TypedLayer<InputType, OutputType, inputSize, outputSize,
                   alignment> {
public:
    HalfKp() = default;

    virtual ~HalfKp() = default;

    using AccumulatorType
        = Accumulator<OutputType, WeightType, BiasType, outputSize * 2>;

    // Propagate data through the layer, updating the specified half of the
    // accumulator (side to move goes in lower half).
    inline void updateAccum(const IndexArray& indices, AccumulatorHalf half,
        AccumulatorType& output)
    {
        output.init_half(half, this->_biases);
        for (auto it = indices.begin();
             it != indices.end() && *it != LAST_INDEX; ++it) {
            output.add_half(half, this->_weights[*it]);
        }
    }

    // Perform an incremental update
    void updateAccum(const IndexArray& added, const IndexArray& removed,
        size_t added_count, size_t removed_count, AccumulatorHalf half,
        AccumulatorType& output)
    {
        for (size_t i = 0; i < added_count; i++) {
            output.add_half(half, this->_weights[added[i]]);
        }
        for (size_t i = 0; i < removed_count; i++) {
            output.sub_half(half, this->_weights[removed[i]]);
        }
    }

    virtual inline void doForward(const InputType*, OutputType*) const noexcept
    {
        // no-op for this layer: use updateAccum
    }

    virtual std::istream& read(std::istream& s)
    {
        for (size_t i = 0; i < outputSize && s.good(); ++i) {
            _biases[i] = read_little_endian<BiasType>(s);
        }
        for (size_t i = 0; i < inputSize && s.good(); ++i) {
            for (size_t j = 0; j < outputSize && s.good(); ++j) {
                _weights[i][j] = read_little_endian<WeightType>(s);
            }
        }
        return s;
    }

    virtual const WeightType* getCol(size_t row) const noexcept
    {
        return _weights[row];
    }

    virtual void setCol(size_t row, const WeightType* col)
    {
        for (size_t i = 0; i < outputSize; ++i)
            _weights[row][i] = col[i];
    }

private:
    alignas(alignment) BiasType _biases[outputSize];
    alignas(alignment) WeightType _weights[inputSize][outputSize];
};

#endif
