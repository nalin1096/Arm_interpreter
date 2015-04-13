/*******************************************************************************
The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of ARM Processor

Developer's Name: Nalin Gupta && Sahar Siddiqui
Developer's Email id: nalin14065@iiitd.ac.in && sahar14091@iiitd.ac.in
Date: 31 March 2015
********************************************************************************/


/* myARMSim.cpp
   Purpose of this file: implementation file for myARMSim
*/



#include "myARMSim.h"
#include <stdlib.h>
#include <stdio.h>

//Register file
static unsigned int R[16];
//flags
static int N,C,V,Z;
//memory
static unsigned char MEM[4000];

//intermediate datapath and control path signals
static unsigned int instruction_word;
static unsigned int operand1;
static unsigned int operand2;

//hex code of instruction
static long long hex;

//instruction set variables
static int cond;
static int format;
static int immediate;
static int opcode;
static int set;
static int Rn;
static int Rd;
static int op2;
static int S;
static int temp;
static int register_id;
static int link;
static int L;
static unsigned int offset;
static unsigned int mem_loc;


//Loop to execute each instruction fetched from Memory
void run_armsim() {
  while(1) {
    fetch();
    decode();
    execute();
    mem();
    write_back();
  }
}

// it is used to set the reset values
//reset all registers and memory content and flags to 0
void reset_proc() {
  int i;
  for (i=0;i<16;i++){
    R[i] = 0;
  }
  for (i=0;i<4000;i++){
    MEM[i] = 0;
  }
  N=C=V=Z=0;
}

//load_program_memory reads the input memory, and populates the instruction memory
void load_program_memory(char *file_name) {
  FILE *fp;
  unsigned int address, instruction;
  fp = fopen(file_name, "r");
  if(fp == NULL) {
    printf("Error opening input mem file\n");
    exit(1);
  }
  while(fscanf(fp, "%x %x", &address, &instruction) != EOF) {
    write_word(MEM, address, instruction);
  }
  fclose(fp);
}

//writes the data memory in "data_out.mem" file
void write_data_memory() {
  FILE *fp;
  unsigned int i;
  fp = fopen("data_out.mem", "w");
  if(fp == NULL) {
    printf("Error opening dataout.mem file for writing\n");
    return;
  }
  
  for(i=0; i < 4000; i = i+4){
    fprintf(fp, "%X %X\n", i, read_word(MEM, i));
  }
  fclose(fp);
}

//should be called when instruction is swi_exit
void swi_exit() {
  write_data_memory();
  exit(0);
}


//reads from the instruction memory and updates the instruction register
void fetch() {
  printf("FETCH:Fetch instruction 0x%X from address 0x%X\n",read_word(MEM, R[15]),R[15]);
}

