#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/user.h>
#include <libutil.h>
#include <sys/sysctl.h>
#include <kvm.h>
#include <sys/param.h>
#include <sys/queue.h>
#include <sys/socket.h>
#include <libprocstat.h>

size_t getKernelStacks(void *destination) {
	int name[4];
	size_t len;

	name[0] = CTL_KERN;
	name[1] = KERN_PROC;
	name[2] = KERN_PROC_KSTACK;
	name[3] = syscall(20);

	sysctl(name, 4, destination, &len, NULL, 0);

	return len;
}

int main(int argc, const char **argv) {
	int cnt;
	pid_t pid;

	assert(argc == 2);
	pid = atoi(argv[1]);

	struct kinfo_vmentry *vme = kinfo_getvmmap(pid, &cnt);
	printf("cnt: %d\n", cnt);

	for (int i = 0; i < cnt; ++i) {
		printf("vme[%3d].kve_structsize       = 0x%08x\n", i, vme[i].kve_structsize);
		printf("vme[%3d].kve_type             = 0x%08x\n", i, vme[i].kve_type);
		printf("vme[%3d].kve_start            = 0x%016lx\n", i, vme[i].kve_start);
		printf("vme[%3d].kve_end              = 0x%016lx\n", i, vme[i].kve_end);
		printf("vme[%3d].kve_offset           = 0x%016lx\n", i, vme[i].kve_offset);
		printf("vme[%3d].kve_vn_fileid        = 0x%016lx\n", i, vme[i].kve_vn_fileid);
		printf("vme[%3d].kve_vn_fsid          = 0x%08x\n", i, vme[i].kve_vn_fsid);
		printf("vme[%3d].kve_flags            = 0x%08x\n", i, vme[i].kve_flags);
		printf("vme[%3d].kve_resident         = 0x%08x\n", i, vme[i].kve_resident);
		printf("vme[%3d].kve_private_resident = 0x%08x\n", i, vme[i].kve_private_resident);
		printf("vme[%3d].kve_protection       = 0x%08x\n", i, vme[i].kve_protection);
		printf("vme[%3d].kve_ref_count        = 0x%08x\n", i, vme[i].kve_ref_count);
		printf("vme[%3d].kve_shadow_count     = 0x%08x\n", i, vme[i].kve_shadow_count);
		printf("vme[%3d].kve_vn_type          = 0x%08x\n", i, vme[i].kve_vn_type);
		printf("vme[%3d].kve_vn_size          = 0x%016lx\n", i, vme[i].kve_vn_size);
		printf("vme[%3d].kve_vn_rdev          = 0x%08x\n", i, vme[i].kve_vn_rdev);
		printf("vme[%3d].kve_vn_mode          = 0x%04hx\n", i, vme[i].kve_vn_mode);
		printf("vme[%3d].kve_status           = 0x%04hx\n", i, vme[i].kve_status);
		for (int j = 0; j < 12; ++j) {
			printf("vme[%3d]._kve_ispare[%2d]      = 0x%08x\n", i, j, vme[i]._kve_ispare[j]);
		}
		printf("vme[%3d].kve_path             = '%s'\n", i, vme[i].kve_path);
	}

	struct procstat *sysctl_ps = procstat_open_sysctl();
	int kipp_cnt;
	kvm_t *kvm = kvm_open(NULL, "/dev/mem", NULL, 0, "kvm error: ");
	struct kinfo_proc *kipp = kvm_getprocs(kvm, KERN_PROC_ALL, 0, &kipp_cnt);
	(void)kipp;
	procstat_close(sysctl_ps);

	return 0;
}
