/*******************************************************************************
 FileName:     Leitura de entrada analógica e conversor AD - FATEC SANTO ANDRÉ
 Dependencies: Veja a seção de includes
 Processor:    PIC18F4550
 Compiler:     MPLAB X v3.65 + XC8 1.40
 Company:      FATEC Santo Andre
 Author:       Prof. Edson Kitani
 Date:         07/10/2020
 Software License Agreement: Somente para fins didáticos
 *******************************************************************************
 File Description: O programa parametriza as entradas analógicas, faz a leitura delas e converte o valor de 
 tensão e devolve no display. O programa está configurado para trabalhar no PicsimLab (simulador)* 
 Change History:
 1.0   07/10/2020  Versão inicial
 
*******************************************************************************/

// Includes do Compilador

#include <p18f4550.h>
#include <stdio.h>
#include <stdlib.h>

// Includes do Projeto

#include"Config.h"
#include"displayLCD.h"

#define B0 PORTBbits.RB0
#define B1 PORTBbits.RB1
#define B2 PORTBbits.RB2

//Protótipos das Funções

void delay_ms(unsigned int tempodeatraso);
void chave_comando();


//variáveis globais
unsigned char flag_controle;
unsigned int incremento_rpm;
unsigned int incremento_velocidade;

void main(void)
{    
    //constantes do projeto
    const unsigned char texto_0[] = "FATEC STO. ANDRE";
    const unsigned char texto_1[] = "CLUSTER VEICULAR";
    const unsigned char texto_4[] = "KM/h";
    const unsigned char texto_3[] = "RPM";
    
    unsigned int AN0;
   
    //parametrizacao
    ADCON0 = 0b00000001;
    ADCON1 = 0x0E;
    ADCON2 = 0x8E;
    TRISB = 0x00;
    
    
    

    
    //saudacao do programa
    ConfiguraLCD();
    DesligaCursor();
    PosicaoCursorLCD(1,1);
    EscreveFraseRamLCD(texto_0);
    PosicaoCursorLCD(2,1);
    EscreveFraseRamLCD(texto_1);
    delay_ms(1000);
    
    
    while(1)
    
    {      
//        ADCON0 = 0b00000011;//Ativa a leitura
//        while(ADCON0bits.DONE)//Enquanto a leitura não terminar no display nao aparecera        
//            {     
//            } 
        if(!B1)
        {
            flag_controle = 1;
            chave_comando();
        }
        
        if(!B0){
            if(flag_controle == 1){
                if(incremento_velocidade != 200){
                    incremento_velocidade = incremento_velocidade + 100;                    
                }
                if(incremento_rpm != 8000){
                    incremento_rpm = incremento_rpm + 50;                   
                }

            }else{
                if(incremento_velocidade != 0){
                    incremento_velocidade = incremento_velocidade - 100;
                }
                incremento_rpm = incremento_rpm + 50;
            }  
        }else{
                if(incremento_velocidade != 0){
                    incremento_velocidade = incremento_velocidade - 100;
                }
                if(incremento_rpm != 0){
                    incremento_rpm = incremento_rpm - 50;
                }             
        }

        
        
    }
    
    
    return;
    
}




/******************************************************************************               
* Funcao:   delay_ms(unsigned int delaytime)
* Entrada:  unsigned int delaytime - recebe valores de 0 a 65536
* Saida:    Nenhuma (void)
* Descricao: Gera um atraso de tempo conforme o valor recebido de delaytime
             O tempo interno é baseado no delay 1 ms. Assim, a cada loop temos o
 *           decremento da variável delaytime, até que ele atinja zero. Exemplo:
 *           se o delyatime = 500 a função terminará e retornará para o programa
 *           principal após 500 ms.
/******************************************************************************/

void chave_comando(){
    switch(flag_controle){
        case 1:
            flag_controle = !flag_controle;
        break;
        
        case 0:
            flag_controle = !flag_controle;
        break;       
    }
    
    
    
}
    




void delay_ms(unsigned int tempodeatraso)
{
    while(--tempodeatraso)
    {
        __delay_ms(1); //Gera um atraso de 1 ms
    }
}