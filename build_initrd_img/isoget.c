// isoget.c -- 从光盘获取可执行程式

#include "common.h"
#include "syscall.h"
#include "task.h"

#define PROGRAM_COPY_TOTAL_NUM 2

char * program_copy_cmd[PROGRAM_COPY_TOTAL_NUM] = {
	"file iso/EXTRA/EE.;1 hd/bin/ee",
	"file iso/EXTRA/LIBC.SO;1 hd/lib/libc.so",
};

int main(VOID * task, int argc, char * argv[])
{
	char input[100]={0};
	int i;
	if((argc == 2) && strcmp(argv[1],"-all") == 0)
	{
		int ret;
		TASK_MSG msg;
		for(i=0;i < PROGRAM_COPY_TOTAL_NUM;i++)
		{
			strcpy(input, program_copy_cmd[i]);
			if(syscall_execve(input) == 0)
				syscall_wait(task);
			while(TRUE)
			{
				ret = syscall_get_tskmsg(task,&msg,TRUE);
				if(ret != 1)
				{
					syscall_idle_cpu();
					continue;
				}
				if(msg.type == MT_TASK_FINISH)
				{
					syscall_finish(msg.finish_task.exit_task);
					if(msg.finish_task.exit_code == -1)
					{
						return -1;
					}
					break;
				}
			}
			syscall_monitor_write("\n");
		}
	}
	else
	{
		syscall_monitor_write("usage: isoget [-all]");
	}
	return 0;
}
