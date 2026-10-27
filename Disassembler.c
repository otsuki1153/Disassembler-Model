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
        free(comando);
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

        uint16_t NNN;

        switch (n1){
            case 0x0:
                if(n2 == 0x0 && n3 == 0xE && n4 == 0){
                    strcpy(comando, "CLS");
                } else if(n2 == 0x0 && n3 == 0xE && n4 == 0xE){
                    strcpy(comando, "RET");
                } else{
                    strcpy(comando, "SYS");
                    NNN = opcode & 0xFFF;
                    snprintf(comando, 50, "JP SYS, 0x%03X", NNN);
                }
                break;
            case 0x1:
                // strcpy(comando, "JP");
                NNN = opcode & 0xFFF;
                snprintf(comando, 50, "JP, 0x%03X", NNN);
                break;
            case 0x2:
                // strcpy(comando, "CALL");
                NNN = opcode & 0xFFF;
                snprintf(comando, 50, "CALL, 0x%03X", NNN);
                break;
            case 0x3:
                // strcpy(comando, "SE");
                int pos = posicao;
                if(n2 == low_byte){
                    pos += 2;
                }
                snprintf(comando, 50, "SE V%X, 0x%02X, 0x%04X", n2, low_byte,pos);
                break;
            case 0x4:
                // strcpy(comando, "SNE");
                int pos = posicao;
                if(n2 != low_byte){
                    pos += 2;
                }
                snprintf(comando, 50, "SNE V%X, 0x%02X, 0x%04X", n2, low_byte, pos);
                break;
            case 0x5:
                if(n4 == 0x0){
                    // strcpy(comando, "SE");
                    int pos = posicao;
                    if(n2 == n3){
                        pos += 2;
                    }
                    snprintf(comando, 50, "SE V%X V%X, 0x%04X",n2, n3, pos);
                } else{
                    strcpy(comando, "????");
                }
                break;
            case 0x6:
                // strcpy(comando, "LD");
                snprintf(comando, 50, "LD V%X, 0x%02X", n2, low_byte);
                break;
            case 0x7:
                // strcpy(comando, "ADD");
                snprintf(comando, 50, "ADD V%X, 0x%02X", n2, low_byte);
                break;
            case 0x8:
                if(n4 == 0x0){
                    // strcpy(comando, "LD");
                    snprintf(comando, 50, "LD V%X, 0x%X", n3, n2);
                } else if(n4 == 0x1){
                    strcpy(comando, "OR");
                } else if(n4 == 0x2){
                    strcpy(comando, "AND");
                } else if(n4 == 0x3){
                    strcpy(comando, "XOR");
                } else if(n4 == 0x4){
                    strcpy(comando, "ADD");
                } else if(n4 == 0x5){
                    strcpy(comando, "SUB");
                } else if(n4 == 0x6){
                    strcpy(comando, "SHR");
                } else if(n4 == 0x7){
                    strcpy(comando, "SUBN");
                } else if(n4 == 0xE){
                    strcpy(comando, "SHL");
                }else{
                    strcpy(comando, "????");
                }
                break;
            case 0x9:
                if(n4 == 0x0){
                    strcpy(comando, "SNE");
                } else{
                    strcpy(comando, "????");
                }
                break;
            case 0xA:
                // strcpy(comando, "LD");
                NNN = opcode & 0xFFF;
                snprintf(comando, 50, "LD I, 0x%03X", NNN);
                break;
            case 0xB:
                strcpy(comando, "JP");
                break;
            case 0xC:
                strcpy(comando, "LD");
                break;
            case 0xD:
                strcpy(comando, "DRW");
                break;
            case 0xE:
                if(n3 == 0x9 && n4 == 0xE){
                    strcpy(comando, "SKP");
                } else if(n3 == 0xA && n4 == 0x1){
                    strcpy(comando, "SKNP");
                } else{
                    strcpy(comando, "????");
                }
                break;
            case 0xF:
                if(n3 == 0x0){
                    if(n4 == 0x7){
                        strcpy(comando, "LD Vx, DT");
                    } else if(n4 == 0xA){
                        strcpy(comando, "LD Vx, K");
                    } else{
                        strcpy(comando, "????");
                    }
                } else if (n3 == 0x1){
                    if(n4 == 0x5){
                        strcpy(comando, "LD DT, Vx");
                    } else if(n4 == 0x8){
                        strcpy(comando, "LD ST, Vx");
                    } else if(n4 == 0xE){
                        strcpy(comando, "ADD I, Vx");
                    } else{
                        strcpy(comando, "????");
                    }
                } else if(n3 == 0x2){
                    if(n4 == 0x9){
                        strcpy(comando, "LD F, Vx");
                    } else{
                        strcpy(comando, "????");
                    }
                } else if(n3 == 0x3){
                    if(n4 == 0x3){
                        strcpy(comando, "LD B, Vx");
                    } else{
                        strcpy(comando, "????");
                    }
                } else if(n3 == 0x5){
                    if(n4 == 0x5){
                        strcpy(comando, "LD [I], Vx");
                    } else{
                        strcpy(comando, "????");
                    }
                } else if(n3 == 0x6){
                    if(n4 == 0x5){
                        strcpy(comando, "LD Vx, [I]");
                    } else{
                        strcpy(comando, "????");
                    }
                } else{
                    strcpy(comando, "????");
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
    free(comando);
    return 0;
}
