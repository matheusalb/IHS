#
#  Configure:
#  source /opt/yocto/poky/1.3.2/environment-setup-core2-poky-linux
#

app: app.cpp
	g++ sprite.cpp -o hello $(pkg-config allegro-5 allegro_image-5 allegro_font-5 --libs --cflags)
	# $(CXX) app.cpp -o app
	# cp app /opt/tftp
	
clean:
	rm -rf *.o *~ core app
