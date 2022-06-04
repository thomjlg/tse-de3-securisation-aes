#include  "fonctions.h"
#include "stdint.h"
#include <iostream>
using namespace std;

void RotWord(int matrice[4]){
    int temp;
    temp = matrice[0];
    matrice[0] = matrice[1];
    matrice[1] = matrice[2];
    matrice[2] = matrice[3];
    matrice[3] = temp;
}

void SubWord(int matrice[4], int sbox[256]){
    int i;
    for (i = 0; i < 4; i++){
        matrice[i] = sbox[matrice[i]];
    }
}

void KeyExpansion(int round, int expendedKey[4][40], int matrice[4][4], int sbox[256], int rconbox){
    int p, key[4], temp1[4], temp3[4], temp5[4];
    
    int firstCol = round * 4 - 4;
    int secondCol = round * 4 - 3;
    int thirdCol = round * 4 - 2;
    int lastCol = round * 4 - 1;
    
    int previousFirstCol = round * 4 - 8;
    int previousSecondCol = round * 4 - 7;
    int previousThirdCol = round * 4 - 6;
    int previousLastCol = round * 4 - 5;
    
    for(p=0; p<4; p++){
        key[p] = expendedKey[p][previousLastCol];
        temp1[p] = key[p];
    }
    
    RotWord(temp1);
    SubWord(temp1, sbox);
    int k;
    for(k=0; k<4; k++){
        temp3[k] = temp1[k] ^ expendedKey[k][previousFirstCol];
    }
//    temp4 = Rcon(round, rconbox);
    int temp4[4] = {rconbox, 0x00, 0x00, 0x00};
    
    int l;
    for(l=0; l<4; l++){
        temp5[k] = temp3[k] ^ temp4[k];
    }
    
    int z;
    for(z=0; z<4; z++){
        expendedKey[z][firstCol] = temp5[z];
    }
    int y;
    for(y=0; y<4; y++){
        expendedKey[y][secondCol] = expendedKey[y][firstCol] ^ expendedKey[y][previousSecondCol];
    }
    int x;
    for(x=0; x<4; x++){
        expendedKey[x][thirdCol] = expendedKey[x][secondCol] ^ expendedKey[x][previousThirdCol];
    }
    int u;
    for(u=0; u<4; u++){
        expendedKey[u][lastCol] = expendedKey[u][thirdCol] ^ expendedKey[u][previousLastCol];
    }

    int row, col;
    for(row = 0; row < 4; row++){
        for(col = 1; col < 4; col++){
            matrice[row][col] = expendedKey[row][round * 4 - (col + 1)];
        }
    }
}

void SubBytes(int matrice[4][4], int sbox[256]){
    int i, j;
  
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            matrice[i][j] = sbox[matrice[i][j]];
        }
    }
    cout << "1. remplacement de la matrice par les valeurs de la sbox" << endl;
}


void ShiftRows(int matrice[4][4]){

    int temp;
    
    //    rotate 2nd row
    temp = matrice[1][1];
    matrice[1][1] = matrice[1][2];
    matrice[1][2] = matrice[1][3];
    matrice[1][3] = matrice[1][0];
    matrice[1][0] = temp;
    
    //    rotate 3rd row
    temp = matrice[2][2];
    matrice[2][1] = matrice[2][3];
    matrice[2][2] = matrice[2][0];
    matrice[2][3] = matrice[2][1];
    matrice[2][0] = temp;
    
    //    rotate 4th row
    temp = matrice[3][3];
    matrice[3][1] = matrice[3][0];
    matrice[3][2] = matrice[3][1];
    matrice[3][3] = matrice[3][2];
    matrice[3][0] = temp;
    
    cout << "2. decalage des valeurs des lignes 2, 3, 4" << endl;
}

void MixColumns(int matrice[4]){
    int a[4]; // copie de matrice[4]
    int b[4]; // chaque element de a *2
    int c;
    int h;

    for (c = 0; c < 4; c++) {
        a[c] = matrice[c];
        /* h is 0xff if the high bit of matrice[c] is set, 0 otherwise */
        h = (matrice[c] >> 7) & 1; /* arithmetic right shift, thus shifting in either zeros or ones */
        b[c] = matrice[c] << 1; /* implicitly removes high bit because b[c] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line */
        b[c] ^= h * 0x1B; /* Rijndael's Galois field */
    }
    matrice[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
    matrice[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
    matrice[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
    matrice[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
    
    cout << "3. modification des valeurs de la matrice par produit polynomial (en 4 étapes : 1 par ligne de la matrice)" << endl;
}

void AddRoundKey(int matrice[4][4], int clef[4][4]){
    int i, j;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            matrice[i][j] = matrice[i][j] ^ clef[i][j];
        }
    }
    cout << "4. addition bit a bit entre les valeurs et la clef" << endl;
}

void InvAddRoundKey(int matrice[4][4], int clef[4][4]){
    int i, j;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            matrice[i][j] = matrice[i][j] ^ clef[i][j];
        }
    }
    cout << "1. addition bit a bit entre les valeurs et la clef" << endl;
}

