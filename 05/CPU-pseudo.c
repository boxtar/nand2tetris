
/*
 * PSEUDO CODE FOR CPU IMPLEMENTATION
 *
 * Assumptions:
 * int = word = 16 bit
 * bool = 1 bit
 *
 */

// Inputs (3)
int instruction // 16 bit instruction for execution
int in_M // M value input ( M = 16 bit value at RAM[A] )
bool reset // 1 bit reset pin

// Outputs (4)
int out_M // M value output
int address_M // M address
bool write_M // indicates if we are writing to mem loc. at A ( M = RAM[A] )
int pc // address of next instruction
  
// Pin Declaration for some sort of correctness
int ALU_out = FALSE

// AND-gate
// Sets up control bit for Memory Write
// Set writeM if this is a C-instruction and
// 4th bit of instruction is set (which indicates destination = M)
write_M = ( instruction[15] & instruction[3] )

// MUX16-gate
// if this is an A-instruction then we're assigning a value to the A register
// else we require the previous output of the ALU to perform a C-instruction
int in_A = instruction[15] ?  ALU_out : instruction

// OR-gate
// Control bit for A REGISTER
// If this is an A-instruction then we need to load the instruction into Register A
// OR if this is a C-instruction and user specified to load into register A (instruction[5])
// This works - do a quick T.T to prove it
int write_A = ( ! instruction[15] | instruction[5] )

// REGISTER [16 BITS]
// Load value into A REGISTER if load bit is set
// The output emitted is the previous value of the register (t-1)
// So A_out = the value at t-1. The first 15 bits of the output are
// fanned out into address_M which denotes the target RAM address (M)
int out_A = REGISTER( in=in_A, load=write_A, out[0..14]=address_M )
  
// AND-gate
// Control bit for D REGISTER
// Only load D REGISTER if this is a C-instruction and the appropriate dest bit is set
int write_D = ( instruction[15] & instruction[4] )
  
// REGISTER [16 BITS]
// Load value into D REGISTER if load bit is set
int out_D = REGISTER( in=ALU_out, load=write_D )

// MUX16-gate
// 12th bit of instruction (named a) denotes whether to use A or M
int A_or_M = instruction[12] ? in_M : out_A

// Call upon the ALU to do some magic
// out is fanned to ALU_out
out_M = ALU ( x=out_D, y=A_or_M,
                  zx=instruction[11],
                  nx=instruction[10],
                  zy=instruction[9],
                  ny=instruction[8],
                  f=instruction[7],
                  no=instruction[6],
                  zr=zr,
                  ng=ng,
                  out=ALU_out
              )

// Deal with any jump instruction
// Only need to test for the following conditions: JGT, JEQ, JLT
// The combination of these cover all jump conditions
notZero = !zr
notNegative = !ng

JGT1 = ( notZero && notNegative )
JGT = ( JGT1 && instruction[0] )
JEQ = ( zr && instruction[1] )
JLT = ( ng && instruction[3] )

// Or these together to get one output indicating if a jump is required
LD1 = ( JGT | JEQ )
LD2 = ( LD1 | JLT )

// Now make sure that this is a C-instruction before telling PC to load
load_PC = ( LD2 & instruction[15] )

// Invoke Program Counter