void dp_decode () {
  if (opcode == 0){  //and
    if (immediate) {
      printf("DECODE: Operation is AND, first operand R%d, Second operand #%d, destination register R%d\n",Rn,op2,Rd);
      printf("DECODE: Read register R%d = %d, op2 = %d\n",Rn,R[Rn],op2);
    }
    else if (!immediate){
      printf("DECODE: Operation is AND, first operand R%d, Second operand R%d, destination register R%d\n",Rn,register_id,Rd);
      printf("DECODE: Read register R%d = %d, R%d = %d\n",Rn,R[Rn],register_id,op2);
    }
  }
  else if (opcode == 1){  //eor
    if (immediate) {
      printf("DECODE: Operation is EOR, first operand R%d, Second operand #%d, destination register R%d\n",Rn,op2,Rd);
      printf("DECODE: Read register R%d = %d, op2 = %d\n",Rn,R[Rn],op2);
    }
    else if (!immediate){
      printf("DECODE: Operation is EOR, first operand R%d, Second operand R%d, destination register R%d\n",Rn,register_id,Rd);
      printf("DECODE: Read register R%d = %d, R%d = %d\n",Rn,R[Rn],register_id,op2);
    }
  }
  else if (opcode == 2) { //sub
    if (immediate) {
      printf("DECODE: Operation is SUB, first operand R%d, Second operand #%d, destination register R%d\n",Rn,op2,Rd);
      printf("DECODE: Read register R%d = %d, op2 = %d\n",Rn,R[Rn],op2);
    }
    else if (!immediate){
      printf("DECODE: Operation is SUB, first operand R%d, Second operand R%d, destination register R%d\n",Rn,register_id,Rd);
      printf("DECODE: Read register R%d = %d, R%d = %d\n",Rn,R[Rn],register_id,op2);
    }
  }
  else if (opcode == 4){  //add
    if (immediate) {
      printf("DECODE: Operation is ADD, first operand R%d, Second operand #%d, destination register R%d\n",Rn,op2,Rd);
      printf("DECODE: Read register R%d = %d, op2 = %d\n",Rn,R[Rn],op2);
    }
    else if (!immediate){
      printf("DECODE: Operation is ADD, first operand R%d, Second operand R%d, destination register R%d\n",Rn,register_id,Rd);
      printf("DECODE: Read register R%d = %d, R%d = %d\n",Rn,R[Rn],register_id,op2);
    }
  }
  else if (opcode == 5){  //adc
    if (immediate) {
      printf("DECODE: Operation is ADC, first operand R%d, Second operand #%d, destination register R%d\n",Rn,op2,Rd);
      printf("DECODE: Read register R%d = %d, op2 = %d\n",Rn,R[Rn],op2);
    }
    else if (!immediate){
      printf("DECODE: Operation is ADC, first operand R%d, Second operand R%d, destination register R%d\n",Rn,register_id,Rd);
      printf("DECODE: Read register R%d = %d, R%d = %d\n",Rn,R[Rn],register_id,op2);
    }
  }
  else if (opcode == 10) {  //cmp
    if (immediate) {
      printf("DECODE: Operation is CMP, first operand R%d, Second operand #%d\n",Rn,op2);
      printf("DECODE: Read register R%d = %d, op2 = %d\n",Rn,R[Rn],op2);
    }
    else if (!immediate){
      printf("DECODE: Operation is CMP, first operand R%d, Second operand R%d\n",Rn,register_id);
      printf("DECODE: Read registers R%d = %d, R%d = %d\n",Rn,R[Rn],register_id,op2);
    }
  }
  else if (opcode == 12){  //orr
    if (immediate) {
      printf("DECODE: Operation is ORR, first operand R%d, Second operand #%d, destination register R%d\n",Rn,op2,Rd);
      printf("DECODE: Read register R%d = %d, op2 = %d\n",Rn,R[Rn],op2);
    }
    else if (!immediate){
      printf("DECODE: Operation is ORR, first operand R%d, Second operand R%d, destination register R%d\n",Rn,register_id,Rd);
      printf("DECODE: Read register R%d = %d, R%d = %d\n",Rn,R[Rn],register_id,op2);
    }
  }
  else if (opcode == 13) {  //mov
    if (immediate) {
      printf("DECODE: Operation is MOV, First operand #%d, destination register R%d\n",op2,Rd);
      printf("DECODE: Read value op2 = %d\n",op2);
    }
    else if (!immediate){
      printf("DECODE: Operation is MOV, First operand R%d, destination register R%d\n",register_id,Rd);
      printf("DECODE: Read register R%d = %d\n",register_id,op2);
    }
  }
  else if (opcode == 13) {  //mvn
    if (immediate) {
      printf("DECODE: Operation is MVN, First operand #%d, destination register R%d\n",op2,Rd);
      printf("DECODE: Read value op2 = %d\n",op2);
    }
    else if (!immediate){
      printf("DECODE: Operation is MVN, First operand R%d, destination register R%d\n",register_id,Rd);
      printf("DECODE: Read register R%d = %d\n",register_id,op2);
    }
  }
}

void dt_decode () {
  L = (hex & 0x00100000) >> 20;
  if(L == 0){ //str
    if (!immediate){
      offset = (hex & 0x00000FFF);
      printf("DECODE: Operation is STR, First operand R%d, offset #%d, destination register R%d\n",Rn,offset,Rd);
      printf("DECODE: Read register R%d = %d, offset = %d\n",Rn,R[Rn],offset);
    }
    else if (immediate){
      offset = (hex & 0x00000FFF);
      printf("DECODE: Operation is STR, First operand R%d, offset #%d, destination register R%d\n",Rn,R[offset],Rd);
      printf("DECODE: Read register R%d = %d, offset = %d\n",Rn,R[Rn],R[offset]);
    }   
  }
  else if(L == 1){  //ldr
    if (!immediate){
      offset = (hex & 0x00000FFF);
      printf("DECODE: Operation is LDR, First operand R%d, offset #%d, destination register R%d\n",Rn,offset,Rd);
      printf("DECODE: Read register R%d = %d, offset = %d\n",Rn,R[Rn],offset);
    }
    else if (immediate){
      offset = (hex & 0x00000FFF);
      printf("DECODE: Operation is LDR, First operand R%d, offset #%d, destination register R%d\n",Rn,R[offset],Rd);
      printf("DECODE: Read register R%d = %d, offset = %d\n",Rn,R[Rn],R[offset]);
    }      
  }
}

