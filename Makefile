CPP  = g++ 
OBJ  = demo_cgt_EGH_decode.o demo_cgt_EGH.o lemon/arg_parser.o 
LINKOBJ  = lemon/arg_parser.o 
LIBS =  
CXXFLAGS = -I . -O3 
RM = rm -f
BIN = generate_egh decode_egh

all: lemon/arg_parser.o demo_cgt_EGH.o demo_cgt_EGH_decode.o $(BIN)

clean: 
	${RM} $(OBJ) $(BIN) 

generate_egh: demo_cgt_EGH.o $(LINKOBJ)
	$(CPP) $(LINKOBJ) demo_cgt_EGH.o -o "generate_egh" $(LIBS)

decode_egh: demo_cgt_EGH_decode.o $(LINKOBJ)
	$(CPP) $(LINKOBJ) demo_cgt_EGH_decode.o -o "decode_egh" $(LIBS)

demo_cgt_EGH_decode.o: demo_cgt_EGH_decode.cpp cgt_codegen_EGH_decode.h
	$(CPP) -c demo_cgt_EGH_decode.cpp -o demo_cgt_EGH_decode.o $(CXXFLAGS)

demo_cgt_EGH.o: demo_cgt_EGH.cpp cgt_codegen_EGH.h
	$(CPP) -c demo_cgt_EGH.cpp -o demo_cgt_EGH.o $(CXXFLAGS)

lemon/arg_parser.o: lemon/arg_parser.cc
	$(CPP) -c lemon/arg_parser.cc -o lemon/arg_parser.o $(CXXFLAGS)
