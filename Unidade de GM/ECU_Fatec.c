/******************************************************************************
FileName:	ECU-FATEC. C - FATEC SANTO ANDR�
Dependencies:	Veja a secao de includes
Processor:	PIC18F4550
Compiler:	MPLAB X v. 3.65 + XC8
Company:	Fatec Santo Andre
Author:		Ricardo de Souza e Felipe Alves
Date:		05/05/2021

Software Licence Agreement: Somente para fins didaticos

******************************************************************************
File Description: Este programa funciona como uma ECU de EFI monoponto para o
*	ve�culo Gol verde da praca tecnica.
*	Essa versao � usado no LAB1 para teste das placas

Change History:
1.0	12/02/2019	versao inicial
2.0	01/05/2021	atualizacao para trabalhar com a biblioteca do adc.c
*	Atualizado a posicao dos prototipos das funcoes
*	Adaptado para o PICSIM-LAB
3.0	11/05/21 valores do TPS em porcentagem
******************************************************************************/




// Diretivas do compilador

#include <xc.h>
#include <p18f4550.h>
#include <stdlib.h>

// Includes do Projeto

//#include "Config.h" quando usar a placa Fatec
#include   "config_4550.h"	// quando Usar simulador
#include   "displayLCD.h"	// funcoes do display
#include   "adc.h"		// funcoes das entradas analogicas
#include   "fatec.h"		// funcoes especiais da placa Fatec

// Prototipos das funcoes

void delay_ms (unsigned int tempodeAtraso);
void interrupt high_priority Interrupcoes (void);
//------------------------------------------------------------------
//Define apelidos

#define saidadoBico PORTBbits.RB3	// liga desliga bico injetor
#define oscilador   PORTBbits.RB2	// sinal da interrupcao de tempo
#define sinaldoMAP  PORTBbits.RB1	// interrupcao do sinal do Map
#define sinaldoPIP  PORTBbits.RB0	// interrupcao do sinal do PIP

#define botaoB1	    PORTEbits.RE0
#define botaoB2	    PORTEbits.RE1
#define botaoB3	    PORTEbits.RE2

//#define algumas variaveis globais

unsigned long int tickTime	      =0;	// gera contador de tempo
unsigned long int tempodoRPM	  =0;	// contador de intervalo de tempo
unsigned long int tempodoMAP	  =0;	// contador do intervalo da frequancia
unsigned long int frequenciadoRPM =0;	// valor calculado da freq. do RPM
unsigned long int frequenciadoMAP =0;	// valor calculado da freq. do MAP
unsigned long int rotacaodoMotor  =0;	// rotacao do motor
unsigned long int t100_ms 	   =1000;// contador de tempo de 100ms

unsigned int valordoTPS		      =0;	// media dos valores atuais do TPS
unsigned int valorAnteriordoTPS   =0;	// valor anterior do TPS
unsigned int variacaodoTPS       = 0;
unsigned int taxadeAceleracao	  =0;	// valor da bomba de acelerecao
unsigned int valordoACT		      =0;	// media dos valores da temperatura
unsigned int flipflopdoBotao	  =0;	// flip flop para mudanca de borda
unsigned int ContaBordadoPIP	  =0;	// flip flop para saltar 02 PIPs

unsigned int tempodeInjecao      = 0; //Tempo de inje��o
unsigned int tempodeInjecaoLCD = 100; //Tempo de inje��o para display
unsigned int contadordoAN0        =0; //Contador de leitura do AN0
unsigned int somatoriodoAN0      = 0; //Somat�rio dos valores de AN0
unsigned int contadordoAN1       = 0; // Contador de leitura do AN1
unsigned int somatoriodoAN1      = 0; //Somat�rio dos valores de AN1

//Define constantes Globais

const unsigned char tempodeInjecaoMaximo    = 80; //Valor max.(X0,1ms)
const unsigned char tempodeInjecaonaPartida = 40; // Valor durante a partida
const unsigned char tempodeInjecaoemRegime  = 26; //Valor em opera��o

const unsigned int rotacaodePartidaMinimo = 100;//Partida do motor (min)
const unsigned int rotacaodePartidaMaximo = 600; //Partida do Motor (max)

//*********************************************************************
//                PROGRAMA PRINCIPAL
//*********************************************************************

