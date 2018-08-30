FLAGS=-Wall -Werror -pedantic

build/depend: src/*.cpp src/*.hpp src/*-test.cc
	@mkdir -p build
	g++ $(FLAGS) -IQuadProgpp/src -MM $^ | sed "s|\(.*\):|build/\1:|" >$@

include build/depend


tst: $(patsubst src/%-test.cc,build/%-test.ok,$(wildcard src/*-test.cc))

build/%-test.ok: build/%-test.exe
	$^
	touch $@

build/%-test.exe: src/%-test.cc build/libquadprogmm.a QuadProgpp/src/libquadprog.a
	g++ $(FLAGS) -IQuadProgpp/src $^ -lboost_unit_test_framework -o $@


lib: build/libquadprogmm.a

build/libquadprogmm.a: $(patsubst src/%.cpp,build/%.o,$(wildcard src/*.cpp))
	ar rvs $@ $^

build/%.o: src/%.cpp QuadProgpp/src/libquadprog.a
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
