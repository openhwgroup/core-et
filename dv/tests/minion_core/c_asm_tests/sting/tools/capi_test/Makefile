CXX = ccache g++
CXXFLAGS = -std=c++11 -Imock -Icapi -Og -g
CPPFLAGS = -MMD -MP
LDFLAGS =
LDLIBS =
RM = rm -rf

target = testme

.PHONY: all
all: $(target)

.PHONY: clean
clean:
	$(RM) $(target)  build

.PHONY: test
test: $(target)
	./$(target)

mock_srcs = mock/mock.cpp \
			mock/cpu_resource.cpp \
			mock/board_conf.cpp
capi_srcs = $(wildcard capi/*.cpp)
test_srcs = $(wildcard test/*.cpp)

define add_library
$(1)_lib = build/lib$(1).a
$(1)_objs = $$($(1)_srcs:%.cpp=build/%.o)
$(1)_deps = $$($(1)_objs:%.o=%.d)

$$($(1)_lib): $$($(1)_objs)
	$$(AR) $$(ARFLAGS) $$@ $$^

-include $$($(1)_deps)
endef

$(eval $(call add_library,mock))
$(eval $(call add_library,capi))
$(eval $(call add_library,test))

$(target): $(test_objs) $(mock_lib) $(capi_lib)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

.SECONDEXPANSION:

build/%.o: %.cpp
	@mkdir -p $(@D)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

