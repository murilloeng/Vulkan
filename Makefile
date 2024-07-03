#compiler
CXX = g++
LIBS += -l glfw -l vulkan
INCS += -I .. -I ../external/cpp/inc
WARS += -Wall -Wno-unused-variable -Wno-unused-result
WARS += -Wno-format-security -Wno-return-type -Wno-unused-function -Wformat-overflow=0
CXXFLAGS += -std=c++20 -fPIC -pipe -fopenmp -MT $@ -MMD -MP -MF $(subst .o,.d, $@) $(DEFS) $(INCS) $(WARS)

#mode
ifneq ($(m), r)
	mode = debug
	CXXFLAGS += -ggdb3
else
	mode = release
	CXXFLAGS += -Ofast
endif

#ouput
out = dist/$(mode)/vulkan.out

#sources
src := $(sort $(shell find -path './src/*.cpp'))

#shaders
vert := $(sort $(shell find -path './shd/*.vert'))
frag := $(sort $(shell find -path './shd/*.frag'))

#objects
obj_vert := $(sort $(addsuffix .spv, $(basename $(vert))_vert))
obj_frag := $(sort $(addsuffix .spv, $(basename $(frag))_frag))
obj := $(sort $(subst ./src/, build/$(mode)/, $(addsuffix .o, $(basename $(src)))))

#dependencies
dep := $(subst .o,.d, $(obj))

#rules
all : $(out)
	@echo 'build($(mode)): complete!'

run : $(out)
	@./$(out)

debug : 
	@gdb $(out) -x gdb.txt

$(out) : $(obj_vert) $(obj_frag) $(obj)
	@echo 'executable - $(mode): $@'
	@mkdir -p $(dir $@) && rm -rf $@
	@$(CXX) -fopenmp -o $(out) $(obj) $(LIBS)

shd/%_vert.spv : shd/%.vert
	@echo 'compiling - $(mode): $<'
	@glslc $< -o $@

shd/%_frag.spv : shd/%.frag
	@echo 'compiling - $(mode): $<'
	@glslc $< -o $@

shd/%_geom.spv : shd/%.geom
	@echo 'compiling - $(mode): $<'
	@glslc $< -o $@

build/$(mode)/%.o : src/%.cpp build/$(mode)/%.d
	@echo 'compiling - $(mode): $<'
	@mkdir -p $(dir $@) && rm -rf $@
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(dep) : ;

-include $(dep)

clean :
	@rm -rf dist/$(mode)
	@rm -rf build/$(mode)
	@echo 'clean($(mode)): complete!'

print-% :
	@echo $* = $($*)

.PHONY : all run gui fea clean print-%