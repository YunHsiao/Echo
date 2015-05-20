// stdafx.cpp : 只包括标准包含文件的源文件
// test-3.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
extern D3DXVECTOR3 g_vScreenOrigin(0.f, 0.f, 0.f);
extern D3DXVECTOR3 g_vScreenCenter(CLIENT_WIDTH/2.f, CLIENT_HEIGHT/2.f, 0.f);
extern D3DXVECTOR3 g_vDefaultDir(0.f, -1.f, 0.f);