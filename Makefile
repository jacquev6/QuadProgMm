lib: build/libquadprogmm.a

build/depend: src/QP/*.cpp src/QP/*.hpp
	@mkdir -p build
	g++ -IQuadProgpp/src -MM $^ | sed "s|\(.*\):|build/\1:|" >$@

include build/depend

build/%.o: src/QP/%.cpp QuadProgpp/src/libquadprog.a
	@mkdir -p build
	g++ -IQuadProgpp/src -c $< -o $@

build/libquadprogmm.a: $(patsubst src/QP/%.cpp,build/%.o,$(wildcard src/QP/*.cpp))
	ar rvs $@ $^

QuadProgpp/src/libquadprog.a:
	git submodule init
	git submodule update
	(cd QuadProgpp; cmake .; make)

post_doc: doc/spring_chain_example.out doc/quick_start.out doc/user_guide.out

doc/%.out: build/%.exe
	$^ >$@

build/%.exe: doc/artifacts/%.cpp build/libquadprogmm.a
	g++ -Isrc $^ QuadProgpp/src/libquadprog.a -o $@
