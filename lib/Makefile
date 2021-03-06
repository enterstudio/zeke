# Zeke - /lib Makefile
#
# Copyright (c) 2014 - 2016 Olli Vanhoja <olli.vanhoja@cs.helsinki.fi>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met: 
#
# 1. Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer. 
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution. 
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Configuration files ##########################################################
# Generic config
include $(ROOT_DIR)/genconfig/buildconf.mk
include $(ROOT_DIR)/genconfig/dist.mk
include $(ROOT_DIR)/makefiles/commands.mk
################################################################################
# We use suffixes because it's fun
.SUFFIXES:                  	# Delete the default suffixes
.SUFFIXES: .c .o .h .S ._S	# Define our suffix list

# Supress warnings about C11 extensions in libc
CCFLAGS += -Wno-c11-extensions

# Generate a list of library names
ifeq ($(origin LIBS), undefined)
	LIBS := $(patsubst %/,%,$(shell ls -d */))
endif
# Include library makefiles
include $(patsubst %,%/lib.mk,$(LIBS))
# LIB_IDIRs to IDIR
IDIR += $(patsubst %,-I%,$(LIB_IDIR))

################################################################################
# Available selections for source code files:
SRC-	=# C sources
SRC-n	=#
SRC-y	=#
ASRC-	=# Assembly sources
ASRC-n	=#
ASRC-y	=#
# (A)SRC- and (A)SRC-n meaning that submodule won't be compiled

# All sources
SRC-y += $(foreach var,$(LIBS), $($(var)-SRC-y))
ASRC-y += $(foreach var,$(LIBS), $($(var)-ASRC-y))

# Assembly Obj files
ASOBJS := $(patsubst %.S, %.o, $(ASRC-y))

# C Obj files
OBJS := $(patsubst %.c, %.o, $(SRC-y))

# Deps
DEPS := $(patsubst %.c, %.d, $(SRC-y))

# a files for static librarries
LIBSA = $(addsuffix .a, $(LIBS))


all: | $(LIBS) $(LIBSA)
.PHONY: $(LIBS) gen_idir clean

$(LIBS):
	@if [ -f $@/Makefile ]; then $(MAKE) -C $@ all; fi

# Generate a list of include directories required by the libraries
gen_idir_mkfile:
	echo "LIB_IDIR += $(LIB_IDIR)" >> lib_idir.gen.mk

$(ASOBJS): %.o: %.S $(AUTOCONF_H)
	$(eval AIDIR := $(IDIR))
	$(as-command)

$(OBJS): %.o: %.c $(AUTOCONF_H)
	$(cc-command)

-include $(DEPS)

$(LIBSA): $(OBJS) $(ASOBJS)
	$(eval CUR_LIB := $(basename $@))
	$(eval CUR_OBJS := $(patsubst %.S, %.o, $($(CUR_LIB)-ASRC-y)))
	$(eval CUR_OBJS += $(patsubst %.c, %.o, $($(CUR_LIB)-SRC-y)))
	$(eval CUR_ARFLAGS := $($(CUR_LIB)-ARFLAGS))
	@echo "AR $@"
	@ar rcs $@ $(CUR_OBJS) $(CUR_ARFLAGS)

clean:
	$(RM) $(ASOBJS) $(OBJS) $(DEPS)
	$(RM) *.a
	$(RM) lib_idir.gen.mk
	$(foreach dir,$(LIBS),if [ -f $(dir)/Makefile ]; then $(MAKE) -C $(dir) clean; fi;)
