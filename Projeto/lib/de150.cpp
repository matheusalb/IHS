#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

class De150
{

private:
    unsigned char numbersSeven[10];
    enum OPS
    {
        HEX_RIGHT,
        HEX_LEFT,
        RED_LEDS,
        GREEN_LEDS,
        BUTTONS,
        SWITCHES
    };

    int dev;

    uint32_t numberSwitch;
    uint32_t numberButton;

    void real_write(int debug, int dev, uint32_t *buffer, int opt)
    {
        int ret;
        // wait();
        ret = write(dev, buffer, opt);
        ret = write(dev, buffer, opt);
        if (ret != -1)
            printf("Escrita ok!\n");
    }

public:

    De150(string s)
    {
        cout << "hello, " << s << endl;
        numbersSeven[0] = ~0x3F;
        numbersSeven[1] = ~0x06;
        numbersSeven[2] = ~0x5B;
        numbersSeven[3] = ~0x4f;
        numbersSeven[4] = ~0x66;
        numbersSeven[5] = ~0x6d;
        numbersSeven[6] = ~0x7d;
        numbersSeven[7] = ~0x07;
        numbersSeven[8] = ~0x7f;
        numbersSeven[9] = ~0x6f;

        numberSwitch = 0;
        numberButton = 20;

        openBoard();
    }

    void printTest() {
        cout << "teste" << endl;
    }

    void writeDisplayRight(int number)
    {
        int num1 = number / 1000;
        int num2 = (number % 1000) / 100;
        int num3 = ((number % 1000) % 100) / 10;
        int num4 = ((number % 1000) % 100) % 10;

        uint32_t numero = (numbersSeven[num4] << 8) | numbersSeven[num3];
        numero = (numero << 8) | numbersSeven[num2];
        numero = (numero << 8) | numbersSeven[num1];
        real_write(0, dev, &numero, HEX_RIGHT);
    }

    void writeDisplayRight(string number)
    {
        // to write in 7 display segment: concatenate bits from the rightmost to the leftmost
        // num4 << num3 << num2 << num1
        // no display [num1, num2, num3, num4]
        int num1 = number[0] - '0';
        int num2 = number[1] - '0';
        int num3 = number[2] - '0';
        int num4 = number[3] - '0';

        uint32_t numero = (numbersSeven[num4] << 8) | numbersSeven[num3];
        numero = (numero << 8) | numbersSeven[num2];
        numero = (numero << 8) | numbersSeven[num1];

        real_write(0, dev, &numero, HEX_RIGHT);
    }

    void writeDisplayLeft(int numberLeft, int numberRight)
    {
        // Números de dois digitos cada, pfvr obg, é oq da na placa

        int num1 = numberLeft / 10;
        int num2 = numberLeft % 10;
        int num3 = numberRight % 10;
        int num4 = numberRight / 10;

        uint32_t numero = (numbersSeven[num4] << 8) | numbersSeven[num3];
        numero = (numero << 8) | numbersSeven[num2];
        numero = (numero << 8) | numbersSeven[num1];
        real_write(0, dev, &numero, HEX_LEFT);
    }

    void writeDisplays(int left, int middle, int right)
    {
        writeDisplayLeft(left, middle);
        writeDisplayRight(right);
    }

    uint32_t readSwtichWithButton()
    {
        int i, j = 1, k;
        int ret = 0;
        uint32_t op_num, ant;

        int dev = open("/dev/de2i150_altera", O_RDWR);

        //printf("%d\n",dev);

        // real_write(0,dev,&zerol,RED_LEDS);
        // real_write(0,dev,&zerol,GREEN_LEDS);
        // real_write(0,dev,&number,HEX_RIGHT);
        bool cont = true;

        while (cont)
        {

            read(dev, &ant, BUTTONS);
            numberButton = ant;
            while (numberButton == ant)
                read(dev, &numberButton, BUTTONS);

            if (numberButton == 7)
            {
                read(dev, &op_num, SWITCHES);
                printf("Switches em : %d\n", op_num);
                cont = false;
            }
        }
        close(dev);
        return op_num;
    }

    // Retorna o id do butao (3, 2, 1, 0) na placa;
    int readButton(){
        int ret = -1;
        uint32_t op_num, ant;

        int dev = open("/dev/de2i150_altera", O_RDWR);
        read(dev, &ant, BUTTONS);
        bool count = true;

        while (ret == -1){

            numberButton = ant;
            while (numberButton == ant)
                read(dev, &numberButton, BUTTONS);

            if (numberButton >= 0 && numberButton < 15)
            {
                printf("leu: %d\n", numberButton);
                count = false;
            }

            if(numberButton == 7) ret = 0;
            else if(numberButton == 11) ret = 1;
            else if(numberButton == 13) ret = 2;
            else if(numberButton == 14) ret = 3;
            else ant = numberButton;
        }

        close(dev);
        return ret;
    }

    uint32_t readButtonInput(){
        uint32_t tecla_apert;
        read(dev, &tecla_apert, BUTTONS);
        return tecla_apert;
    }

    int openBoard() {
        dev = open("/dev/de2i150_altera", O_RDWR);
        if(dev == -1){
            printf("de150::openBoard -> nao conseguiu abrir!!\n");
            return -1;
        }
        return 1;
    }

    int closeBoard() {
        close(dev);
    }
};