#pragma once
// <Precompilation Header>

// ******* //
// <Basic> //
// ******* //
#pragma comment(lib,"winmm.lib")

#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#include <cassert>

#include <wrl/client.h>

// ***** //
// <STL> //
// ***** //
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <fileSystem>
#include <chrono>
#include <type_traits>

#define _USE_MATH_DEFINES
#include <math.h>

// *********** //
// <DirectX12> //
// *********** //
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

// DirectX Tool Kit

// DirectX Tex

// ********** //
// <Original> //
// ********** //
#include "Framework/FlFramework.hxx"