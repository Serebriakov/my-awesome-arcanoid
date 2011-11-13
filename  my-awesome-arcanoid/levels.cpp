#include "levels.h"
#include <vector>


Levels::Levels()
{

}

void Levels::change(int num)
{


	int level1[]=	
	{
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1
	};


	int level2[]=	
	{
		0,1,0,1,0,1,0,1,0,1,0,1,
		1,0,1,0,1,0,1,0,1,0,1,0,
		0,1,0,1,0,1,0,1,0,1,0,1,
		1,0,1,0,1,0,1,0,1,0,1,0,
		0,1,0,1,0,1,0,1,0,1,0,1,
		1,0,1,0,1,0,1,0,1,0,1,0,
		0,1,0,1,0,1,0,1,0,1,0,1,
		1,0,1,0,1,0,1,0,1,0,1,0,
		0,1,0,1,0,1,0,1,0,1,0,1,
		1,0,1,0,1,0,1,0,1,0,1,0
	
	};

	int level3[]=	
	{
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,1,1,0,1,1,0,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		0,0,0,1,1,1,1,1,1,1,1,1,
		0,0,0,1,1,0,1,1,1,1,1,1,
		0,0,0,0,1,0,1,1,1,1,1,1,
		0,0,0,0,0,0,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,1,1

	};
	int level4[]=	
	{
		1,1,1,1,1,1,1,1,1,0,0,0,
		1,1,1,1,1,1,1,1,1,0,0,0,
		1,1,1,1,1,1,1,1,1,0,0,0,
		1,1,0,0,0,1,1,1,1,0,0,0,
		1,1,0,0,0,1,1,1,1,0,0,0,
		1,1,0,0,0,1,1,1,1,0,0,0,
		1,1,0,0,0,1,1,1,1,0,0,0,
		1,1,0,0,0,1,1,1,1,0,0,0,
		1,1,1,1,1,1,1,1,1,1,0,0,
		1,1,1,1,1,1,1,1,1,1,0,0
	};
	int level5[]=	
	{
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1
	};
	int level6[]=	
	{
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1
	};
	int level7[]=	
	{
		0,0,0,0,1,0,1,0,1,0,1,0,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,0,0,0,1,1,0,0,0,1,0,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,0,
		0,1,0,1,0,1,1,0,0,0,1,1,
		0,0,0,1,0,1,1,0,0,0,1,0,
		0,0,0,0,0,0,1,0,0,0,1,1,
		0,0,1,1,0,0,0,1,0,0,1,0,
		0,0,0,1,1,0,1,0,1,1,0,1
	};
	int level8[]=	
	{
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,1,0,1,0,1,0,1,0,1,
		1,1,1,0,1,0,1,0,1,0,1,1,
		1,0,0,1,0,1,0,1,0,1,1,1,
		1,1,1,0,1,0,1,0,1,0,1,1,
		1,0,0,1,0,1,0,1,0,1,1,1,
		1,1,1,0,1,0,1,0,1,0,1,1,
		1,1,0,1,0,1,0,1,0,1,1,1,
		1,1,1,0,1,0,1,0,1,0,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1
	};

	switch (num)
	{

	
case 1:
	for (int i=0;i<120;i++)
		current_level[i]=level1[i];
		lvl=1;
		break;
case 2:
	for (int i=0;i<120;i++)
		current_level[i]=level2[i];
		lvl=2;
		break;

case 3:
	for (int i=0;i<120;i++)
		current_level[i]=level6[i];
		lvl=3;
		break;
case 4:
	for (int i=0;i<120;i++)
		current_level[i]=level4[i];
		lvl=4;
		break;
case 5:
	for (int i=0;i<120;i++)
		current_level[i]=level5[i];
		lvl=5;
		break;

case 6:
	for (int i=0;i<120;i++)
		current_level[i]=level6[i];
		lvl=6;
		break;
case 7:
	for (int i=0;i<120;i++)
		current_level[i]=level7[i];
		lvl=7;
		break;
case 8:
	for (int i=0;i<120;i++)
		current_level[i]=level8[i];
		lvl=8;
		break;

case 9:
	for (int i=0;i<120;i++)
		current_level[i]=level3[i];
		lvl=9;
		break;
	}

}
