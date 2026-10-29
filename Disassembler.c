#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


int main(){
    FILE *arquivo;
    
    uint8_t high_byte;
    uint8_t low_byte;
    uint8_t bytes[2];
    uint16_t posicao = 0x200;
    char comando[20];
    size_t lidos;
    
    char rawFont[30];
    int op = 10;

    printf("Escolha o arquivo que quer desmontar:\n");
    printf("1. chip8 Logo\n");
    printf("2. IBM Logo\n");
    printf("3. chorax+\n");
    printf("4. flags\n");
    printf("5. quirks\n");
    printf("6. keypad\n");
    printf("7. beep\n");
    printf("8. scrolling\n\n\n");

    printf("Digite sua opção: ");
    scanf("%d", &op);

    while(op < 0 || op > 8){
        printf("Opção inválida! \n\n");
        printf("Escolha o arquivo que quer desmontar:\n");
        printf("1. chip8 Logo\n");
        printf("2. IBM Logo\n");
        printf("3. chorax+\n");
        printf("4. flags\n");
        printf("5. quirks\n");
        printf("6. keypad\n");
        printf("7. beep\n");
        printf("8. scrolling\n\n\n");
        printf("Digite sua opção: ");
        scanf("%d", &op);
    }
    
    switch(op){
        case 1:
            strcpy(rawFont, "chip8Raw/1-chip8-logo.ch8");
            break;
        case 2:
            strcpy(rawFont, "chip8Raw/IBM Logo.ch8");
            break;
        case 3:
            strcpy(rawFont, "chip8Raw/3-chorax+.ch8");
            break;
        case 4:
            strcpy(rawFont, "chip8Raw/4-flags.ch8");
            break;
        case 5:
            strcpy(rawFont, "chip8Raw/5-quirks.ch8");
            break;
        case 6:
            strcpy(rawFont, "chip8Raw/6-keypad.ch8");
            break;
        case 7:
            strcpy(rawFont, "chip8Raw/7-beep.ch8");
            break;
        case 8:
            strcpy(rawFont, "chip8Raw/8-scrolling.ch8");
            break;
        
        default:
            break;
    }

    if(comando == NULL){
        printf("Erro na alocação inicial!\n");
        return 1;
    }


    arquivo = fopen(rawFont, "rb");

    if(arquivo == NULL){
        printf("Erro no arquivo!!");
        return 1;
    } else{
        printf("concluido a leitura");
    }
    printf("\n");
    
    
    while((lidos = fread(bytes, sizeof(uint8_t),2,arquivo)) == 2){
        low_byte = bytes[1];
        high_byte = bytes[0];

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
                    snprintf(comando, 50, "SYS 0x%03X", NNN);
                }
                break;
            case 0x1:
                snprintf(comando, 50, "JP 0x%03X", NNN);
                break;
            case 0x2:
                snprintf(comando, 50, "CALL 0x%03X", NNN);
                break;
            case 0x3:
                snprintf(comando, 50, "SE V%X, 0x%02X", n2, low_byte);
                break;
            case 0x4:
                snprintf(comando, 50, "SNE V%X, 0x%02X", n2, low_byte);
                break;
            case 0x5:
                if(n4 == 0x0){
                    snprintf(comando, 50, "SE V%X, V%X",n2, n3);
                } else{
                    strcpy(comando, "????");
                }
                break;
            case 0x6:
                snprintf(comando, 50, "LD V%X, 0x%02X", n2, low_byte);
                break;
            case 0x7:
                snprintf(comando, 50, "ADD V%X, 0x%02X", n2, low_byte);
                break;
            case 0x8:
                if(n4 == 0x0){
                    snprintf(comando, 50, "LD V%X, V%X", n2, n3);
                } else if(n4 == 0x1){
                    snprintf(comando, 50, "OR V%X, V%X", n2, n3);
                } else if(n4 == 0x2){
                    snprintf(comando, 50, "AND V%X, V%X", n2, n3);
                } else if(n4 == 0x3){
                    snprintf(comando, 50, "XOR V%X, V%X", n2, n3);
                } else if(n4 == 0x4){
                    snprintf(comando, 50, "ADD V%X, V%X", n2, n3);
                } else if(n4 == 0x5){
                    snprintf(comando, 50, "SUB V%X, V%X", n2, n3);
                } else if(n4 == 0x6){
                    snprintf(comando, 50, "SHR V%X, V%X", n2, n3);
                } else if(n4 == 0x7){
                    snprintf(comando, 50, "SUBN V%X, V%X", n2, n3);
                } else if(n4 == 0xE){
                    snprintf(comando, 50, "SHL V%X, V%X", n2, n3);
                }else{
                    strcpy(comando, "????");
                }
                break;
            case 0x9:
                if(n4 == 0x0){
                    snprintf(comando, 50, "SNE V%X, V%X", n2, n3);
                } else{
                    strcpy(comando, "????");
                }
                break;
            case 0xA:
                snprintf(comando, 50, "LD I, 0x%03X", NNN);
                break;
            case 0xB:
                snprintf(comando, 50, "JP V0, 0x%03X",NNN);
                break;
            case 0xC:
                snprintf(comando, 50, "RND V%X, 0x%02X", n2, low_byte);
                break;
            case 0xD:
                snprintf(comando, 50, "DRW V%X, V%X, 0x%X", n2,n3, n4);
                break;
            case 0xE:
                if(n3 == 0x9 && n4 == 0xE){
                    snprintf(comando, 50, "SKP V%X",n2);
                } else if(n3 == 0xA && n4 == 0x1){
                    snprintf(comando, 50, "SKNP V%X",n2);
                } else{
                    strcpy(comando, "????");
                }
                break;
            case 0xF:
                if(n3 == 0x0){
                    if(n4 == 0x7){
                        snprintf(comando, 50, "LD V%X, DT",n2);
                    } else if(n4 == 0xA){
                        snprintf(comando, 50, "LD V%X, K",n2);
                    } else{
                        strcpy(comando, "????");
                    }
                } else if (n3 == 0x1){
                    if(n4 == 0x5){
                        snprintf(comando, 50, "LD DT, V%X",n2);
                    } else if(n4 == 0x8){
                        snprintf(comando, 50, "LD ST, V%X",n2);
                    } else if(n4 == 0xE){
                        snprintf(comando, 50, "ADD I, V%X",n2);
                    } else{
                        strcpy(comando, "????");
                    }
                } else if(n3 == 0x2){
                    if(n4 == 0x9){
                        snprintf(comando, 50, "LD F, V%X",n2);
                    } else{
                        strcpy(comando, "????");
                    }
                } else if(n3 == 0x3){
                    if(n4 == 0x3){
                        snprintf(comando, 50, "LD B, V%X",n2);
                    } else{
                        strcpy(comando, "????");
                    }
                } else if(n3 == 0x5){
                    if(n4 == 0x5){
                        snprintf(comando, 50, "LD [I], V%X",n2);
                    } else{
                        strcpy(comando, "????");
                    }
                } else if(n3 == 0x6){
                    if(n4 == 0x5){
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
        
        printf("\n 0x%04X : %04X -> %s", posicao, opcode, comando);
        posicao += 2;
    }

    if(lidos == 1){
        printf("sobrou um byte\n");
    }
    fclose(arquivo);
    return 0;
}
