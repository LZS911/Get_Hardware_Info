#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;

typedef struct hardware_info
{
	char model_name[31];	//存放cpu型号
	char freq[11];			//存放cpu主频
	char memory_total[21];  //存放内存总大小
}info;


int get_hardware_info(info *); 

int main()
{
	info hi;

	if(get_hardware_info(&hi))
	{
		cout<<"获取当前系统硬件配置信息失败!"<<endl;
	}

	else
	{
		cout<<hi.model_name<<endl<<hi.freq<<endl<<hi.memory_total<<endl;
	}


	return 0;

}

/*
 * 函数功能:获取当前新系统硬件配置信息(包括CPU型号和主频 物理内存大小)
 * 
 * 参数描述:
 * hi 用于接受硬件配置信息的缓存区
 *
 * 放回值:成功返回0, 失败返会非零值(1表示文件打开失败, 2表示配置文件数据格式有问题)
 *
 * 作者:gll
 * 时间:2018-8-27 10:56
*/
int get_hardware_info(info *hi)
{
	FILE *fp_cpu_info = NULL;
	FILE *fp_mem_info = NULL;
	char line[101];
	int success_cnt = 0;

	fp_cpu_info = fopen("/proc/cpuinfo", "r");
	
	if(fp_cpu_info == NULL) 
	{
		fclose(fp_cpu_info);

		return 1;
	}

	fp_mem_info = fopen("/proc/meminfo", "r");
	
	if(fp_mem_info == NULL) 
	{

		fclose(fp_mem_info);

		return 1;
	}


	while(fgets(line, sizeof(line), fp_cpu_info))
	{

		if(strstr(line, "model name"))
		{
			char *p = NULL;

			if(line[strlen(line) - 1] == '\n')
				line[strlen(line) - 1] = '\0';

			p = strchr(line, ':');

			if(p != NULL) 
			{   
				while(*++p == ' ');
				strcpy(hi -> model_name, p);
				success_cnt++;
			}
			
		}
		
		if(strstr(line, "cpu MHz"))
		{
			char *p = NULL;
			
			if(line[strlen(line) - 1] == '\n')
				line[strlen(line) - 1] = '\0';


			p = strchr(line, ':');

			if(p != NULL) 
			{   
				while(*++p == ' ');
				strcpy(hi -> freq, p);
				success_cnt++;

			}
		}
	}
	

	while(fgets(line, sizeof(line), fp_mem_info))
	{

		if(strstr(line, "MemTotal"))
		{
			char *p = NULL;

			if(line[strlen(line) - 1] == '\n')
				line[strlen(line) - 1] = '\0';


			p = strchr(line, ':');

			if(p != NULL) 
			{  
				while(*++p == ' ');
				strcpy(hi -> memory_total, p);
				success_cnt++;
			}
			
		}

 
	}

	fclose(fp_cpu_info);
	fclose(fp_mem_info);


	if(3 != success_cnt)
	{
		return 2;
	}

	return 0;
}
