KMOD = kd
SRCS =  kd.c dump.c z85.c ptdump.c
CFLASGS = -O0 -g -fno-strict-aliasing

.include <bsd.kmod.mk>
