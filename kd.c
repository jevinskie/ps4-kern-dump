#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>

#include <vm/vm.h>
#include <vm/vm_param.h>
#include <vm/pmap.h>
#include <vm/vm_map.h>
#include <vm/vm_object.h>
#include <vm/vm_page.h>
#include <vm/vm_pageout.h>
#include <vm/vm_extern.h>
#include <vm/uma.h>

#include "dump.h"
#include "ptdump.h"

typedef struct vm_map const *cvm_t;
typedef struct vm_map_entry const *cvme_t;

extern vm_map_t kernel_map;
extern vm_map_t kmem_map;
extern vm_map_t exec_map;
extern vm_map_t pipe_map;
extern vm_map_t buffer_map;
extern void *kernbase;

static void dump_map(cvm_t map) {
	int i = 0;
	uprintf("map: %p\n", map);
	uprintf("map->flags: 0x%02hhx\n", map->flags);
	uprintf("map->size: 0x%016lx\n", map->size);
	uprintf("map->system_map: 0x%02hhx\n", map->system_map);
	uprintf("map->pmap: %p\n", map->pmap);
	cvme_t head = &map->header;
	for (cvme_t vme = head; vme && (vme != head || i == 0); vme = vme->next) {
		uprintf("vme[%3d] = %p\n", i, vme);
		uprintf("vme[%3d]->start = 0x%016lx\n", i, vme->start);
		uprintf("vme[%3d]->end = 0x%016lx\n", i, vme->end);
		uprintf("vme[%3d]->avail_ssize = 0x%016lx\n", i, vme->avail_ssize);
		uprintf("vme[%3d]->adj_free = 0x%016lx\n", i, vme->adj_free);
		uprintf("vme[%3d]->max_free = 0x%016lx\n", i, vme->max_free);
		uprintf("vme[%3d]->object = %p\n", i, vme->object.vm_object);
		uprintf("vme[%3d]->offset = 0x%016lx\n", i, vme->offset);
		uprintf("vme[%3d]->eflags = 0x%08x\n", i, vme->eflags);
		uprintf("vme[%3d]->protection: 0x%02hhx\n", i, vme->protection);
		uprintf("vme[%3d]->max_protection: 0x%02hhx\n", i, vme->max_protection);
		uprintf("vme[%3d]->inheritance: 0x%02hhx\n", i, vme->inheritance);
		uprintf("vme[%3d]->wired_count = 0x%08x\n", i, vme->wired_count);
		uprintf("vme[%3d]->wired_count = 0x%08x\n", i, vme->wired_count);
		uprintf("vme[%3d]->lastr = 0x%016lx\n", i, vme->lastr);
		uprintf("vme[%3d]->cred = %p\n", i, vme->cred);
		++i;
	}
}

static int event_handler(struct module *module, int event, void *arg) {
	int e = 0;

	switch (event) {
		case MOD_LOAD:
			uprintf("kd load\n");
			uprintf("kernel_map: %p\n", kernel_map);
			printf("kd load\n");
			if (kernel_map) {
				uprintf("dumping kernel_map\n");
				dump_map(kernel_map);
			}
			if (kmem_map) {
				uprintf("dumping kmem_map\n");
				dump_map(kmem_map);
			}
			if (exec_map) {
				uprintf("dumping exec_map\n");
				dump_map(exec_map);
			}
			if (pipe_map) {
				uprintf("dumping pipe_map\n");
				dump_map(pipe_map);
			}
			if (buffer_map) {
				uprintf("dumping buffer_map\n");
				dump_map(buffer_map);
			}
			uprintf("dumping kernbase\n");
			printf("dumping kernbase\n");
			dump("kernbase", &kernbase, 0x10000);
			#if 1
			uprintf("dumping page tables\n");
			printf("dumping page tables\n");
			ptdump(NULL, 4);
			uprintf("done dumping\n");
			printf("done dumping\n");
			#endif
			break;
		case MOD_UNLOAD:
			uprintf("kd unload\n");
			printf("kd unload\n");
			break;
		default:
			e = EOPNOTSUPP;
			break;
	}

	return e;
}

static moduledata_t kd_conf = {
	"kd",
	 event_handler,
	 NULL
};

DECLARE_MODULE(kd, kd_conf, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);
