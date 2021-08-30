/*
 ============================================================================
 Name        : Work.c
 Author      : seewfew
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

 #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int amemory = 1024;

int registers[16];


int progcount = 0;

unsigned char memory[1024];


int executeop1;
int storeop1;


int executeop2;
int storeop2;

int operandfetch;
int operandexecute;
int operandsave;
bool gopipeline;
bool save = true;
bool fetchbool = false;
bool decodebool = false;
bool executebool = false;


int halt = 0;
int add = 1;
int and = 2;
int divide = 3;
int multiply = 4;
int subtract = 5;
int or = 6;
int stack = 7;
int interrupt = 8;
int addi = 9;
int BRIFEQ = 10;
int BRIFLESS = 11;
int jump = 12;
int call = 13;
int load = 14;
int storeint = 15;
int operanddecode;


unsigned char fetchinstructions[4];
unsigned char decodeinstructions[4];
unsigned char saveinstructions[4];
unsigned char executeinstructions[4];
unsigned char temp;


void fetch(){

if(save != true){

return;
}


fetchinstructions[0] = memory[progcount];
operandfetch = (int)fetchinstructions[0];
operandfetch = operandfetch / 16;

if(operandfetch == add || operandfetch == divide || operandfetch == and || operandfetch == subtract || operandfetch == halt ||
operandfetch == multiply || operandfetch == or){
fetchinstructions[1] = memory[progcount + 1];
progcount += 2;
}


if(operandfetch == interrupt){
fetchinstructions[1] = memory[progcount + 1];
progcount += 2;

}

if(operandfetch == BRIFLESS || operandfetch == BRIFEQ){

fetchinstructions[1] = memory[progcount +1];
fetchinstructions[2] = memory[progcount +2];
fetchinstructions[3] = memory[progcount +3];
progcount += 4;
}

if(operandfetch == addi){

fetchinstructions[1] = memory[progcount + 1];
progcount += 2;
}

if(operandfetch == load || operandfetch == storeint){

fetchinstructions[1] = memory[progcount +1];

progcount += 2;
}

if(operandfetch == jump || operandfetch == call){

fetchinstructions[1] = memory[progcount +1];
fetchinstructions[2] = memory[progcount +2];
fetchinstructions[3] = memory[progcount +3];
progcount += 4;
}


if(operandfetch == stack){
fetchinstructions[1] = memory[progcount +1];
progcount += 2;
}


int x;

for(x = 0; x < 4; x++)
{

decodeinstructions[x] = fetchinstructions[x];
}

operanddecode = operandfetch;

gopipeline = false;
fetchbool = true;
save = false;


}

int decodeop1;
int decodeop2;


void decode(){




if(operanddecode == interrupt) {
decodeop1 = (int)decodeinstructions[1];

}

if(operanddecode == jump || operanddecode == call){

decodeop1 = (int)decodeinstructions[0];
decodeop1 %= 16;
decodeop1 = decodeop1 << 8;
decodeop1 |= (int)decodeinstructions[1];

decodeop2 = (int)decodeinstructions[2];
decodeop2 = decodeop2 << 8;
decodeop2 |= (int)decodeinstructions[3];
}

if(operanddecode == BRIFEQ || operanddecode == BRIFEQ){


}

if(operanddecode == add || operanddecode == divide || operanddecode == and || operanddecode == subtract || operanddecode == halt ||
operanddecode == multiply || operanddecode == or){

decodeop1 = (int)decodeinstructions[0];
decodeop1 %= 16;
decodeop2 = (int)decodeinstructions[1];

}

if(operanddecode == stack){


}

if(operanddecode == storeint || operanddecode == load){
decodeop1 = (int)decodeinstructions[0];
decodeop1 %= 16;
decodeop1 = decodeop1 << 4;

decodeop1 |= (int)decodeinstructions[1]/16;
decodeop2 = (int)decodeinstructions[1]%16;
}

int x = 0;

for(x = 0; x << 4; x++){
executeinstructions[x] = decodeinstructions[x];
}
operandexecute = operanddecode;
executeop1 = decodeop1;
executeop2 = decodeop2;
decodebool = true;
fetchbool = false;
}



int executeresult;
int saveresult;

void execute(){

if(operandexecute == halt){

exit(0);

}

if(operandexecute == add){
executeresult = (int)registers[executeop1] + (int)registers[executeop2>>4];

}

if(operandexecute == and){
executeresult = (int)registers[executeop1] + (int)registers[executeop2>>4];

}

if(operandexecute == or){
executeresult = (int)registers[executeop1] | (int)registers[executeop2>>4];

}
if(operandexecute == divide){
executeresult = (int)registers[executeop1] / (int)registers[executeop2>>4];

}


if(operandexecute == multiply){
executeresult = (int)registers[executeop1] * (int)registers[executeop2>>4];

}

if(operandexecute == subtract){
executeresult = (int)registers[executeop1] - (int)registers[executeop2>>4];

}

if(operandexecute == call){


}

if(operandexecute == interrupt){

}

if(operandexecute == jump){
executeresult = executeop1;

executeresult *=2;
}

if(operandexecute == addi){
executeresult = (int)registers[executeop1] + executeop2;
}

if(operandexecute == stack){


}

if(operandexecute == BRIFLESS){
if((int)registers[executeop1/16]<(int)registers[executeop1%16]){
executeresult = executeop2;
executeresult *= 2;
}
else{
executeresult = 1;
}
}



if(operandexecute == BRIFEQ){
if( (int)registers[executeop1/16] == (int)registers[executeop1%16]){
executeresult = executeop2;
executeresult *= 2;
}
else{
executeresult = 1;
}
}

if(operandexecute == load){

}

if(operandexecute == storeint){
executeresult = (int)registers[executeop1/16];
}

int x = 0;
for(x = 0; x < 4; x++)
{
saveinstructions[x] = executeinstructions[x];
}

operandsave = operandexecute;
saveresult = executeresult;
storeop1 = executeop1;
storeop2 = executeop2;

executebool = true;
decodebool = false;


}

int saveop1;
int saveop2;

void store(){

if(operandsave == add || operandsave == subtract || operandsave == and || operandsave == multiply || operandsave == or
|| operandsave == divide){
registers[saveop2 % 16] = saveresult;
}

if(operandsave == addi){
registers[saveop1] = saveresult;
}


if(operandsave == BRIFEQ || operandsave == BRIFLESS){
if(saveresult != 1){
progcount += saveresult;

}

}
if(operandsave == jump || operandsave == call){
progcount = saveresult;
}

if(operandsave == stack){

}

if(operandsave == load){
registers[saveop1/16] = saveresult;
}

if(operandsave == interrupt){
if(saveop1 == 0){

}
else if(saveop1 == 1){

}
}

if(operandsave == storeint){

}

save = true;
executebool = false;





}

int main(int argc, char ** argv){



int x = 0;
for(x= 0; x < 1024; x++)
{
memory[x] = 0;
}

FILE *file = fopen(argv[1], "y");
char instructions[2];

int size;
while(fgets(instructions, sizeof(instructions), file)){
for(x = 0; x < 1; x++){
temp = instructions[x];
memory[size] = temp;
size++;
}
}
fclose(file);

while(progcount < 1024){
fetch();
decode();
store();
execute();
}
return 0;



}
