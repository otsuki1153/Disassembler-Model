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

        uint16_t NNN = opcode & 0xFFF;

        switch (n1){
            case 0x0:
                if(n2 == 0x0 && n3 == 0xE && n4 == 0){
                    strcpy(comando, "CLS");
                } else if(n2 == 0x0 && n3 == 0xE && n4 == 0xE){
                    strcpy(comando, "RET");
                } else{
                    // strcpy(comando, "SYS");
                    snprintf(comando, 50, "SYS 0x%03X", NNN);
                }
                break;
            case 0x1:
                // strcpy(comando, "JP");
                snprintf(comando, 50, "JP 0x%03X", NNN);
                break;
            case 0x2:
                // strcpy(comando, "CALL");
                snprintf(comando, 50, "CALL 0x%03X", NNN);
                break;
            case 0x3:
                // strcpy(comando, "SE");
                // pos = posicao;
                // if(n2 == low_byte){
                //     pos += 2;
                // }
                snprintf(comando, 50, "SE V%X, 0x%02X", n2, low_byte);
                break;
            case 0x4:
                // strcpy(comando, "SNE");
                // pos = posicao;
                // if(n2 != low_byte){
                //     pos += 2;
                // }
                snprintf(comando, 50, "SNE V%X, 0x%02X", n2, low_byte);
                break;
            case 0x5:
                if(n4 == 0x0){
                    // strcpy(comando, "SE");
                    // pos = posicao;
                    // if(n2 == n3){
                    //     pos += 2;
                    // }
                    snprintf(comando, 50, "SE V%X, V%X",n2, n3);
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
                    snprintf(comando, 50, "LD V%X, V%X", n2, n3);
                } else if(n4 == 0x1){
                    // strcpy(comando, "OR");
                    snprintf(comando, 50, "OR V%X, V%X", n2, n3);
                } else if(n4 == 0x2){
                    // strcpy(comando, "AND");
                    snprintf(comando, 50, "AND V%X, V%X", n2, n3);
                } else if(n4 == 0x3){
                    // strcpy(comando, "XOR");
                    snprintf(comando, 50, "XOR V%X, V%X", n2, n3);
                } else if(n4 == 0x4){
                    // strcpy(comando, "ADD");
                    snprintf(comando, 50, "ADD V%X, V%X", n2, n3);
                } else if(n4 == 0x5){
                    // strcpy(comando, "SUB");
                    snprintf(comando, 50, "SUB V%X, V%X", n2, n3);
                } else if(n4 == 0x6){
                    // strcpy(comando, "SHR");
                    snprintf(comando, 50, "SHR V%X, V%X", n2, n3);
                } else if(n4 == 0x7){
                    // strcpy(comando, "SUBN");
                    snprintf(comando, 50, "SUBN V%X, V%X", n2, n3);
                } else if(n4 == 0xE){
                    // strcpy(comando, "SHL");
                    snprintf(comando, 50, "SHL V%X, V%X", n2, n3);
                }else{
                    strcpy(comando, "????");
                }
                break;
            case 0x9:
                if(n4 == 0x0){
                    // pos = posicao;
                    // if(n2 != n3){
                    //     pos += 2;
                    // }
                    // strcpy(comando, "SNE");
                    snprintf(comando, 50, "SNE V%X, V%X", n2, n3);
                } else{
                    strcpy(comando, "????");
                }
                break;
            case 0xA:
                // strcpy(comando, "LD");
                snprintf(comando, 50, "LD I, 0x%03X", NNN);
                break;
            case 0xB:
                // strcpy(comando, "JP");
                snprintf(comando, 50, "JP V0, 0x%03X",NNN);
                break;
            case 0xC:
                // strcpy(comando, "LD");
                snprintf(comando, 50, "RND V%X, 0x%02X", n2, low_byte);
                break;
            case 0xD:
                // strcpy(comando, "DRW");
                snprintf(comando, 50, "DRW V%X,V%X,0x%X", n2,n3, n4);
                break;
            case 0xE:
                if(n3 == 0x9 && n4 == 0xE){
                    // strcpy(comando, "SKP");
                    // pos = posicao;
                    snprintf(comando, 50, "SKP V%X",n2);
                } else if(n3 == 0xA && n4 == 0x1){
                    // strcpy(comando, "SKNP");
                    snprintf(comando, 50, "SKNP V%X",n2);
                } else{
                    strcpy(comando, "????");
                }
                break;
            case 0xF:
                if(n3 == 0x0){
                    if(n4 == 0x7){
                        // strcpy(comando, "LD Vx, DT");
                        snprintf(comando, 50, "LD V%X, DT",n2);
                    } else if(n4 == 0xA){
                        // strcpy(comando, "LD Vx, K");
                        snprintf(comando, 50, "LD V%X, K",n2);
                    } else{
                        strcpy(comando, "????");
                    }
                } else if (n3 == 0x1){
                    if(n4 == 0x5){
                        // strcpy(comando, "LD DT, Vx");
                        snprintf(comando, 50, "LD DT, V%X",n2);
                    } else if(n4 == 0x8){
                        // strcpy(comando, "LD ST, Vx");
                        snprintf(comando, 50, "LD ST, V%X",n2);
                    } else if(n4 == 0xE){
                        // strcpy(comando, "ADD I, Vx");
                        snprintf(comando, 50, "ADD I, V%X",n2);
                    } else{
                        strcpy(comando, "????");
                    }
                } else if(n3 == 0x2){
                    if(n4 == 0x9){
                        // strcpy(comando, "LD F, Vx");
                        snprintf(comando, 50, "LD F, V%X",n2);
                    } else{
                        strcpy(comando, "????");
                    }
                } else if(n3 == 0x3){
                    if(n4 == 0x3){
                        // strcpy(comando, "LD B, Vx");
                        snprintf(comando, 50, "LD B, V%X",n2);
                    } else{
                        strcpy(comando, "????");
                    }
                } else if(n3 == 0x5){
                    if(n4 == 0x5){
                        // strcpy(comando, "LD [I], Vx");
                        snprintf(comando, 50, "LD [I], V%X",n2);
                    } else{
                        strcpy(comando, "????");
                    }
                } else if(n3 == 0x6){
                    if(n4 == 0x5){
                        // strcpy(comando, "LD Vx, [I]");
                        snprintf(comando, 50, "LD V%X, [I]",n2);
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
