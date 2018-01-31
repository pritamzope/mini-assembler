CXX = g++
FLAGS = -Wall

MAIN_FILE = src/main.cpp
MAIN_OBJ_FILE = src/main.o
PASS_1_FILE = src/pass_1.cpp
PASS_1_OBJ_FILE = src/pass_1.o
PASS_2_FILE = src/pass_2.cpp
PASS_2_OBJ_FILE = src/pass_2.o
SYMTAB_FILE = src/symtab.cpp
SYMTAB_OBJ_FILE = src/symtab.o
OPTTAB_FILE = src/optab.cpp
OPTTAB_OBJ_FILE = src/optab.o
ERRORTAB_FILE = src/errortab.cpp
ERRORTAB_OBJ_FILE = src/errortab.o

OBJECTS_FILES = ${MAIN_OBJ_FILE} ${PASS_1_OBJ_FILE} ${PASS_2_OBJ_FILE} ${SYMTAB_OBJ_FILE} ${OPTTAB_OBJ_FILE} ${ERRORTAB_OBJ_FILE}


minassm : ${OBJECTS_FILES}
	${CXX} -o $@ ${OBJECTS_FILES}
	
${MAIN_OBJ_FILE} :
	${CXX} -c ${FLAGS} ${MAIN_FILE} -o $@

${PASS_1_OBJ_FILE} : 
	${CXX} -c ${FLAGS} ${PASS_1_FILE} -o $@

${PASS_2_OBJ_FILE} : 
	${CXX} -c ${FLAGS} ${PASS_2_FILE} -o $@
	
${SYMTAB_OBJ_FILE} : 
	${CXX} -c ${FLAGS} ${SYMTAB_FILE} -o $@

${OPTTAB_OBJ_FILE} : 
	${CXX} -c ${FLAGS} ${OPTTAB_FILE} -o $@

${ERRORTAB_OBJ_FILE} : 
	${CXX} -c ${FLAGS} ${ERRORTAB_FILE} -o $@
	
clean:
	rm -rf src/*.o

