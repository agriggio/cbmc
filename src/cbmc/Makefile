SRC = cbmc_main.cpp cbmc_parse_options.cpp bmc.cpp cbmc_dimacs.cpp \
      cbmc_languages.cpp counterexample_beautification.cpp \
      bv_cbmc.cpp symex_bmc.cpp show_vcc.cpp cbmc_solvers.cpp \
      xml_interface.cpp cover.cpp all_properties.cpp

OBJ += ../ansi-c/ansi-c$(LIBEXT) \
      ../linking/linking$(LIBEXT) \
      ../big-int/big-int$(LIBEXT) \
      ../goto-programs/goto-programs$(LIBEXT) \
      ../goto-symex/goto-symex$(LIBEXT) \
      ../pointer-analysis/value_set$(OBJEXT) \
      ../pointer-analysis/value_set_analysis_fi$(OBJEXT) \
      ../pointer-analysis/value_set_domain_fi$(OBJEXT) \
      ../pointer-analysis/value_set_fi$(OBJEXT) \
      ../pointer-analysis/value_set_dereference$(OBJEXT) \
      ../pointer-analysis/dereference_callback$(OBJEXT) \
      ../pointer-analysis/add_failed_symbols$(OBJEXT) \
      ../pointer-analysis/rewrite_index$(OBJEXT) \
      ../pointer-analysis/goto_program_dereference$(OBJEXT) \
      ../goto-instrument/full_slicer$(OBJEXT) \
      ../goto-instrument/nondet_static$(OBJEXT) \
      ../analyses/analyses$(LIBEXT) \
      ../langapi/langapi$(LIBEXT) \
      ../xmllang/xmllang$(LIBEXT) \
      ../assembler/assembler$(LIBEXT) \
      ../solvers/solvers$(LIBEXT) \
      ../util/util$(LIBEXT)

INCLUDES= -I ..

LIBS =

include ../config.inc
include ../common

CLEANFILES = cbmc$(EXEEXT)

all: cbmc$(EXEEXT)

ifneq ($(wildcard ../bv_refinement/Makefile),)
  OBJ += ../bv_refinement/bv_refinement$(LIBEXT)
  CP_CXXFLAGS += -DHAVE_BV_REFINEMENT
endif

ifneq ($(wildcard ../cpp/Makefile),)
  OBJ += ../cpp/cpp$(LIBEXT)
  CP_CXXFLAGS += -DHAVE_CPP
endif

ifneq ($(wildcard ../java_bytecode/Makefile),)
  OBJ += ../java_bytecode/java_bytecode$(LIBEXT)
  CP_CXXFLAGS += -DHAVE_JAVA_BYTECODE
  ifneq ($(LIBZIPLIB),)
    LIBS += -L $(LIBZIPLIB) -lzip -lz
  endif
endif

ifneq ($(wildcard ../specc/Makefile),)
  OBJ += ../specc/specc$(LIBEXT)
  CP_CXXFLAGS += -DHAVE_SPECC
endif

ifneq ($(wildcard ../php/Makefile),)
  OBJ += ../php/php$(LIBEXT)
  CP_CXXFLAGS += -DHAVE_PHP
endif

###############################################################################

cbmc$(EXEEXT): $(OBJ)
	$(LINKBIN)

.PHONY: cbmc-mac-signed

cbmc-mac-signed: cbmc$(EXEEXT)
	strip cbmc$(EXEEXT) ; codesign -v -s $(OSX_IDENTITY) cbmc$(EXEEXT)

