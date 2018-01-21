DESTDIR?=/usr

OBJECTS:=main AutoWOL
LIBS:=boost_program_options
DIRS:=bin build
OBJECTS:=$(addprefix build/, $(addsuffix .o, ${OBJECTS} Version))
LIBS:=$(addprefix -l, ${LIBS})
LDPATHS:=$(addprefix -L, ${LDPATHS})

COMMIT:=$(shell git rev-parse HEAD)
VERSION:=$(shell git describe --exact-match --tags ${COMMIT})
ifeq (${.SHELLSTATUS},0)
	GEN_VERSION:=@echo "const char* version = \"${VERSION}\";" >> src/Version.cpp
else
	GEN_VERSION:=@echo "const char* version = \"git(${COMMIT}\";" >> src/Version.cpp
endif

.PHONY: all install clean

all: bin/autoWOL

${DIRS}: %:
	@mkdir $@

bin/autoWOL: ${OBJECTS} | bin
	@echo "Linking autoWol"
	@${CXX} ${LDFLAGS} -o $@ $^ ${LDPATHS} ${LIBS}

src/Version.cpp:
	@echo Generating $@
	${GEN_VERSION}

build/%.o: src/%.cpp | build
	@echo "Building $<"
	@${CXX} ${CXXFLAGS} ${INCLUDES} -c $< -o $@ -MT $@.d -MMD

install: bin/autoWOL
	@echo "Installing autoWOL to ${DESTDIR}/bin/autoWOl"
	@install -s $< ${DESTDIR}/bin

clean:
	@echo "Cleaning directories ${DIRS}"
	@rm -rf ${DIRS}
