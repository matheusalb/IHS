#
#  Configure:
#  source /opt/yocto/poky/1.3.2/environment-setup-core2-poky-linux
#

app: app.cpp
	$(CXX) app.cpp -o app -lm -lallegro
	# cp app /opt/tftp
	
clean:
	rm -rf *.o *~ core app
