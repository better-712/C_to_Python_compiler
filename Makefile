
SRCS = main.cpp scanner.yy.cpp parser.tab.cpp spl_driver.cpp ast.cpp

TESTS = test/test_1_r01.spl test/test_1_r02.spl

splc: $(SRCS)
	g++ -g -std=c++17 $(SRCS) -o $@

scanner.yy.cpp: scanner.l
	flex -o scanner.yy.cpp scanner.l

parser.tab.cpp: parser.y
	bison -t -d parser.y -o parser.tab.cpp

clean:
	rm -rf scanner.yy.cpp
	rm -rf parser.tab.cpp parser.tab.hpp stack.hh location.hh
	rm -rf splc

test: bin/splc $(TESTS)
	./bin/splc test/test_1_r01.spl > test/test_1_r01.txt
	./bin/splc test/test_1_r02.spl > test/test_1_r02.txt
	./bin/splc test/test_1_o01.spl > test/test_1_o01.txt
	diff test/test_1_r01.out test/test_1_r01.txt
	diff test/test_1_r02.out test/test_1_r02.txt
	diff test/test_1_o01.out test/test_1_o01.txt

gen: splc
	./splc test/test_1_r05.spl > test/test_1_r05.txt
	./splc test/test_1_r06.spl > test/test_1_r06.txt
	./splc test/test_1_r07.spl > test/test_1_r07.txt
	./splc test/test_1_r08.spl > test/test_1_r08.txt
	./splc test/test_1_r09.spl > test/test_1_r09.txt
	./splc test/test_1_r10.spl > test/test_1_r10.txt

.PHONY: clean test