void main(void)
{
//Texto das Telas

const unsigned char Texto_0[] ="Injecao do Gol";
const unsigned char Texto_1[] = "Prof. Edson 2021";
const unsigned char Texto_2[] = "RPM=     ACT=   ";
const unsigned char Texto_3[] = "TPS=     INJ=   ";
const unsigned char Texto_4[] = "RPM=     MAP=   ";
const unsigned char Texto_5[] = "TPS=   % ACC=   ";

//Define variaveis do Programa principal
    int flipflopdaPagina= 0; //Flip flop de mudan�a de p�gina


//Define as fun��es dos pinos e inicializa os buffers de sa�da
    LATA = 0x00;
    LATB = 0xFF; //Carrega FF para apagar os LEDS
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;

    TRISB = 0x03; //Define PB como sa�das e entradas de interrup��o
    TRISE = 0xFF; //Define como entradas (bot�es)
    TRISC = 0x00; //Define como sa�das
    TRISAbits.TRISA0 = 1; //Entreada para AN0
    TRISAbits.TRISA1 = 1; //Entrada para AN1

//*****************************************************************
//Configura a entrada AN0 e An1 ee todas as oturas do byte RA para digital
//Ver tabela da p�gina 260 do Manual do PIC 18F4550 para outras configura��es

    configuraADC(2);

//Configura Timer 0 (Consulte o Manual do PIC 18F4550 na p�gina 125)
//Lembre-se que o clock � de 48MHz quando se trabalha com USB
//Modo de 8 bits, oscilador Interno, Prescaler Ativo, Prescaler =8 

    T0CON = 0b11000010;
    TMR0L = 105; //Valor do estouro

//Configura a Interrup��o do TImer 0
    INTCONbits.TMR0IE = 1; //Habilita Timer 0 pelo flag de overflow

//Habilita interrup��o externa 0 a 1
//Pino B0 e B1 s�o entradas e pinos B[3-7] s�o sa�das
    INTCONbits.RBIE = 0;
    INTCONbits.INT0IE = 1;
    INTCON3bits.INT1IE= 1;
    INTCONbits.GIE = 1;

//Inicializa display e faz sauda��o

    ConfiguraLCD();
    PosicaoCursorLCD(1,1);
    EscreveFraseRamLCD(Texto_0);
    PosicaoCursorLCD(2,1);
    EscreveFraseRamLCD(Texto_1);
    DesligaCursor();
    delay_ms(2000);
    LimpaDisplay();

//Tela inicial

    PosicaoCursorLCD(1,1);
    EscreveFraseRamLCD(Texto_4);
    PosicaoCursorLCD(2,1);
    EscreveFraseRamLCD(Texto_5);

//La�o Infinito

    while(1)
    {

        if(botaoB1== 0 && flipflopdoBotao == 0) //Avalia borda de descida do bot�o
        {
            flipflopdoBotao = 1; //Memoriza que bot�o foi pressionado
            if(flipflopdaPagina==0)
            {
                PosicaoCursorLCD(1,1); //Mostra Tela de Par�metros 1
                EscreveFraseRamLCD(Texto_2);
                PosicaoCursorLCD(2,1);
                EscreveFraseRamLCD(Texto_3);
                flipflopdaPagina = 1; //Memoriza que o bot�o foi acionado
            }
            else
            {
                if(flipflopdaPagina ==1)
                {
                    PosicaoCursorLCD(1,1); //Mostra Tela de Par�metros 2
                    EscreveFraseRamLCD(Texto_4);
                    PosicaoCursorLCD(2,1);
                    EscreveFraseRamLCD(Texto_5);
                    flipflopdaPagina = 0; //Flag de apoio para detec��o de borda
                }
            }
        }
        else
        {
            if(botaoB1 == 1 && flipflopdoBotao == 1) //Muda de p�gina
            {
                flipflopdoBotao = 0;
            }
        }

//Atualiza os valores dos campos do display
        rotacaodoMotor=  frequenciadoRPM/10;
        rotacaodoMotor = 30000/rotacaodoMotor;
        numbertoLCD(rotacaodoMotor, 1, 5, 5, 5);
        numbertoLCD(variacaodoTPS, 2, 5, 5, 5);
        

        if(flipflopdaPagina == 0) //Valores da P�gina 1
        {
            numbertoLCD(frequenciadoMAP, 1, 14, 5, 3);
            numbertoLCD(taxadeAceleracao, 2, 14, 5, 3);
        }
        else //Valores da p�gina 2
        {
            numbertoLCD(valordoACT, 1, 14, 5, 3);
            numbertoLCD(tempodeInjecao, 2, 14, 1, 3);
        }
    }
}


/*********************************************************************************
 * Esta � a �rea das interrupcoes de tempo e evento (Timer0, INT0 e INT1)
 * 
 * 
 ********************************************************************************/

