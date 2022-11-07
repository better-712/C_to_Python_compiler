
SRCS = main.cpp scanner.yy.cpp parser.tab.cpp spl_driver.cpp ast.cpp

bin/splc: $(SRCS)
	g++ -g -std=c++17 $(SRCS) -o $@

scanner.yy.cpp: scanner.l
	flex -o scanner.yy.cpp scanner.l

parser.tab.cpp: parser.y
	bison -t -d parser.y -o parser.tab.cpp

clean:
	rm -rf scanner.yy.cpp
	rm -rf parser.tab.cpp parser.tab.hpp stack.hh location.hh
	rm -rf bin/splc

