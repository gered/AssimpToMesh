# this Makefile is based on the ones from devkitPro
# TODO: probably should simplify it a bunch (mainly the rules at the bottom)

TARGET		:= assimptomesh



#-------------------------------------------------------------------------------
# detect build configuration
#-------------------------------------------------------------------------------
ifndef ("CFG")
CFG=Debug
endif

ifeq ("$(CFG)","Debug")
BUILD := Debug
endif
ifeq ("$(CFG)","Release")
BUILD := Release
endif
ifeq ("$(BUILD)", "")
$(error invalid configuration specified via CFG argument)
endif

$(info $(CFG) configuration selected)



#-------------------------------------------------------------------------------
# set up platform specifics
#-------------------------------------------------------------------------------
UNAME := $(shell uname)

#-------------------------------------------------------------------------------
# default windows libraries and options
PLATFORM          := win32
PLATFORM_CFLAGS   := 
PLATFORM_LD_FLAGS := -static-libgcc -static-libstdc++
PLATFORM_LIBS     := -lassimp
PLATFORM_DEFINES  := 
#-------------------------------------------------------------------------------
ifeq ($(UNAME), Linux)
# it appears that we're running on linux
PLATFORM          := linux
PLATFORM_CFLAGS   := 
PLATFORM_LD_FLAGS := 
PLATFORM_LIBS     := -lassimp
PLATFORM_DEFINES  := 
endif
#-------------------------------------------------------------------------------
ifeq ($(UNAME), Darwin)
# NOTE: this is set up assuming Assimp was installed via Homebrew. Unfortunately
#       the library version is included in the path, so this will have to be
#       updated if we need to build against a newer version of Assimp
PLATFORM          := osx
PLATFORM_CFLAGS   := -I/usr/local/Cellar/assimp/2.0.863/include/
PLATFORM_LD_FLAGS := -L/usr/local/Cellar/assimp/2.0.863/
PLATFORM_LIBS     := -lassimp
PLATFORM_DEFINES  := 
endif
#-------------------------------------------------------------------------------



#-------------------------------------------------------------------------------
ifeq ("$(BUILD)","Debug")
DEFINES		:= -DDEBUG -DDEBUG_ASSERT_BREAK $(PLATFORM_DEFINES)
CFLAGS		:= $(DEFINES) -g -Wall $(PLATFORM_CFLAGS)
CXXFLAGS	:= $(CFLAGS)
LDFLAGS		:= $(PLATFORM_LD_FLAGS)
LIBS		:= -g $(PLATFORM_LIBS)
endif
#-------------------------------------------------------------------------------
ifeq ("$(BUILD)","Release")
DEFINES		:= $(PLATFORM_DEFINES)
CFLAGS		:= $(DEFINES) -O2 -Wall $(PLATFORM_CFLAGS)
CXXFLAGS	:= $(CFLAGS)
LDFLAGS		:= -O2 $(PLATFORM_LD_FLAGS)
LIBS		:= $(PLATFORM_LIBS)
endif
#-------------------------------------------------------------------------------



#-------------------------------------------------------------------------------
# find sources, setup commands
#-------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#-------------------------------------------------------------------------------

SOURCES		:= ${shell find ./src -type d}
export OUTPUT	:= $(TARGET)
export VPATH	:= $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

CFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

export CXX	:= g++
export CC	:= g++
export LD	:= g++
export OFILES	:= $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

.PHONY: $(BUILD) clean

#-------------------------------------------------------------------------------
$(BUILD):
	$(info building ...)
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#-------------------------------------------------------------------------------
clean:
	$(info clean ...)
	@rm -fr $(BUILD)

#-------------------------------------------------------------------------------
else



#-------------------------------------------------------------------------------
# main targets
#-------------------------------------------------------------------------------
$(OUTPUT): $(OFILES)
	$(info linking $(notdir $@))
	$(LD) -o $@ $(LDFLAGS) $(OFILES) $(LIBS)
	cp $(OUTPUT) ../




#-------------------------------------------------------------------------------
# build rules by filetype
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
%.a:
#-------------------------------------------------------------------------------
	$(info $(notdir $<))
	@rm -f $@
	@$(AR) -rc $@ $^

#-------------------------------------------------------------------------------
%.o: %.cpp
	$(info $(notdir $<))
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

#-------------------------------------------------------------------------------
%.o: %.c
	$(info $(notdir $<))
	@$(CC) -c $< -o $@ $(CFLAGS) 

#-------------------------------------------------------------------------------
%.o: %.s
	$(info $(notdir $<))
	@$(CC) -x assembler-with-cpp $(ASFLAGS) -c $< -o $@

#-------------------------------------------------------------------------------
%.o: %.S
	@echo $(notdir $<)
	@$(CC) -x assembler-with-cpp $(ASFLAGS) -c $< -o $@

#-------------------------------------------------------------------------------
endif
#-------------------------------------------------------------------------------

