#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

class De150
{
public:
    De150(string s)
    {
        cout << "hello, " << s << endl;
    }

    void printTest() {
        cout << "teste" << endl;
    }

    void writeDisplayRight(int number)
    {
        int dev = open("/dev/de2i150_altera", O_RDWR);

        int num1 = number / 1000;
        int num2 = (number % 1000) / 100;
        int num3 = ((number % 1000) % 100) / 10;
        int num4 = ((number % 1000) % 100) % 10;

        uint32_t numero = (numbersSeven[num4] << 8) | numbersSeven[num3];
        numero = (numero << 8) | numbersSeven[num2];
        numero = (numero << 8) | numbersSeven[num1];
        real_write(0, dev, &numero, HEX_RIGHT);
        close(dev);
    }

    void writeDisplayRight(string number)
    {
        // to write in 7 display segment: concatenate bits from the rightmost to the leftmost
        // num4 << num3 << num2 << num1
        // no display [num1, num2, num3, num4]
        int dev = open("/dev/de2i150_altera", O_RDWR);
        int num1 = number[0] - '0';
        int num2 = number[1] - '0';
        int num3 = number[2] - '0';
        int num4 = number[3] - '0';

        uint32_t numero = (numbersSeven[num4] << 8) | numbersSeven[num3];
        numero = (numero << 8) | numbersSeven[num2];
        numero = (numero << 8) | numbersSeven[num1];

        real_write(0, dev, &numero, HEX_RIGHT);
        close(dev);
    }

    void writeDisplayLeft(int numberLeft, int numberRight)
    {
        // Números de dois digitos cada, pfvr obg, é oq da na placa
        int dev = open("/dev/de2i150_altera", O_RDWR);

        int num1 = numberLeft / 10;
        int num2 = numberLeft % 10;
        int num3 = numberRight % 10;
        int num4 = numberRight / 10;

        uint32_t numero = (numbersSeven[num4] << 8) | numbersSeven[num3];
        numero = (numero << 8) | numbersSeven[num2];
        numero = (numero << 8) | numbersSeven[num1];
        real_write(0, dev, &numero, HEX_LEFT);
        close(dev);
    }

    void writeDisplay(int left, int middle, int right)
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
    int readButton()
    {
        int i, j = 1, k;
        int ret = 0;
        uint32_t op_num, ant;

        int dev = open("/dev/de2i150_altera", O_RDWR);
        bool count = true;

        while (count)
        {
            ant = 15;
            numberButton = ant;

            while (numberButton == ant)
                read(dev, &numberButton, BUTTONS);

            if (numberButton >= 0 && numberButton < 15)
            {
                printf("leu: %d\n", numberButton);
                count = false;
            }
        }

        close(dev);

        if (numberButton == 7)
            return 3;
        else if (numberButton == 11)
            return 2;
        else if (numberButton == 13)
            return 1;
        else if (numberButton == 14)
            return 0;
        else
            return -1;
    }

private:
    char numbersSeven[10] = {~0x3F, ~0x06, ~0x5B, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x07, ~0x7f, ~0x6f};
    enum OPS
    {
        HEX_RIGHT,
        HEX_LEFT,
        RED_LEDS,
        GREEN_LEDS,
        BUTTONS,
        SWITCHES
    };

    uint32_t numberSwitch = 0;
    uint32_t numberButton = 20;

    void real_write(int debug, int dev, uint32_t *buffer, int opt)
    {
        int ret;
        // wait();
        ret = write(dev, buffer, opt);
        ret = write(dev, buffer, opt);
        if (ret != -1)
            printf("Escrita ok!\n");
    }
};