void branch_decode () {
  if (!link) {
    printf("DECODE: Operation is BRANCH\n");
    if (cond == 0)
      printf("DECODE: BEQ\n");
    else if (cond == 1)
      printf("DECODE: BNE\n");
    else if (cond == 10)
      printf("DECODE: BGE\n");
    else if (cond == 11)
      printf("DECODE: BLT\n");
    else if (cond == 12)
      printf("DECODE: BGT\n");
    else if (cond == 13)
      printf("DECODE: BLE\n");
    else if (cond==14)
      printf("DECODE: B\n");
  }
}

//reads the instruction register, reads operand1, operand2 from register file, decides the operation to be performed in execute stage
void decode() {
  hex =read_word(MEM,R[15]);
  cond=(hex & 0xF0000000)>>28;
  format=(hex & 0x0C000000)>>26;
  immediate=(hex & 0x02000000)>>25;
  opcode=(hex & 0x01E00000)>>21;
  S = (hex & 0x00080000)>>20;
  Rn=(hex & 0x000F0000)>>16;
  Rd=(hex & 0x0000F000)>>12;
  if(format==0) //dp
  {
    if (immediate) {
      op2 = (hex & 0x000000FF);
      static int shift;
      shift = (hex & 0x00000F00)>>8;
      op2 <<= shift;
    }
    else if (!immediate){
      register_id = (hex & 0x0000000F);
      op2 = R[register_id];
    }
    dp_decode ();
  }
  else if (format == 1){  //dt
    dt_decode();
  }

  else if(format == 2)  //branch
  {
    link = (hex & 0x01000000) >> 24;
    branch_decode();
  }

  else if (format == 3){  //exit
      printf("Exit\n\n");
      swi_exit();
  }
}

void updateFlag(){
  if(temp==0){
    Z=1;
  }
  else if(temp<0){
    Z=0;
    N=1;
  }
  else{
    Z=0;
    N=0;
  }
}

void dp_execute(){
  if (opcode == 0){  //AND
    temp = R[Rn] & op2;
    printf("EXECUTE: AND %d and %d\n",R[Rn],op2);
  }
  else if (opcode == 1){  //EOR
    temp = R[Rn] ^ op2;
    printf("EXECUTE: EOR %d and %d\n",R[Rn],op2);
  }
  else if (opcode == 2){ //SUB
    temp = R[Rn] - op2;
    printf("EXECUTE: SUB %d and %d\n",R[Rn],op2);
  }
  else if (opcode == 4){  //ADD
    temp = R[Rn] + op2;
    printf("EXECUTE: ADD %d and %d\n",R[Rn],op2);
  }
  else if (opcode == 5){  //ADC
    temp = R[Rn] + op2 + C;
    printf("EXECUTE: ADC %d and %d\n",R[Rn],op2);
  }
  else if (opcode == 10){ //CMP
    temp = R[Rn] - op2;
    updateFlag();
    printf("EXECUTE: CMP %d and %d and put Z=%d\n",R[Rn],op2,Z);
  }
  else if (opcode == 12){  //ORR
    temp = R[Rn] | op2;
    printf("EXECUTE: ORR %d and %d\n",R[Rn],op2);
  }
  else if (opcode == 13){ //MOVE
    temp = op2;
    if(!immediate)
      printf("EXECUTE: MOVE value of R%d in R%d\n",register_id,Rd);
    else if(immediate)
      printf("EXECUTE: MOVE %d in R%d\n",op2,Rd);
  }
  else if (opcode == 15){  //MVN
    temp = ~(op2);
    if(!immediate)
      printf("EXECUTE: MVN value of R%d in R%d\n",register_id,Rd);
    else if(immediate)
      printf("EXECUTE: MVN %d in R%d\n",op2,Rd);
  }
}

