# EGH filter implementation
The source code of the paper S. Z. Kiss, E. Hosszu, J. Tapolcai, L. Ronyai, and O. Rottenstreich, "*Bloom Filter with a False Positive Free Zone*", Infocom 2018

## Install

To compile the c++ code type (Linux, Mac):
```
make
```
If the compilation is succesfull there are two executives `./generate_egh` and `./decode_egh`.

### Generate EGH filter (`generate_egh`)

The `./generate_egh` generates an EGH filter for a given *n* and *d* parameters of the  False Positive Free Zone. For example type

For example, we can generate the construction shown in Table I.

```
$ ./generate_egh  -n 48 -d 2 -codes
EGH filter constructions
[1]  S. Z. Kiss, E. Hosszu, J. Tapolcai, L. Ronyai, and O. Rottenstreich, Bloom Filter with a False Positive Free Zone

 [cgt_codegen_EGH.h:149] Generating Eppstein Goodrich Hirschberg 2-disjunct codes with n=48
  [cgt_codegen_EGH.h:105] The primes are 2, 3, 5, 7, 11, 
  [cgt_codegen_EGH.h:158] The primes multiplied together is 2310, while their sum is 28
 [demo_cgt_EGH.cpp:104] runtime was 0 sec
 [demo_cgt_EGH.cpp:105] code length 28
 [demo_cgt_EGH.cpp:107] bitnum m=28 size of universe n=48
bit 1: 101010101010101010101010101010101010101010101010
bit 2: 010101010101010101010101010101010101010101010101
bit 3: 100100100100100100100100100100100100100100100100
bit 4: 010010010010010010010010010010010010010010010010
bit 5: 001001001001001001001001001001001001001001001001
bit 6: 100001000010000100001000010000100001000010000100
bit 7: 010000100001000010000100001000010000100001000010
bit 8: 001000010000100001000010000100001000010000100001
bit 9: 000100001000010000100001000010000100001000010000
bit10: 000010000100001000010000100001000010000100001000
bit11: 100000010000001000000100000010000001000000100000
bit12: 010000001000000100000010000001000000100000010000
bit13: 001000000100000010000001000000100000010000001000
bit14: 000100000010000001000000100000010000001000000100
bit15: 000010000001000000100000010000001000000100000010
bit16: 000001000000100000010000001000000100000010000001
bit17: 000000100000010000001000000100000010000001000000
bit18: 100000000001000000000010000000000100000000001000
bit19: 010000000000100000000001000000000010000000000100
bit20: 001000000000010000000000100000000001000000000010
bit21: 000100000000001000000000010000000000100000000001
bit22: 000010000000000100000000001000000000010000000000
bit23: 000001000000000010000000000100000000001000000000
bit24: 000000100000000001000000000010000000000100000000
bit25: 000000010000000000100000000001000000000010000000
bit26: 000000001000000000010000000000100000000001000000
bit27: 000000000100000000001000000000010000000000100000
bit28: 000000000010000000000100000000001000000000010000
```
#### Manual
```
$ ./generate_egh  -help
EGH filter constructions
[1]  S. Z. Kiss, E. Hosszu, J. Tapolcai, L. Ronyai and O. Rottenstreich, Bloom Filter with a False Positive Free Zone
Usage:
  ./generate_egh [--help|-h|-help] [-codes] [-d int] [-fig_fixd] [-fig_fixn]
     [-file_prefix str] [-fixm int] [-log int] [-n int] [-verify]
Where:
  --help|-h|-help
     Print a short help message
  -codes
     Print the items in the EGH filter as bitvectors
  -d int
     The number of items in the EGH filter
  -fig_fixd
     Generate a chart with fixed number of failures (Fig 2)
  -fig_fixn
     Generate a chart with fixed number of item
  -file_prefix str
     Prefix of the file the results are written
  -fixm int
     Search n and d for a maximum code length
  -log int
     The logging level: 0 - results and errors only, 1 - min, 6 - max info
  -n int
     The size of the universe
  -verify
     Verify codes
```



### Accurate listing elements in EGH filter (`decode_egh`)

The `./decode_egh` is for accurate listing the elements of advanced EGH filter (with counters) for a given *n* and *d* parameters of the False Positive Free Zone. 
```
$ ./decode_egh -n 1000 -d 2
Demo of the EGH filter decoding algorithm
[1]  S. Z. Kiss, E. Hosszu, J. Tapolcai, L. Ronyai, and O. Rottenstreich, Bloom Filter with a False Positive Free Zone

 [cgt_codegen_EGH.h:149] Generating Eppstein Goodrich Hirschberg 2-disjunct codes with n=1000
  [cgt_codegen_EGH.h:105] The primes are 2, 3, 5, 7, 11, 13, 17, 19, 
  [cgt_codegen_EGH.h:158] The primes multiplied together is 9.69969e+06, while their sum is 77
 [cgt_codegen_EGH_decode.h:686] We insert the following 2 random items into EGH filter: 807 249 
  [cgt_codegen_EGH_decode.h:690] Decoding started
  [cgt_codegen_EGH_decode.h:708] Generate elementary symetric polynomials
  [cgt_codegen_EGH_decode.h:709] sigma_1=1056
  [cgt_codegen_EGH_decode.h:738] sigma_2=200943
  [cgt_codegen_EGH_decode.h:580] Find root of Sturm sequence f(z)= z^2 -1056z +200943
 [cgt_codegen_EGH_decode.h:644] 2 distinct real roots are found: 
249.00000024733678469895337425908212224725336045439004212
806.99999542435451012088908952827375043953266527283636455
```

#### Manual
```
$ ./decode_egh -help
Demo of the EGH filter decoding algorithm
[1]  S. Z. Kiss, E. Hosszu, J. Tapolcai, L. Ronyai, and O. Rottenstreich, Bloom Filter with a False Positive Free Zone
Usage:
  ./decode_egh [--help|-h|-help] [-d int] [-fig4] [-log int] [-n int]
Where:
  --help|-h|-help
     Print a short help message
  -d int
     The number of items in the EGH filter
  -fig4
     Generate data for Figure 4 of the paper
  -log int
     The logging level: 0 - results and errors only, 1 - min, 6 - max info
  -n int
     The size of the universe
```

#### Possible issues

The precision of storing the coefficients and integers is defined at line 41 of *cgt_codegen_EGH_decode.h*
```
#define         COEFTYPE          ttmath::Big<6,3>
#define         BIGINT          ttmath::Int<7>
```

If the program writes an overflow error, please increase these values. 
