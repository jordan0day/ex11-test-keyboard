ifeq ($(PLATFORM),)
  UNAME_S=$(shell uname -s)
	
	ifeq ($(UNAME_S), Linux)
	PLATFORM=linux
	else ifeq ($(UNAME_S), Darwin)
	PLATFORM=macos
	else
	$(error Sorry, builds not yet supported for platform: $(UNAME_S)!)
	endif
	
	export PLATFORM
endif

ifeq ($(PLATFORM), linux)
  FILE=Makefile.linux
else ifeq ($(PLATFORM), macos)
	FILE=Makefile.macos
endif

$(info Performing build for $(PLATFORM)...)

BASEDIR :=$(shell pwd)
PRIVDIR := $(BASEDIR)/priv

ERL_CFLAGS ?= -I$(ERTS_INCLUDE_DIR) -I$(ERL_EI_INCLUDE_DIR)
ERL_LDFLAGS ?= -L$(ERL_EI_LIBDIR) -lerl_interface -lei

CFLAGS = -Wall -O3 -std=c99

include $(FILE)

.PHONY:	all clean

all: ensure_priv $(NIF)

$(OUTDIR)/%.o: c_src/%.c
	$(CC) -c $(ERL_CFLAGS) $(CFLAGS) -o $@ $<

$(NIF): $(OUTDIR)/ex11_nif.o
	$(CC) -shared $^ $(ERL_LDFLAGS) $(LDFLAGS) -o $@

ensure_priv:
	mkdir -p $(OUTDIR)

clean:
	$(RM) -r $(OUTDIR)

