PROJECT = nrf24le1-bme280

TARGET = _target_sdcc_nrf24le1_32
MEMORYMODEL = --model-large

HEX2BIN = ../hex2bin/hex2bin
SDCC = sdcc

BUILD := build
OBJDIR := $(BUILD)/obj
DEPDIR := $(BUILD)/dep

SOURCES := $(wildcard *.c)
SRCFILES := $(subst main.c,,$(SOURCES))
DEPFILES := $(subst .c,.d,$(addprefix $(DEPDIR)/,$(SRCFILES)))
DEPFILES += $(DEPDIR)/main.d
OBJFILES := $(SRCFILES:.c=.rel)
OBJFILES := $(addprefix $(OBJDIR)/,$(OBJFILES))

LDSDCC = -L/usr/local/share/sdcc/lib/large -lmcs51 -llibsdcc
LDNRF = -Lsdk/$(TARGET)/lib -lnrf24le1

LIBNRF = sdk/$(TARGET)/lib/nrf24le1.lib

CCFLAGS=--model-large --std-c99 --opt-code-size -Isdk/include -Isdk/$(TARGET)/include/
LDFLAGS=--code-loc 0x0000 --code-size 0x4000 --xram-loc 0x0000 --xram-size 0x400 $(LDSDCC) $(LDNRF)

PROJECTBIN = $(BUILD)/$(PROJECT).bin
CRC8CALC = tools/crc8calc

all: $(LIBNRF) $(PROJECTBIN)

-include $(DEPFILES)

$(OBJDIR)/%.rel : %.c $(DEPFILES)
	@echo
	@echo "Building object file '$@'"
	[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR) > /dev/null
	$(SDCC) -c $(CCFLAGS) $< -o $(OBJDIR)/
	@echo "Finished building object file '$@'"

$(BUILD)/$(PROJECT).ihx : $(OBJFILES)
	@echo
	@echo "Building hex file '$@'"
	$(SDCC) $(CCFLAGS) $(OBJFILES) main.c -o $@ $(LDFLAGS) 
	@echo "Finished building hex file '$@'"

$(BUILD)/%.bin: $(BUILD)/%.ihx
	@echo
	@echo "Building binary file '$@'"
	$(HEX2BIN) -p 00 $@ $^
	@echo "Finished building binary file '$@'"
	# copy to home
	cp -vf $(PROJECTBIN) ~/ 

$(LIBNRF):
	make -C sdk all

$(DEPDIR)/%.d: %.c %.h
	@echo
	@echo "Building dependency file '$@'"
	[ -d $(DEPDIR) ] || mkdir -p $(DEPDIR) > /dev/null
	$(SDCC) -MM $(CCFLAGS) $< >> $@.tmp
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp > $@
	rm -f $@.tmp
	@echo "Finished building dependency file '$@'"

tools: tools/crc8calc

tools/crc8calc : tools/crc8calc.c
	@echo
	@echo "Building '$@'"
	gcc $< -o $@
	@echo "Finished building '$@'"

clean:
	rm -rf  $(BUILD)/* && rmdir $(BUILD)

cleanall:
	rm -rf  $(BUILD)/* && rmdir $(BUILD)
	make -C sdk clean
	
.PHONY: all clean cleanall tools
