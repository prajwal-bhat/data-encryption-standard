//This implementation of DES takes random number as input for plaintext and for keys also and then decrypts the same.
//All the input and out are in binary system
//Use cc file.c -lm to compile correctly for some of the function 
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

int roundkeymat[16][48];//Round key used in each round of DES

//take randomly generated key and implementation of pbox
void make_keys(){
	int pbox[6][8]={{14,17,11,24,1,5,3,28},
				{15,6,21,10,23,19,12,4},
				{26,8,16,7,27,20,13,2},
				{41,52,31,37,47,55,30,40},
				{51,45,33,48,44,49,39,56},
				{34,53,46,42,50,36,29,32}};
	int i,key[64];
	int newkey[56],leftkey[28],rightkey[28],mergedkey[56],rdkey[48];
	for(i=0;i<64;i++)
		key[i]=rand()%2;	
	int j=0,k,row,col;
	printf("\nKey:\n");
	for(i=0;i<64;i++)
		printf("%d",key[i]);
	for(i=0;i<64;i++){
		if(i==0 ||(i+1)%8!=0){
			newkey[j]=key[i];
			j++;
		}
		else if((i+1)%8==0){
			newkey[j]=key[i+1];
			i++;
			j++;
		}			
	}
	//Dividing the keys into left and right subarrays
	for(i=0;i<28;i++){
		leftkey[i]=newkey[i];
		rightkey[i]=newkey[i+28];
	}
	for(i=0;i<16;i++){
		int temp1=leftkey[0];
		int temp2=rightkey[0];
		for(j=0;j<27;j++){
			leftkey[j]=leftkey[j+1];
			rightkey[j]=rightkey[j+1];
		}
		leftkey[27]=temp1;
		rightkey[27]=temp2;		
		if(i==0||i==1||i==8||i==15){
			temp1=leftkey[0];
			temp2=rightkey[0];
			for(j=0;j<27;j++){
				leftkey[j]=leftkey[j+1];
				rightkey[j]=rightkey[j+1];
			}
			leftkey[27]=temp1;
			rightkey[27]=temp2;	
		}
		for(k=0;k<28;k++){
			mergedkey[k]=leftkey[k];
			mergedkey[k+28]=rightkey[k];
		}
		k=0;		
		//use pbox array to make round key matrix
		for(row=0;row<6;row++){
			for(col=0;col<8;col++){
				rdkey[k]=mergedkey[pbox[row][col]-1];				
				k++;
			}
		}
		for(int n=0;n<48;n++){
			roundkeymat[i][n]=rdkey[n];
		}
	}			
}

//Key expansion using expansion matric 
void expand_keys(int *expanded,int *a){
	int expansion[8][6]={{32,1,2,3,4,5},
						{4,5,6,7,8,9},
						{8,9,10,11,12,13},
						{12,13,14,15,16,17},
						{16,17,18,19,20,21},
						{20,21,22,23,24,25},
						{24,25,26,27,28,29},
						{28,29,30,31,32,1}};
	int i,j,k=0;
	for(i=0;i<8;i++){
		for(j=0;j<6;j++){
		expanded[k++]=a[expansion[i][j]-1];	
		}
	}
}

void devide_array(int *itext,int *larray,int *rarray){
	int i;
	for(i=0;i<32;i++){
		larray[i]=itext[i];
		rarray[i]=itext[i+32];
	}
}

//binary to decimal conversion
int binary_to_decimal(int *n,int size){
	int i,j=0,sum=0;
	for(i=size-1;i>=0;i--){
		sum+=n[i]*pow(2,j++);
	}
	return sum;
}

//decimal to binary conversion
void decimal_to_binary(int n,int *result){
	int i,j=0,arr[4];
	for(i=0;i<4;i++){
		arr[i]=n%2;
		n/=2;
	}
	for(i=3;i>=0;i--){
		result[j++]=arr[i];
	}
}