void interrupt high_priority Interrupcoes(void)
{
    if(INTCONbits.TMR0IF == 1) //Esta interrup��o ocorre a cada 0,1ms
    {
        ADCON0bits.CHS = 00;//Ativa leitura do AN0
        ADCON0bits.GO  = 1;
        
        INTCONbits.TMR0IF = 0; //Reset o flag para a pr�xima interrup��o
        tickTime++; //Acumula o numero de interrup��es para usar como delay
        tempodoRPM++; //Contador para calcular rota��o do motor
        tempodoMAP++; //Contador para calcular frequ�ncia do MAP
        tempodeInjecaoLCD--; //Decrementa o tempo de inje��o
        t100_ms--; //Decrementa o contador de 100ms

        oscilador = !oscilador;

        if(tempodeInjecaoLCD>tempodeInjecaoMaximo) //Limita o tempo
        {
            tempodeInjecaoLCD = tempodeInjecaoMaximo;
        }
        if(tempodeInjecaoLCD<=0) //Evita valores negativos
        {
            tempodeInjecaoLCD = 0; //Evita valores negativos
            saidadoBico = 0;
        }
//***************************************************************************************
//L� a entrada anal�gica AN0 - Sinal do TPS 

        contadordoAN0++;
        somatoriodoAN0 = somatoriodoAN0 + lerADC(0);

        if(contadordoAN0>7) //Acumula 8 medidas
        {
            valordoTPS = somatoriodoAN0 >> 3; //M�dia das 8 medidas
            variacaodoTPS = valordoTPS >>3;
            variacaodoTPS = variacaodoTPS*100/127;
            contadordoAN0 = 0;
            somatoriodoAN0 = 0;
        }

//***************************************************************************************
//L� a entrada anal�gica AN1 - Sinal do ACT 

        contadordoAN1++;
        somatoriodoAN1 = somatoriodoAN1 + lerADC(1);
        if(contadordoAN1 > 7) //Acumula 8 medidas
        {
            valordoACT = somatoriodoAN1 >> 3; //M�dia das 8 medidas
            contadordoAN1 = 0;
            somatoriodoAN1 = 0;
        }

//*****************************************************************************************
//As linhas abaixo ser�o executadas a cada 100ms
//Detectaa derivada do TPS 

        if(t100_ms <= 0)
        {
            if(valordoTPS > valorAnteriordoTPS)
            {
                taxadeAceleracao = valordoTPS - valorAnteriordoTPS;
            }
            valorAnteriordoTPS = valordoTPS;
            t100_ms = 1000; //Recarrega o temporizador
        }
//****************************************************************************************/
        TMR0L = 107 + TMR0L; //Valor do estouro + res�duo para 100us

    /**Com 220 a frequ�ncia � de 20,13563 kHz (24,83 us por interrup��o)
     * Com 183 a frequ�ncia � de 9,967474 kHz (50,16 us por interrup��o)
     * Com 107 a frequ�ncia � de 4,992028 kHz (100,2 us por interrup��o)
    */
    }
//************************************************************************************************
//Gerencia interrup��o Externa 0 do sinal de Rota��o via PIP

    if(INT0IE && INT0IF)
    {
        INT0IF = 0; //Libera da interrup��o

    //Se a 100<rota��o < 600, injeta na borda de descida

        if((rotacaodoMotor>rotacaodePartidaMinimo)&&
            (rotacaodoMotor<rotacaodePartidaMaximo)&& (flipflopdoBotao == 0))    
        {
            saidadoBico = 1; //Liga o bico injetor
            tempodeInjecao = tempodeInjecaonaPartida; // Carrega o tempo de inje��o
            tempodeInjecaoLCD = tempodeInjecao; //Colocar no buffer do display
            ContaBordadoPIP = 0;
        }
        else
        {
    //Se a rotacao > 450, injeta na borda de subida a cada dois pulsos
            if((rotacaodoMotor>450)&&(flipflopdoBotao == 1)&&(ContaBordadoPIP>2))    
            {
                saidadoBico = 1; //Liga o bico injetor
                tempodeInjecao = tempodeInjecaoemRegime; //Carrega o tempo de inje��o
                tempodeInjecaoLCD = tempodeInjecao; //Coloca no buffer do display
                ContaBordadoPIP = 0;
            }
            else
            {
                ContaBordadoPIP++; //Incrementa contador de PIP
            }
        }   

        if(flipflopdoBotao == 1)
        {
            frequenciadoRPM = tempodoRPM; //Atualiza rota��o no display
            tempodoRPM = 0; //Zera contador de tempo da rota��o
        }
        if(flipflopdoBotao == 0)
        {
            INTCON2bits.INTEDG0 = 1; //Liga interrup��o na burda de subida
            flipflopdoBotao = 1;
        }
        else
        {
            INTCON2bits.INTEDG0 = 0; //Liga interrup��o na borda de descida
            flipflopdoBotao = 0;
        }
    }

//******************************************************************************************************
// Gerencia Interrup��oi Externa 1 do sinal de MAP
        if((INT1IE && INT1IF))
        {
            INT1IF = 0; //Libera da interrup��o

            frequenciadoMAP = tempodoMAP; //Carrega a contagem e pulso entre bordas
            tempodoMAP = 0; //Zera contador
            frequenciadoMAP = 10000/frequenciadoMAP; //Apenas converte para frequ�ncia
        }
}
    
/**
 * Esta fun��o � usada para gerar um delay program�vel, cuja base de tempo � de 
 * 1 ms. Assim se voc� colocar delay_ms(500), ele esperar� um tempo de 500 ms
 * para seguir o programa
 */
void delay_ms(unsigned int tempodeAtraso)     //Gera delay para o novo compilador
{
    while(--tempodeAtraso)
    {
        __delay_ms(1);
    }
}




