void dt_execute () {
  offset = (hex & 0x00000FFF);
  if (L==0) {
    if (!immediate){
      mem_loc = R[Rn] + offset;
      printf("EXECUTE: Add offset %d to %d to get memory location\n",offset,R[Rn]);
    }
    else if (immediate) {
      mem_loc = R[Rn] + R[offset];
      printf("EXECUTE: Add offset %d to %d to get memory location\n",R[offset],R[Rn]);
    }
  }
  else if (L==1) {
    if (!immediate){
      mem_loc = R[Rn] + offset;
      printf("EXECUTE: Add offset %d to %d to get memory location\n",offset,R[Rn]);
    }
    else if (immediate){
      mem_loc = R[Rn] + R[offset];
      printf("EXECUTE: Add offset %d to %d to get memory location\n",R[offset],R[Rn]);
    }
  }
}

void branch_execute(){
  long long hex;
  hex =read_word(MEM,R[15]);
  offset = (hex & 0x00FFFFFF);
  int signedBit = (offset & 0x00800000) >> 23;
  if(signedBit){
    offset += 0xFF000000;
  }
  offset = (offset << 2);
  if(cond == 0 && Z == 1){
    R[15] -= 4;
    R[15] += (int)offset + 8;
    printf("EXECUTE: R[15] gets the memory location 0x%X\n",read_word(MEM,R[15]+4));
  }
  else if(cond == 1 && Z == 0){
    R[15] -= 4;
    R[15] += (int)offset + 8;
    printf("EXECUTE: R[15] gets the memory location 0x%X\n",read_word(MEM,R[15]+4));
  }
  else if(cond == 10 && N == V){
    R[15] -= 4;
    R[15] += (int)offset + 8;
    printf("EXECUTE: R[15] gets the memory location 0x%X\n",read_word(MEM,R[15]+4));
  }
  else if(cond == 11 && N != V){
    R[15] -= 4;
    R[15] += (int)offset + 8;
    printf("EXECUTE: R[15] gets the memory location 0x%X\n",read_word(MEM,R[15]+4));
  }
  else if(cond == 12 && !Z && (N == V)){
    R[15] -= 4;
    R[15] += (int)offset + 8;
    printf("EXECUTE: R[15] gets the memory location 0x%X\n",read_word(MEM,R[15]+4));
  }
  else if(cond == 13 && Z || (N != V)){
    R[15] -= 4;
    R[15] += (int)offset + 8;
    printf("EXECUTE: R[15] gets the memory location 0x%X\n",read_word(MEM,R[15]+4));
  }
  else if(cond == 14){
    R[15] -= 4;
    R[15] += (int)offset + 8;
    printf("EXECUTE: R[15] gets the memory location 0x%X\n",read_word(MEM,R[15]+4));
  }
  else 
    printf("EXECUTE: No execute operation\n");
}

//executes the ALU operation based on ALUop
void execute() {

  if (format == 0)
    dp_execute();
  else if (format == 1)
    dt_execute();
  else if (format == 2)
    branch_execute();
}

//perform the memory operation
void mem() {
  if (format == 0 || format == 2) {
    printf("MEMORY: No memory operation\n");
  }
  if (format == 1) {
    if (L==0) {
      mem_write_word (MEM,mem_loc,R[Rd]);
      printf("MEMORY: Store %d in Heap memory\n",R[Rd]);
    }
    else if (L==1){
      temp = read_word(MEM,mem_loc);
      printf("MEMORY: Load %d from Heap memory\n",temp);
    }
  }
}

void dp_writeback () {
  if (opcode != 10){
    R[Rd] = temp;
    printf("WRITEBACK: Write %d in R%d\n",temp,Rd);
  }
  else 
    printf("WRITEBACK: No writeback operation\n");
}

void dt_writeback() {
  if (L==1){
    R[Rd] = temp;
    printf("WRITEBACK: Write %d in R%d\n",R[Rd],Rd);
  }
  else if (L == 0){
    printf("WRITEBACK: No writeback operation\n");
  }
}

//writes the results back to register file
void write_back() {
  if (format==0)
    dp_writeback();
  else if (format == 1)
    dt_writeback();
  else if (format==2) 
    printf("WRITEBACK: No writeback operation\n");
  R[15]+=4;
  printf("\n");
}

int read_word(char *mem, unsigned int address) {
  int *data;
  data =  (int*) (mem + address);
  return *data;
}

void write_word(char *mem, unsigned int address, unsigned int data) {
  int *data_p;
  data_p = (int*) (mem + address + 2000);
  *data_p = data;
}


void mem_write_word(char *mem, unsigned int address, unsigned int data) {
  int *data_p;
  data_p = (int*) (mem + address);
  *data_p = data;
}
