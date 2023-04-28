SYSTEM = x86-64_linux
LIBFORMAT = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

ifeq ($(SYSTEM),x86-64_linux)
	CPLEXDIR = /opt/ibm/ILOG/CPLEX_Studio129/cplex
	CONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio129/concert
endif

# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

ifeq ($(SYSTEM),x86-64_linux)
	CCC = g++ -O0
endif

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

ifeq ($(SYSTEM),x86-64_linux)
	CCOPT = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD
	COPT = -m64 -fPIC -fno-strict-aliasing
endif

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXBINDIR = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CCLNDIRS = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)

ifeq ($(SYSTEM),x86-64_linux)
	CCLNFLAGS = -lconcert -lilocplex -lcplex -lm -pthread -ldl
endif

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR = $(CPLEXDIR)/include

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 

#-------------------------------------------------------------

out: out.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o out out.o $(CCLNFLAGS)

out.o: main.cpp
	$(CCC) -c $(CCFLAGS) main.cpp -o out.o

# ------------------------------------------------------------

clean :
	/bin/rm -rf *.o *~
	/bin/rm -rf out
	/bin/rm -rf *.mps *.ord *.sos *.lp *.sav *.net *.msg *.log *.clp

# ------------------------------------------------------------

# Local Variables:
# mode: makefile
# End:
