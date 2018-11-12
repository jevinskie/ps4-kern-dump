#include "ptdump.h"
#include "dump.h"

#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/endian.h>
#include <sys/libkern.h>
#include <sys/param.h>
#include <sys/lock.h>
#include <sys/mutex.h>
#include <sys/systm.h>
#include <vm/vm.h>
#include <vm/vm_page.h>
#include <vm/vm_pageout.h>
#include <vm/uma.h>
#include <vm/uma_int.h>
#include <machine/md_var.h>
#include <machine/vmparam.h>
#include <machine/vmparam.h>

#define pte_address(p) ((p)&0x000ffffffffff000UL)

static void output(void* p, void* data, uint64_t len) {
	dump("ptdump", data, len);
}

static uint64_t get_base(void* p) {
	return rcr3();
}

static uint64_t* read_page(void* p, uint64_t address) {
	return (uint64_t*)PHYS_TO_DMAP(address);
}

static void free_page(void* p, uint64_t* page) {
}

static uint64_t* ptdump_dump(void *p, uint64_t address) {
	uint64_t* page=read_page(p, address);
	if (page) {
		output(p, &address, 8);
		output(p, page, 4096);
	}
	return page;
}

static void ptdump_page(void *p, uint64_t address, int levels_remaining);

static void ptdump_recurse(void *p, uint64_t* page, int levels_remaining) {
	int i;
	for (i=0;i<512;i++) {
		if ((page[i]&1) && !(page[i]&(1<<7)))
			ptdump_page(p,pte_address(page[i]),levels_remaining-1);
	}
}

static void ptdump_page(void *p, uint64_t address, int levels_remaining) {
	uint64_t* page=ptdump_dump(p,address);
	if (!page) return;
	if (levels_remaining>0)
		ptdump_recurse(p,page,levels_remaining);
	free_page(p,page);
}

void ptdump(void *p, int levels) {
	ptdump_page(p,get_base(p),levels);
}
