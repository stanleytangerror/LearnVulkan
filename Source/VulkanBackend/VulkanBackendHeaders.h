#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <windows.h>

//#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <array>
#include <optional>
#include <set>
#include <mutex>
#include <ranges>
#include <map>

using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

#if __cpp_lib_byte
using b8 = std::byte;
#else
using b8 = unsigned char;
#endif

template <typename T, int Dim>
struct Vec
{
	std::array<T, Dim> value;

	T x() const { return value[0]; }
	T y() const { return value[1]; }
	T z() const { return value[2]; }
	T w() const { return value[3]; }
};

using Vec2i = Vec<i32, 2>;
using Vec2u = Vec<u32, 2>;
using Vec2f = Vec<f32, 2>;
using Vec3f = Vec<f32, 3>;
using Vec3i = Vec<i32, 3>;
using Vec3u = Vec<u32, 3>;
using Vec4f = Vec<f32, 4>;
