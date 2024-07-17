#pragma once

#include <concepts>

namespace BSerializer {
    template <typename _T>
    concept Serializable = requires(const _T Obj_C, _T Obj_V, const void* Data_C, void* Data_V) {
        { Obj_C.SerializedSize() } -> std::same_as<size_t>;
        { Obj_C.Serialize(Data_V) } -> std::same_as<void>;
        { _T::Deserialize(Data_C) } -> std::same_as<_T>;
        { _T::Deserialize(Data_C, Obj_V) } -> std::same_as<void>;
    };
}