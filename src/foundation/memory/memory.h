
#pragma once

#include "core/config.h"

namespace Memory
{

	//С��λ�Ŀ����У��ȺŸ�ֵ�ȵ����ڴ濽������Ҫ��ܶ�ܶࡣ
	//��xmemcpy��һ�����߾�����ô�ɵģ��ų�С��120�ֽ����ڵĿ�����gcc���ڴ濽����10����
	//����ò�ƣ��ȺŸ�ֵ���ȱ���<=128�ֽڣ������ǻ����ֳ��ı���
#ifdef __WIN64__
	typedef unsigned int smallest;
#elif defined __OSX__
	typedef unsigned int smallest;
#else
	typedef size_t smallest;
#endif

__forceinline
void CopySmallOne(const smallest* from, smallest* to)
{
	to[0] = from[0];
}

__forceinline
	void CopySmallTwo(const smallest* from, smallest* to)
{
	to[0] = from[0];
	to[1] = from[1];
}

__forceinline
	void CopySmallThree(const smallest* from, smallest* to)
{
	to[0] = from[0];
	to[1] = from[1];
	to[2] = from[2];
}

__forceinline
	void CopySmallFour(const smallest* from, smallest* to)
{
	to[0] = from[0];
	to[1] = from[1];
	to[2] = from[2];
	to[3] = from[3];
}

__forceinline
	void CopySmallFive(const smallest* from, smallest* to)
{
	to[0] = from[0];
	to[1] = from[1];
	to[2] = from[2];
	to[3] = from[3];
	to[4] = from[4];
}

__forceinline
	void CopySmallSix(const smallest* from, smallest* to)
{
	to[0] = from[0];
	to[1] = from[1];
	to[2] = from[2];
	to[3] = from[3];
	to[4] = from[4];
	to[5] = from[5];
}
}
