#include "dump.h"

#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/endian.h>
#include <sys/libkern.h>

#include "assert.h"

#include "z85.h"

static void dump_helper(const char name[DUMP_NAME_SIZE], void *addr, uint32_t len, dump_record_t *rec) {
	assert(len <= DUMP_DATA_SIZE);
	assert(strlen(name) + 1 <= DUMP_NAME_SIZE);

	rec->addr = htobe64((uint64_t)addr);
	rec->len = htobe32(len);
	memset(rec->name, DUMP_NAME_SIZE, 0);
	strncpy(rec->name, name, DUMP_NAME_SIZE);
	memcpy(rec->data, addr, len);
}

void dump(const char name[DUMP_NAME_SIZE], void *addr, uint32_t len) {
	dump_record_t rec;
	char z85_buf[Z85_ENC_BUF_SIZE(sizeof(rec))];
	uintptr_t a = (uintptr_t)addr;

	assert(strlen(name) + 1 <= DUMP_NAME_SIZE);

	uprintf("dumping '%s' at %p size %u\n", name, addr, len);

	while (len > 0) {
		uint32_t rec_data_len = len < DUMP_DATA_SIZE ? len : DUMP_DATA_SIZE;
		dump_helper(name, (void *)a, rec_data_len, &rec);
		len -= rec_data_len;
		a += rec_data_len;
		char *z85_enc = Z85_encode(
			&rec,
			sizeof(rec) - DUMP_DATA_SIZE + rec_data_len,
			z85_buf, sizeof(z85_buf)
		);
		uprintf("%s%s\n", DUMP_TAG, z85_enc);
	}
}
