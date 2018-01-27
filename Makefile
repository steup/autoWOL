DESTDIR?=/usr

CXXFLAGS:=-O2 -gdwarf -flto=4 -fuse-linker-plugin -pthread
LDFLAGS:=-O1 -flto=4 -fuse-linker-plugin -pthread

OBJECTS:=main AutoWOL WOLTarget WOLPacket Error
LIBS:=boost_program_options boost_system
DIRS:=bin build
OBJECTS:=$(addprefix build/, $(addsuffix .o, ${OBJECTS} Version))
DEPS:=$(addsuffix .d, ${OBJECTS})
LIBS:=$(addprefix -l, ${LIBS})
LDPATHS:=$(addprefix -L, ${LDPATHS})

COMMIT:=$(shell git rev-parse HEAD)
VERSION:=$(shell git describe --exact-match --tags ${COMMIT})
ifeq (${.SHELLSTATUS},0)
else
	VERSION:=git(${COMMIT}
endif
GEN_VERSION:=@echo "const char* version = \"${VERSION}\";" >> src/Version.cpp

.PHONY: all install clean

all: bin/autoWOL
	@echo ${DEPS}

${DIRS}: %:
	@mkdir $@

bin/autoWOL: ${OBJECTS} | bin
	@echo "Linking autoWol"
	@${CXX} ${LDFLAGS} -o $@ $^ ${LDPATHS} ${LIBS}
	@rm src/Version.cpp

src/Version.cpp:
	@echo Generating $@
	${GEN_VERSION}

build/%.o: src/%.cpp | build
	@echo "Building $<"
	@${CXX} ${CXXFLAGS} ${INCLUDES} -c $< -o $@ -MF $@.d -MT $@.d -MMD

install: bin/autoWOL
	@echo "Installing autoWOL to ${DESTDIR}/bin/autoWOl"
	@install -s $< ${DESTDIR}/bin

clean:
	@echo "Cleaning directories ${DIRS}"
	@rm -rf ${DIRS}

-include ${DEPS}
