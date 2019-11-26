ALLEGRO_FLAG = $(shell pkg-config --cflags --libs allegro-5.0 allegro_acodec-5.0 allegro_audio-5.0 allegro_color-5.0 allegro_dialog-5.0 allegro_font-5.0 allegro_image-5.0 allegro_main-5.0 allegro_memfile-5.0 allegro_physfs-5.0 allegro_primitives-5.0 allegro_ttf-5.0)
# CPPFLAGS += -std=c++0x 

app:
	g++ app.cpp $(CPPFLAGS) $(ALLEGRO_FLAG) -o app
	echo "Main compilada"

clean:
	rm -f *.o app

# app: app.cpp
# 	$(CXX) app.cpp -o app
# 	# cp app /opt/tftp
	
# clean:
# 	rm -rf *.o *~ core app
