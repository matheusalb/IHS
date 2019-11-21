!#/bin/sh

cd driver
rm /dev/de2i150_altera
rmmod altera_driver
make clean
make
insmod altera_driver.ko
mknod /dev/de2i150_altera c 91 1

cd ..
cd app

make clean
make
sudo ./app