void s_box_sub_op(int *a,int ch,int *result){
	int j,row,col,temp1[2],temp2[4],element,binary[4];
	int s1[4][16]={
{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}};
int s2[4][16]={
{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
{ 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}};
int s3[4][16]={
{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
{ 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 }};
int s4[4][16]={
{7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
{3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}};
int s5[4][16]={
{2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
{4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}};
int s6[4][16]={
{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
{9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
{4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}};
int s7[4][16]={
{4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
{1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
{6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}};
int s8[4][16]={
{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
{1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
{7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
{2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}};
	temp1[0]=a[0];
	temp1[1]=a[5];
	for(j=1;j<5;j++)
		temp2[j-1]=a[j];
	row=binary_to_decimal(temp1,2);
	col=binary_to_decimal(temp2,4);
	switch(ch){
		case 1:element=s1[row][col];
				break;
		case 2:element=s2[row][col];
				break;
		case 3:element=s3[row][col];
				break;
		case 4:element=s4[row][col];
				break;
		case 5:element=s5[row][col];
				break;
		case 6:element=s6[row][col];
				break;
		case 7:element=s7[row][col];
				break;
		case 8:element=s8[row][col];
				break;
	}	
	decimal_to_binary(element,binary);
	for(j=0;j<4;j++)
		result[j]=binary[j];
}

//s-box implementation function
void s_box_op(int *iparr,int *result){	
	int i,j,k=0,binary[4],a1[6],a2[6],a3[6],a4[6],a5[6],a6[6],a7[6],a8[6];
	for(i=0;i<6;i++){
		a1[i]=iparr[i];
		a2[i]=iparr[i+6];
		a3[i]=iparr[i+12];
		a4[i]=iparr[i+18];
		a5[i]=iparr[i+24];
		a6[i]=iparr[i+30];
		a7[i]=iparr[i+36];
		a8[i]=iparr[i+42];
	}
	for(i=0;i<48;i+=6){
		if(i>=0 && i<6){
			s_box_sub_op(a1,1,binary);
			for(j=0;j<4;j++)
				result[k++]=binary[j];
		}
		else if(i>=6 && i<12){
			s_box_sub_op(a2,2,binary);
			for(j=0;j<4;j++)
				result[k++]=binary[j];
		}
		else if(i>=12 && i<18){
			s_box_sub_op(a3,3,binary);
			for(j=0;j<4;j++)
				result[k++]=binary[j];
		}
		else if(i>=18 && i<24){
			s_box_sub_op(a4,4,binary);
			for(j=0;j<4;j++)
				result[k++]=binary[j];
		}
		else if(i>=24 && i<30){
			s_box_sub_op(a5,5,binary);
			for(j=0;j<4;j++)
				result[k++]=binary[j];
		}
		else if(i>=30 && i<36){
			s_box_sub_op(a6,6,binary);
			for(j=0;j<4;j++)
				result[k++]=binary[j];
		}
		else if(i>=36 && i<42){
			s_box_sub_op(a7,7,binary);
			for(j=0;j<4;j++)
				result[k++]=binary[j];
		}
		else if(i>=42 && i<48){
			s_box_sub_op(a8,8,binary);
			for(j=0;j<4;j++)
				result[k++]=binary[j];
		}
	}

}
//permutation box implementation
void permutation_box(int *input,int *result){
	int parr[4][8] = {  {16,  7, 20, 21,29, 12, 28, 17},
						{1, 15, 23, 26,5, 18, 31, 10},
						{2,  8, 24, 14,32, 27,  3,  9},
						{19, 13, 30,  6, 22, 11,  4, 25}};
	int i,j,k=0;
	for(i=0;i<4;i++){
		for(j=0;j<8;j++){
			result[k++]=input[parr[i][j]-1];
		}
	}	
}

void initial_permutation(int *initialtext,int *pt){
	int ip[8][8]={ {58,50,42,34,26,18,10,2},
					{60,52,44,36,28,20,12,4},
					{62,54,46,38,30,22,14,6},
					{64,56,48,40,32,24,16,8},
					{57,49,41,33,25,17,9,1},
					{59,51,43,35,27,19,11,3},
					{61,53,45,37,29,21,13,5},
					{63,55,47,39,31,23,15,7}};
	int row,col,i=0;
	for(row=0;row<8;row++){
		for(col=0;col<8;col++){
			initialtext[i++]=pt[ip[row][col]-1];
		}
	}	
}

void final_permutation(int *rd,int *result){
	int fp[8][8]={  {40,8,48,16,56,24,64,32},
				{39,07,47,15,55,23,63,31},
				{38,06,46,14,54,22,62,30},
				{37,05,45,13,53,21,61,29},
				{36,04,44,12,52,20,60,28},
				{35,03,43,11,51,19,59,27},
				{34,02,42,10,50,18,58,26},
				{33,01,41,9,49,17,57,25}};
	int row,col,i=0;
	for(row=0;row<8;row++){
		for(col=0;col<8;col++){
			result[i++]=rd[fp[row][col]-1];
		}
	}
}
//xor operation based on the input size
void xor_operation(int *input1,int *input2,int *result,int roundno,int size){
	int i;
	if(size==32){
		for(i=0;i<size;i++){
			result[i]=input1[i]^input2[i];
		}
	}
	else if(size==48){
		for(i=0;i<size;i++){
			result[i]=input1[i]^roundkeymat[roundno][i];
		}
	}	
}

int main(){	
	int i,j,initialtext[64],initialtext1[64],ciphertext[64],mergedtext[64],plaintext[64],expanded[48],xored[48],sarr[32],permutedarr[32],decryptedtext[64];
	int ltext[32],rtext[32];
	srand(time(0));	
	for(i=0;i<64;i++)
		plaintext[i]=rand()%2;	
	printf("PlainText:\n");
	for(i=0;i<64;i++)
		printf("%d",plaintext[i]);
	make_keys();
	//Encryption 
	int temp[32];
	initial_permutation(initialtext,plaintext);
	for(i=0;i<16;i++){
		devide_array(initialtext,ltext,rtext);
		expand_keys(expanded,rtext);
		xor_operation(expanded,ltext,xored,i,48);
		s_box_op(xored,sarr);
		permutation_box(sarr,permutedarr);
		for(j=0;j<32;j++)
			temp[j]=rtext[j];
		xor_operation(permutedarr,ltext,rtext,i,32);
		for(j=0;j<32;j++)
			ltext[j]=temp[j];
		for(j=0;j<32;j++){
			initialtext[j]=ltext[j];
			initialtext[j+32]=rtext[j];
		}
	}	
	for(i=0;i<32;i++){
		int temp;
		temp=initialtext[i];
		initialtext[i]=initialtext[i+32];
		initialtext[i+32]=temp;
	}
	final_permutation(initialtext,ciphertext);
	printf("\nCipherText:\n");
	for(i=0;i<64;i++)
		printf("%d",ciphertext[i]);
	printf("\n");
	//Decryption
	initial_permutation(initialtext1,ciphertext);
	for(i=15;i>=0;i--){
		devide_array(initialtext1,ltext,rtext);
		expand_keys(expanded,rtext);
		xor_operation(expanded,ltext,xored,i,48);
		s_box_op(xored,sarr);
		permutation_box(sarr,permutedarr);
		for(j=0;j<32;j++)
			temp[j]=rtext[j];
		xor_operation(permutedarr,ltext,rtext,i,32);
		for(j=0;j<32;j++)
			ltext[j]=temp[j];
		for(j=0;j<32;j++){
			initialtext1[j]=ltext[j];
			initialtext1[j+32]=rtext[j];
		}
	}
	for(i=0;i<32;i++){
		int temp;
		temp=initialtext1[i];
		initialtext1[i]=initialtext1[i+32];
		initialtext1[i+32]=temp;
	}	
	final_permutation(initialtext1,decryptedtext);
	printf("Decrypted Text:\n");
	for(i=0;i<64;i++)
		printf("%d",decryptedtext[i]);
}