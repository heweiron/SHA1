/*final project of ICS 212
 * the Secure Hash Algorithm
 * @author Weirong He
 * @since 3/18/2019
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#define MAX_SIZE 1048576


unsigned int readFile(unsigned char[]);
unsigned int calculateBlocks(unsigned int);
void convertCharArrayToIntArray(unsigned char[], unsigned int[], unsigned int);
void addBitCountToLastBlock(unsigned int[], unsigned int, unsigned int);
void computeMessageDigest(unsigned int[],unsigned int);
void printbitsofchar(unsigned char);
unsigned int S(unsigned int, unsigned int);
unsigned int f(unsigned int, unsigned int, unsigned int, unsigned int);
unsigned int K(unsigned int);
unsigned int W(unsigned int, unsigned int[], int);


/* main method
 * SHA1 program
 */
int main(void) {
    unsigned int numOfChar = 0;
    unsigned char buffer[MAX_SIZE] = {'\0'};
    unsigned int n = 0; //blockCount
    unsigned int message[MAX_SIZE];
    numOfChar = readFile(buffer);
    n = calculateBlocks(numOfChar);
    convertCharArrayToIntArray(buffer, message, numOfChar);
    addBitCountToLastBlock(message, numOfChar, n);
    computeMessageDigest(message, n);



    return 0;
}
/*function #1
 * read file from user input or a file
 * store in an char array
 * add 1 at the end of array
 */
unsigned int readFile(unsigned char buffer[]) {

    char input = 'a';
    unsigned int i = 0;
    input = getchar();
    while(input != EOF) {

        buffer[i] = input;
        i++;
        if(i >= MAX_SIZE) {
            printf("ERROR: Input file is too big for the program!");
            exit(1);
        }
        input = getchar();
    }
    buffer[i] = 0x80;;
    return i;
}

/*function #2
 * calculate the number of blocks
 */
unsigned int calculateBlocks(unsigned int sizeOfFileBytes) {
    unsigned int blockCount = 0;
    blockCount = (((8 * sizeOfFileBytes) + 1) / 512) + 1;
    if((((8 * sizeOfFileBytes) + 1) % 512) > (512 - 64)) {
        blockCount = blockCount + 1;
    }
    return blockCount;
}

/*function #3
 * convert char array to int array
 */
void convertCharArrayToIntArray(unsigned char buffer[], unsigned int message[], unsigned int sizeOfFileBytes) {
    unsigned int n = sizeOfFileBytes;
    int i = 3;
    int t = 0;
    int index = (int)ceil((double)(sizeOfFileBytes+1)/4);

    for(n = (sizeOfFileBytes - ((sizeOfFileBytes+1) % 4))+1; n < sizeOfFileBytes+1; n++) {
        message[index - 1] = message[index - 1] | (buffer[n] << (i * 8));
        i--;
        t = 1;

        //printbitsofchar(buffer[n]);
    }
    i = 0;
    for(n = (sizeOfFileBytes - ((sizeOfFileBytes+1) %4)); n >= 0; n--) {
            
        
        //printbitsofchar(buffer[n]);
        if(t == 1){
            message[index-2] = message[index-2] | (buffer[n] << (i * 8));
        }else{
        
            message[index-1] = message[index-1] | (buffer[n] << (i * 8));
        }
        i++;
        //printf("t = %d\n",t);
        if(i == 4) {
            index--;
            if(t==1){
                if(index < 2) {
                    return;
                }
            }else{
                if(index < 1) {
                    break;
                }
            }
            i = 0;
        }
    }




    return;
}


//DEBUG print bits
void printbitsofchar(unsigned char c) {
    int i = 0;
    unsigned int mask = 0x80;
    for(i=7;i>=0;i--) {
        if(0 != (c & mask)) {
            printf("1");
        }else{
            printf("0");
        }
        mask = mask >>1;
    }
    printf(" = 0x%.2X = '%c' \n",c,c);
}

/*function #4
 * calculate how many 0 to add
 * add 0 to array
 * add length to array make it full
 */
