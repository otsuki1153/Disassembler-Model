#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


int main(){
    FILE *arquivo;
    
    uint16_t Fullbyte = 0x0000; 
    uint8_t high_byte;
    uint8_t low_byte;
    uint16_t posicao = 0x200;
    char * comando =  malloc(50 * sizeof(char));

    if(comando == NULL){
        printf("Erro na alocação inicial!\n");
        return 1;
    }
    arquivo = fopen("IBM Logo.ch8", "rb");

    if(arquivo == NULL){
        printf("Erro no arquivo!!");
        return 1;
    } else{
        printf("concluido a leitura");
    }
    printf("\n");
    
    
    while(fread(&Fullbyte, sizeof(uint16_t),1,arquivo) == 1){
        low_byte = (Fullbyte >> 8) & 0xFF;
        high_byte = Fullbyte & 0xFF;

        uint16_t opcode = (high_byte << 8) | low_byte;
        
        uint8_t n1 = (high_byte >> 4) & 0x0F;
        uint8_t n2 =  high_byte  & 0x0F;
        uint8_t n3 = (low_byte >> 4)  & 0x0F;
        uint8_t n4 = low_byte & 0x0F;

        switch (n1){
            case 0x0:
                if(n2 == 0){
                    if(n3 == 0xE){
                        if(n4 == 0){
                            strcpy(comando, "CLS");
                        }else{
                            strcpy(comando, "RET");
                        }
                    } else{
                        strcpy(comando, "JMP");
                    }
                }
                break;
            case 0x1:
                strcpy(comando, "JMP");
                break;
            case 0x2:
                strcpy(comando, "CALL");
                break;
            case 0x3:
                strcpy(comando, "SKIP");
                break;
            case 0x4:
                strcpy(comando, "SKIP");
                break;
            case 0x5:
                strcpy(comando, "SKIP");
                break;
            case 0x6:
                strcpy(comando, "LD");
                break;
            case 0x7:
                strcpy(comando, "ADD");
                break;
            case 0x8:
                strcpy(comando, "LD");
                break;
            case 0x9:
                strcpy(comando, "SKIP");
                break;
            case 0xA:
                strcpy(comando, "LD");
                break;
            case 0xB:
                strcpy(comando, "JMP");
                break;
            case 0xC:
                strcpy(comando, "LD");
                break;
            case 0xD:
                strcpy(comando, "DRAW");
                break;
            case 0xE:
                strcpy(comando, "SKIP");
                break;
            case 0xF:
                if(n3 == 0){
                    if(n4 == 7){
                        strcpy(comando, "LD");
                    } else if(n4 == 0xA){
                        strcpy(comando, "WAIT");
                    }
                } else if (n3 == 1){
                    if(n4 == 5){
                        strcpy(comando, "TIMER");
                    } else if(n4 == 8){
                        strcpy(comando, "TIMER_S");
                    } else{
                        strcpy(comando, "ADD");
                    }
                } else if(n3 == 2){
                    strcpy(comando, "LD");
                } else if(n3 == 3){
                    strcpy(comando, "WRITE");
                } else if(n3 == 5){
                    strcpy(comando, "WRITE");
                } else if(n3 == 6){
                    strcpy(comando, "READ");
                }
                
                break;
            default:
                strcpy(comando, "????");
                break;
        }
        
        // printf("Lido: 0x%04X | Separado: [0x%02X] [0x%02X] | Juntado Correto: 0x%04X -> %X-%X-%X-%X : posicao: %03X\n", Fullbyte, high_byte, low_byte, opcode, n1, n2, n3, n4, posicao);
        printf("\n 0x%04X : %04X -> %s", posicao, opcode, comando);
        posicao += 2;
    }
    fclose(arquivo);
    return 0;
}
