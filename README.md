# ISS
Instruction Set Simulator

This is part of Computer Architecture Project at Indian Statistical Institute

Number of register : 16
Memory : 32 words
Note : can be changed according to need just change the macros in types.h

Currently Supports on LOAD, STORE and ADD operations
SYNTAX for LOAD :
  ld 'r'destination_register memory_location
  OR
  ld 'r'destination_register #constant

SYNTAX for STORE :
    str destination_memory_location 'r'soure_register
    OR
    ld destination_memory_location #constant

SYNTAX for ADD :
  add op1 op2 dest
  Note : op1 and op2 can be registers constants or memory_location
         dest can be register or memory_location

Note : to specify register prefix 'r' to the register number
       to specify constant prefix '#' to the integer value
       to specify memory_location just write the location (0-31)