void addBitCountToLastBlock(unsigned int message[], unsigned int sizeOfFileInBytes, unsigned int blockCount) {
    int sizeOfTheFileInBits = sizeOfFileInBytes * 8;
    int indexOfLastWord = (int)ceil((double)(sizeOfFileInBytes+1)/4) - 1;
    int indexOfEndOfLastBlock = blockCount * 16 - 1;
    
    int i = 0;
    for(i = indexOfLastWord + 1; i < indexOfEndOfLastBlock; i++) {
        message[i] = 0;
    }
    message[indexOfEndOfLastBlock] = sizeOfTheFileInBits;


    return;
}

/*S function
 * circle left
 * shift num n times
 */
unsigned int S(unsigned int n, unsigned int num) {
    unsigned int result = 0;
    result = (num >> (32 - n)) | (num << n);
    return result;
}

/*f function
 * calculate it with different t
 */
unsigned int f(unsigned int t, unsigned int B, unsigned int C, unsigned int D) {
    unsigned int r = 0;
    if((0 <= t) && (t <= 19)) {
        r = (B & C) | ((~B) & D);
    }else if((20 <= t) && (t <= 39)) {
        r = B ^ C ^ D;
    }else if((40 <= t) && (t <= 59)) {
        r = (B & C) | (B & D) | (C & D);
    }else if((60 <= t) && (t <= 79)) {
        r = B ^ C ^ D;
    }
    return r;
}

/*K function
 * calculate K
 */
unsigned int K(unsigned int t) {
    unsigned int r = 0;
    if((0 <= t) && (t <= 19)) {
        r = 0x5A827999;
    }else if((20 <= t) && (t <= 39)) {
        r = 0x6ED9EBA1;
    }else if((40 <= t) && (t <= 59)) {
        r = 0x8F1BBCDC;   
    }else if((60 <= t) && (t <= 79)) {
        r = 0xCA62C1D6;
    }
    return r;
}

/* W function
 * divide each block into W
 */
unsigned int W(unsigned int t, unsigned int message[], int block) {
    unsigned int r = 0;
    if((t >= 0)&&(t<=15)){
        r = message[16*block+t];
    }else if((t>=16)&&(t<=79)) {
        r = S(1,(W(t-3,message,block)^W(t-8,message,block)^W(t-14,message,block)^W(t-16,message,block)));
        
    }
    return r;



}

/*encrypt file function
 * using W f K S function
 * loop through each blocks
 * print the final Message digest
 */
void computeMessageDigest(unsigned int message[], unsigned int blockCount) {
    unsigned int H0 = 0x67452301;
    unsigned int H1 = 0xEFCDAB89;
    unsigned int H2 = 0x98BADCFE;
    unsigned int H3 = 0x10325476;
    unsigned int H4 = 0xC3D2E1F0;
    int bloop = 0; //loop of blocks
    for(bloop = 0; bloop < blockCount; bloop++){

        unsigned int A = H0;
        unsigned int B = H1;
        unsigned int C = H2;
        unsigned int D = H3;
        unsigned int E = H4;
        unsigned int TEMP = 0;
        unsigned int t = 0;
        //printf("H_0 = %.8x\nH_1 = %.8x\nH_2 = %.8x\nH_3 = %.8x\nH_4 = %.8x\n",H0,H1,H2,H3,H4);
    
        int i =0;
        for(i=0;i<16;i++){
            //printf("W[%d] = %.8x\n",i, W(i,message,bloop));
        }
    
        for(t = 0; t < 80; t++) {
            TEMP = S(5, A) + f(t, B, C, D) + E + W(t,message,bloop) + K(t);
            E = D;
            D = C;
            C = S(30, B);
            B = A;
            A = TEMP;
            //printf("t = %u: %15.8x%15.8x%15.8x%15.8x%15.8x\n",t,A,B,C,D,E );
        }
            H0 = H0 + A;
            H1 = H1 + B;
            H2 = H2 + C;
            H3 = H3 + D;
            H4 = H4 + E;

    }

    printf("Message digest = %.8x %.8x %.8x %.8x %.8x\n",H0,H1,H2,H3,H4);

    return;
}


