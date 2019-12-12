/*-----------------------------------------------------------------------------------------------
    ライブラリ名 : Serial_16B version 1.1
    作成者 : h22i03 ISOmura
    作成日 : 2014/09/14
    対象プラットフォーム : LPC176x(青mbed)
    説明 :  mbedのUARTに本来搭載されている、16byteの送受信バッファ(FIFO)を使えるようにしたSerialライブラリです。
           標準のSerialライブラリは1byteしか送受信バッファが使えないので(バグ？)
          RBRとTHRレジスタに直接アクセスすることでFIFOを有効にしています。
           また、write時にFIFOのデータが無くなることをwhileで待たないので、制御ループ等、
          リアルタイム性が必要とされるプログラムにも使用することができます。
          (その代わり、バッファが溢れないように注意してください。)
          ピンヘッダの全UART,USBTX,USBRXに対応しています。
    サンプル : 
        <インスタンス作成>
            Serial_16B device(p9, p10); //引数は普通のSerialと同じ。
            Serial_16B pc(USBTX, USBRX); //USBにも対応。
        <ボーレート設定>
            pc.baud(115200); //普通のSerialと同じ。
            device.baud(38400);
        <書き込み>
            unsigned char buf[3] = {0,0,0};
            buf[0] = 'I';
            buf[1] = 'S';
            buf[2] = 'O';
            pc.write(buf,3); //バッファ(unsigned char)のポインタ,書き込みバイト数
        <読み込み>
            if(pc.readable()) {
                unsigned char data = pc.get(); // 返り値はunsigned char
                if((buf&0x80) == 0x80){
                    //なんか処理
                }
            }
    更新履歴
        v1.1 コメント追加(2014/09/15)
        v1.0 初出(2014/09/14)
-----------------------------------------------------------------------------------------------*/


#ifndef SERIAL_16B_H
#define SERIAL_16B_H

#include "mbed.h"
#include<vector>

#define UART0_RBR_THR (*(volatile unsigned char *)0x4000c000UL)
#define UART1_RBR_THR (*(volatile unsigned char *)0x40010000UL)
#define UART2_RBR_THR (*(volatile unsigned char *)0x40098000UL)
#define UART3_RBR_THR (*(volatile unsigned char *)0x4009c000UL)
#define UART0_LSR (*(volatile unsigned char *)0x4000c014UL)
#define UART1_LSR (*(volatile unsigned char *)0x40010014UL)
#define UART2_LSR (*(volatile unsigned char *)0x40098014UL)
#define UART3_LSR (*(volatile unsigned char *)0x4009c014UL)

class Serial_16B {
public:
    Serial_16B(PinName tx, PinName rx);
    
    void write(vector<unsigned char>& buffer, unsigned int length);
    unsigned char get( void );
    bool readable( void );
    void baud(int baudrate);
    
private:
    Serial serial_mbed;
    
    PinName UART_tx;
    PinName UART_rx;
};
 
#endif