# directory declaration
LIB_COMMON_DIR = $(LIBRARIES_ROOT)/common

LIB_COMMON_ASMSRCDIR	= $(LIB_COMMON_DIR) $(LIB_COMMON_DIR)/ez_sio $(LIB_COMMON_DIR)/ringbuffer $(LIB_COMMON_DIR)/xprintf
LIB_COMMON_CSRCDIR	= $(LIB_COMMON_DIR) $(LIB_COMMON_DIR)/ez_sio $(LIB_COMMON_DIR)/ringbuffer $(LIB_COMMON_DIR)/xprintf
LIB_COMMON_INCDIR	= $(LIB_COMMON_DIR) $(LIB_COMMON_DIR)/ez_sio $(LIB_COMMON_DIR)/ringbuffer $(LIB_COMMON_DIR)/xprintf

# find all the source files in the target directories
LIB_COMMON_CSRCS = $(call get_csrcs, $(LIB_COMMON_CSRCDIR))
LIB_COMMON_ASMSRCS = $(call get_asmsrcs, $(LIB_COMMON_ASMSRCDIR))

# get object files
LIB_COMMON_COBJS = $(call get_relobjs, $(LIB_COMMON_CSRCS))
LIB_COMMON_ASMOBJS = $(call get_relobjs, $(LIB_COMMON_ASMSRCS))
LIB_COMMON_OBJS = $(LIB_COMMON_COBJS) $(LIB_COMMON_ASMOBJS)

# get dependency files
LIB_COMMON_DEPS = $(call get_deps, $(LIB_COMMON_OBJS))

# extra macros to be defined
LIB_COMMON_DEFINES = -DLIB_COMMON

# genearte library
LIB_LIB_COMMON = $(OUT_DIR)/liblibcommon.a


# library generation rule
$(LIB_LIB_COMMON): $(LIB_COMMON_OBJS)
	$(TRACE_ARCHIVE)
	$(Q)$(AR) $(AR_OPT) $@ $(LIB_COMMON_OBJS)

# specific compile rules
# user can add rules to compile this middleware
# if not rules specified to this middleware, it will use default compiling rules

# Middleware Definitions
LIB_INCDIR += $(LIB_COMMON_INCDIR)
LIB_CSRCDIR += $(LIB_COMMON_CSRCDIR)
LIB_ASMSRCDIR += $(LIB_COMMON_ASMSRCDIR)

LIB_CSRCS += $(LIB_COMMON_CSRCS)
LIB_CXXSRCS +=
LIB_ASMSRCS += $(LIB_COMMON_ASMSRCS)
LIB_ALLSRCS += $(LIB_COMMON_CSRCS) $(LIB_COMMON_ASMSRCS)

LIB_COBJS += $(LIB_COMMON_COBJS)
LIB_CXXOBJS +=
LIB_ASMOBJS += $(LIB_COMMON_ASMOBJS)
LIB_ALLOBJS += $(LIB_COMMON_OBJS)

LIB_DEFINES += $(LIB_COMMON_DEFINES)
LIB_DEPS += $(LIB_COMMON_DEPS)
LIB_LIBS += $(LIB_LIB_COMMON)