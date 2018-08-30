FLAGS=-Wall -Werror -pedantic

build/depend: src/QP/*.cpp src/QP/*.hpp src/QP/*-test.cc
	@mkdir -p build
	g++ $(FLAGS) -IQuadProgpp/src -MM $^ | sed "s|\(.*\):|build/\1:|" >$@

include build/depend


tst: $(patsubst src/QP/%-test.cc,build/%-test.ok,$(wildcard src/QP/*-test.cc))

build/%-test.ok: build/%-test.exe
	$^
	touch $@

build/%-test.exe: src/QP/%-test.cc build/libquadprogmm.a QuadProgpp/src/libquadprog.a
	g++ $(FLAGS) -IQuadProgpp/src $^ -lboost_unit_test_framework -o $@


lib: build/libquadprogmm.a

build/libquadprogmm.a: $(patsubst src/QP/%.cpp,build/%.o,$(wildcard src/QP/*.cpp))
	ar rvs $@ $^

build/%.o: src/QP/%.cpp QuadProgpp/src/libquadprog.a
	@mkdir -p build
	g++ $(FLAGS) -IQuadProgpp/src -c $< -o $@

QuadProgpp/src/libquadprog.a:
	git submodule init
	git submodule update
	(cd QuadProgpp; cmake .; make)


post_doc: doc/spring_chain_example.out doc/quick_start.out doc/user_guide.out

doc/%.out: build/%.exe
	$^ >$@

build/%.exe: doc/artifacts/%.cpp build/libquadprogmm.a
	g++ $(FLAGS) -Isrc $^ QuadProgpp/src/libquadprog.a -o $@
