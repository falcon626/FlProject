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

// ***************** //
// <CharCodeConvert> //
// ***************** //
#include <strconv.h>

// ******** //
// <Assimp> //
// ******** //
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// ****** //
// <FMOD> //
// ****** //
#include "fmod_studio.hpp"

// ********* //
// <DirectX> //
// ********* //
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#include <d3dx12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>

// DirectX Tool Kit
#pragma comment(lib, "DirectXTK12.lib")
#include <SimpleMath.h>

// DirectX Tex
#pragma comment(lib, "DirectXTex.lib")
#include <DirectXTex.h>

#include <d2d1_1.h>
#include <dwrite.h>

// ********** //
// <Original> //
// ********** //
#include "Framework/FlFramework.hxx"