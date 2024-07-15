#pragma once

#include <concepts>

namespace BSerializer {
    template <typename _T>
    concept Serializable = requires(const _T Obj_C, const void* const Data_CC, void* const Data_CV) {
        { Obj_C.SerializedSize() } -> std::same_as<size_t>;
        { Obj_C.Serialize(Data_CV) } -> std::same_as<void>;
        { _T::Deserialize(Data_CC) } -> std::convertible_to<_T>;
    };
}