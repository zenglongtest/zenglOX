// lspci.c -- 和lspci工具相关的函数定义

#include "common.h"
#include "syscall.h"
#include "pci.h"

PCI_DEVCONF_LST pci_devconf_lst = {0};

int main(VOID * task, int argc, char * argv[])
{
	UNUSED(task);

	if( argc == 1 || (argc == 2 && strIsNum(argv[1])) )
	{
		UINT32 count;
		syscall_pci_get_devconf_lst(&pci_devconf_lst);
		if(argc == 2)
			count = strToUInt(argv[1]);
		else
			count = pci_devconf_lst.count;
		if(pci_devconf_lst.ptr == NULL)
		{
			syscall_monitor_write("no pci device detected");
			return -1;
		}

		syscall_monitor_write("PCI dev list:\n");
		for(UINT32 i = 0; i < count ;i++)
		{
			syscall_monitor_write("bus [");
			syscall_monitor_write_dec((UINT32)pci_devconf_lst.ptr[i].cfg_addr.bus);
			syscall_monitor_write("] dev [");
			syscall_monitor_write_dec((UINT32)pci_devconf_lst.ptr[i].cfg_addr.device);
			syscall_monitor_write("] func [");
			syscall_monitor_write_dec((UINT32)pci_devconf_lst.ptr[i].cfg_addr.function);
			syscall_monitor_write("] vend id: ");
			syscall_monitor_write_hex((UINT32)pci_devconf_lst.ptr[i].cfg_hdr.vend_id);
			syscall_monitor_write(" dev id: ");
			syscall_monitor_write_hex((UINT32)pci_devconf_lst.ptr[i].cfg_hdr.dev_id);
			syscall_monitor_write("\n");
		}
		syscall_monitor_write("pci device total count: ");
		syscall_monitor_write_dec(pci_devconf_lst.count);
		//syscall_monitor_write(" total size: "); //debug
		//syscall_monitor_write_dec(pci_devconf_lst.size); //debug
		syscall_monitor_write("\n");
	}
	else
	{
		syscall_monitor_write("usage: lspci [shownum]");
	}
	return 0;
}

