// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"
#include "resource.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#define _CRT_RAND_S
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <fstream>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
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