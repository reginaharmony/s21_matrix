# ======================= GLOSSARY ⊂(｡•́‿•̀｡⊃)

GCC = gcc -Wall -Werror -Wextra -pedantic -std=c11 -g 
GCOV=-fprofile-arcs -ftest-coverage

OS = $(shell uname -s)
ifeq ($(OS), Darwin)
LC = -lcheck
else ifeq ($(OS), Linux)
LC = -lcheck -lsubunit -lpthread -lrt -lm -D_GNU_SOURCE
endif

LIB = s21_matrix.a
TESTS = tests/*.c
TESTN = test
CLANG = clang-format -style=Google

# ======================= TARGETS ⊂(｡•́‿•̀｡⊃)

all: $(LIB)

$(LIB): 
	$(GCC) -c *.c && ar rc $(LIB) *.o && ranlib $(LIB)

test: $(LIB)
	$(GCC) --coverage $(TESTS) $(LIB) -o $(TESTN) $(LC) && ./$(TESTN)

clean:
	rm -rf $(TESTN) *.o *.a *.gch *.gcno *.log *.gcda report/ *.info *.dSYM/

gcov_report: clean
	$(GCC) $(GCOV) *.c  $(TESTS) -o $(TESTN) $(LC) && ./$(TESTN)
	lcov -t "$(TESTN)" -o $(TESTN).info -c -d .
	genhtml -o report $(TESTN).info
	open ./report/index.html
	rm -rf *.gcda *.gcno $(TESTN)

# ======================= CHECKS ⊂(｡•́‿•̀｡⊃)

style:
	$(CLANG) -n *.c *.h && $(CLANG) -i *.c *.h
	$(CLANG) -n */*.c *.h && $(CLANG) -i */*.c *.h

leaks: clean test
	CK_FORK=no leaks -atExit -- ./$(TESTN)  $t > testleaks.log
	open testleaks.log

valgrind: test
	valgrind --tool=memcheck --leak-check=full ./$(TESTN)