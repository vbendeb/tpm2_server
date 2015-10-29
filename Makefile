
TPM2_SRC ?= $(shell readlink -f ../tpm2)
obj ?= $(shell readlink -f ..)/build/tpm2_server
CC = $(CROSS_COMPILE)gcc

CFLAGS =
ifneq ($(DEBUG),)
CFLAGS += -g -O0
endif
CFLAGS += -I $(TPM2_SRC)
CFLAGS += -Wall
CFLAGS += -Wdeclaration-after-statement
CFLAGS += -Werror
CFLAGS += -Werror-implicit-function-declaration
CFLAGS += -Wmaybe-uninitialized
CFLAGS += -Wno-format-security
CFLAGS += -Wno-pointer-sign
CFLAGS += -Wno-trigraphs
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wuninitialized
CFLAGS += -c
CFLAGS += -fconserve-stack
CFLAGS += -fno-delete-null-pointer-checks
CFLAGS += -fno-strict-aliasing
CFLAGS += -fno-strict-overflow
CFLAGS += -fshort-wchar

# Use V=1 for verbose output
ifeq ($(V),)
Q := @
else
Q :=
endif

# This is the default target
all: $(obj)/ntpm

SOURCES = ftdi_spi_tpm.c  mpsse.c  ntpm.c  support.c  tpm2lib_wrapper.c

OBJS = $(patsubst %.c,$(obj)/%.o,$(SOURCES))
DEPS = $(patsubst %.c,$(obj)/%.d,$(SOURCES))

export V DEBUG CFLAGS
.PHONY: $(obj)/libtpm2.a
$(obj)/libtpm2.a:
	make -C $(TPM2_SRC) obj=$(obj)
	$(Q)$(AR) scr $@ $^

$(obj):
	@echo "  MKDIR   $(obj)"
	$(Q)mkdir -p $(obj)

$(obj)/%.d $(obj)/%.o: %.c | $(obj)
	@echo "  CC      $(notdir $<)"
	$(Q)$(CC) $(CFLAGS) -c -MMD -MF $(basename $@).d -o $(basename $@).o $<

.PHONY: clean
clean:
	@echo "  RM      $(obj)"
	$(Q)rm -rf $(obj)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

$(obj)/ntpm: $(OBJS) $(obj)/libtpm2.a
	gcc -o $@ $^  -lftdi -lcrypto
