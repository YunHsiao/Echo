// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#include "resource.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#define _CRT_RAND_S
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <fstream>

// TODO: 在此处引用程序需要的其他头文件
#include <map>
#include <list>
#include <deque>
#include <vector>
#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include <MMSystem.h>
#include "AudioList.h"
#include "ResourceList.h"

#define CLIENT_WIDTH 800
#define CLIENT_HEIGHT 600

typedef std::vector<D3DXVECTOR3> Points;
typedef std::vector<D3DXVECTOR3>::iterator PointsIt;
typedef std::vector<D3DXVECTOR3>::const_iterator PointsCIt;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(s) { if(NULL!=s){(s)->Release();(s)=NULL;} }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(s) { if(NULL!=s){delete (s);(s)=NULL;} }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(s) { if(NULL!=s){delete [](s);(s)=NULL;} }
#endif