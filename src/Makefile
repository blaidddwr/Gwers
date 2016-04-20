.RECIPEPREFIX := +
MAKEFLAGS := $(MAKEFLAGS) --no-print-directory
CXX := g++
NAME := gwers
FILES := Gwers.files



build := ../build/
run := ../run/
lib := ../lib/
incl := ../include/

acxxflags := $(CXXFLAGS) -g -std=c++11
aldflags := $(LDFLAGS)
aldlibs := $(LDLIBS)

libf := $(lib)lib$(NAME).a
libfd1 := $(lib)lib$(NAME).d1.a
libfd2 := $(lib)lib$(NAME).d2.a

raw := $(shell cat $(FILES))
utest := $(filter %.cxx,$(raw))
library := $(filter %.cpp,$(raw))

dpds := $(addprefix $(build),$(library:%.cpp=%.d))
objs := $(addprefix $(build),$(library:%.cpp=%.m.o))
objsd1 := $(addprefix $(build),$(library:%.cpp=%.d1.o))
objsd2 := $(addprefix $(build),$(library:%.cpp=%.d2.o))

udpds := $(dpds) $(addprefix $(build),$(utest:%.cxx=%.t.d))
uobjs := $(objs:%.m.o=%.d2.o) $(addprefix $(build),$(utest:%.cxx=%.t.o))

alldpds := $(udpds)

hdrs := $(addprefix $(incl),$(filter-out %.hh,$(shell ls *.h)))



.PHONY: clean all library test check doc

all: library libraryd1 libraryd2 test
library: $(libf) $(hdrs)
libraryd1: $(libfd1) $(hdrs)
libraryd2: $(libfd2) $(hdrs)
test: $(run)unit

include $(alldpds)

$(libf): $(objs) $(dpds)
+@echo "Building library(metal)."
+@ar rc $@ $(objs)
+@ranlib $@

$(libfd1): $(objsd1) $(dpds)
+@echo "Building library(debug1)."
+@ar rc $@ $(objsd1)
+@ranlib $@

$(libfd2): $(objsd2) $(dpds)
+@echo "Building library(debug2)."
+@ar rc $@ $(objsd2)
+@ranlib $@

$(run)unit: $(uobjs) $(udpds)
+@echo "Building unit tests."
+@$(CXX) $(uobjs) $(aldflags) $(aldlibs) -o $@

depend: $(alldpds)
+@echo Done.

$(build)%.m.o : %.cpp
+@echo "Building object $@"
+@$(CXX) $(acxxflags) -c $< -o $(build)$@

$(build)%.d1.o : %.cpp
+@echo "Building object $@"
+@$(CXX) -D DEBUG $(acxxflags) -c $< -o $(build)$@

$(build)%.d2.o : %.cpp
+@echo "Building object $@"
+@$(CXX) -D DTRACE -D DEBUG $(acxxflags) -c $< -o $(build)$@

$(build)%.t.o : %.cxx
+@echo "Building object $@"
+@$(CXX) -D DTRACE -D DEBUG $(acxxflags) -c $< -o $(build)$@

$(incl)%.h: %.h
+@echo "Linking $<"
+@cp $< $@

$(build)%.d: %.cpp
+@echo "Building depend $@"
+@echo -n "$@ $(build)$*.d1.o $(build)$*.d2.o $(build)" > $@
+@$(CXX) $(acxxflags) -MM $< | sed 's/.o:/.m.o:/' >> $@

$(build)%.t.d: %.cxx
+@echo "Building depend $@"
+@echo -n "$@ $(build)" > $@
+@$(CXX) $(acxxflags) -MM $< | sed 's/.o:/.t.o:/' >> $@

check: test
+@cd $(run) && ./unit

clean:
+@echo "Cleaning all."
+@rm -f $(build)*.o $(run)unit

depclean:
+@echo "Cleaning all dependency files."
+@rm -f $(build)*.d

doc:
+@echo "Making documentation."
+@doxygen
