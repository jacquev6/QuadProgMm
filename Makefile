lib: build/libquadprogmm.a

build/depend: src/QP/*.cpp src/QP/*.hpp
	@mkdir -p build
	g++ -IQuadProgpp/src -MM $^ | sed "s|\(.*\):|build/\1:|" >$@

include build/depend

build/%.o: src/QP/%.cpp
	@mkdir -p build
	g++ -IQuadProgpp/src -c $< -o $@

build/libquadprogmm.a: $(patsubst src/QP/%.cpp,build/%.o,$(wildcard src/QP/*.cpp))
	ar rvs $@ $^


post_doc: doc/user_guide/spring_chain_example.out doc/user_guide/quick_start.out

doc/user_guide/%.out: build/%.exe
	$^ >$@

build/%.exe: doc/user_guide/artifacts/%.cpp build/libquadprogmm.a
	g++ -Isrc $^ QuadProgpp/src/libquadprog.a -o $@