void InvMixColumns(int matrice[4]){
    int a[4]; // copie de matrice[4]
    int b[4]; // chaque element de a *2
    int c;
    int h;

    for (c = 0; c < 4; c++) {
        a[c] = matrice[c];
        /* h is 0xff if the high bit of matrice[c] is set, 0 otherwise */
        h = (matrice[c] >> 7) & 1; /* arithmetic right shift, thus shifting in either zeros or ones */
        b[c] = matrice[c] << 1; /* implicitly removes high bit because b[c] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line */
        b[c] ^= h * 0x1B; /* Rijndael's Galois field */
    }
//    matrice[0] = 14 * a[0] + 9 * a[3] + 13 * a[2] + 11 * a[1];
//    matrice[1] = 14 * a[1] + 9 * a[0] + 13 * a[3] + 11 * a[2];
//    matrice[2] = 14 * a[2] + 9 * a[1] + 13 * a[0] + 11 * a[3];
//    matrice[3] = 14 * a[3] + 9 * a[2] + 13 * a[1] + 11 * a[0];
    
    matrice[0] = b[0] ^ b[0] ^ b[0] ^ b[0] ^ b[0] ^ b[0] ^ b[0] ^ b[3] ^ b[3] ^ b[3] ^ b[3] ^ a[3] + b[2] ^ b[2] ^ b[2] ^ b[2] ^ b[2] ^ b[2] ^ a[2] ^ b[1] ^ b[1] ^ b[1] ^ b[1] ^ b[1] ^ a[1];
    matrice[1] = b[1] ^ b[1] ^ b[1] ^ b[1] ^ b[1] ^ b[1] ^ b[1] ^ b[0] ^ b[0] ^ b[0] ^ b[0] ^ a[0] + b[3] ^ b[3] ^ b[3] ^ b[3] ^ b[3] ^ b[3] ^ a[3] ^ b[2] ^ b[2] ^ b[2] ^ b[2] ^ b[2] ^ a[2];
    matrice[2] = b[2] ^ b[2] ^ b[2] ^ b[2] ^ b[2] ^ b[2] ^ b[2] ^ b[1] ^ b[1] ^ b[1] ^ b[1] ^ a[1] + b[0] ^ b[0] ^ b[0] ^ b[0] ^ b[0] ^ b[0] ^ a[0] ^ b[3] ^ b[3] ^ b[3] ^ b[3] ^ b[3] ^ a[3];
    matrice[3] = b[3] ^ b[3] ^ b[3] ^ b[3] ^ b[3] ^ b[3] ^ b[3] ^ b[2] ^ b[2] ^ b[2] ^ b[2] ^ a[2] + b[1] ^ b[1] ^ b[1] ^ b[1] ^ b[1] ^ b[1] ^ a[1] ^ b[0] ^ b[0] ^ b[0] ^ b[0] ^ b[0] ^ a[0];
    
    cout << "2. modification des valeurs de la matrice par produit polynomial (en 4 étapes : 1 par ligne de la matrice)" << endl;
}

void InvShiftRows(int matrice[4][4]){

    int temp; 
    
    //    rotate 2nd row
    temp = matrice[1][0];
    matrice[1][0] = matrice[1][3];
    matrice[1][2] = matrice[1][1];
    matrice[1][3] = matrice[1][2];
    matrice[1][1] = temp;
    
    //    rotate 3rd row
    temp = matrice[2][0];
    matrice[2][0] = matrice[2][2];
    matrice[2][1] = matrice[2][3];
    matrice[2][3] = matrice[2][1];
    matrice[2][2] = temp;
    
    //    rotate 4th row
    temp = matrice[3][0];
    matrice[3][0] = matrice[3][1];
    matrice[3][1] = matrice[3][2];
    matrice[3][2] = matrice[3][3];
    matrice[3][3] = temp;
    
    cout << "3. decalage des valeurs des lignes 2, 3, 4" << endl;
}

void InvSubBytes(int matrice[4][4] , int invsbox[256]){
    int i, j;
  
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            matrice[i][j] = invsbox[matrice[i][j]];
        }
    }
    cout << "4. remplacement de la matrice par les valeurs de la sbox" << endl;
}
