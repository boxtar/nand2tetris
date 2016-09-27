
/*
PSEUDO CODE
-----------
Assumptions:
int = word = 16 bit
bool = 1 bit
*/

// Inputs
int instruction // 16 bit instruction for execution
int inM // M value input ( M = 16 bit value at RAM[A] )
bool reset // 1 bit reset pin

// Outputs
int outM // M value output
int addressM // M address
bool writeM // indicates if we are writing to mem loc. at A ( M = RAM[A] )
int pc // address of next instruction

// AND-gate
// Set writeM if this is a C-instruction and
// 4th bit of instruction is set (which indicates destination = M)
writeM = ( instruction[15] & instruction[3] );

// MUX16-gate
// if this is an A-instruction then we're assigning a value to the A register
// else we require the previous output of the ALU to perform a C-instruction
int newA = instruction[15] ?  ALUOut : instruction

// AND-gate within OR-gate
// If this is an A-instruction then we need to load the instruction into Register A
// OR if this is a C-instruction AND user specified to load into register A (instruction[5])
int storeToRegisterA = ( ! instruction[15] | ( instruction[5] & instruction[15] ) );

