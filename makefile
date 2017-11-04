BUNDLE = sdlmeter.lv2 
INSTALL_DIR = /usr/local/lib/lv2


$(BUNDLE): manifest.ttl sdlmeter.so
		rm -rf $(BUNDLE)
			mkdir $(BUNDLE)
				cp manifest.ttl sdlmeter.so sdlmeterui.so $(BUNDLE)

sdlmeter.so: sdlmeter.c
		gcc -shared -fPIC sdlmeter.c `pkg-config --cflags --libs lv2-plugin` -o sdlmeter.so 

sdlmeterui.so: ui.c
		gcc -shared -fPIC ui.c `pkg-config --cflags --libs lv2-plugin sdl2 xcb` -o sdlmeterui.so

all: sdlmeter.c ui.c 
		gcc -shared -fPIC sdlmeter.c `pkg-config --cflags --libs lv2-plugin` -o sdlmeter.so 
		gcc -shared -fPIC ui.c `pkg-config --cflags --libs lv2-plugin sdl2 xcb` -o sdlmeterui.so
	
install: $(BUNDLE)
		mkdir -p $(INSTALL_DIR)
			rm -rf $(INSTALL_DIR)/$(BUNDLE)
				cp -R $(BUNDLE) $(INSTALL_DIR)

clean:
		rm -rf $(BUNDLE) sdlmeter.so sdlmeterui.so

deploy: 
		cp sdlmeterui.so sdlmeter.so sdlmeter.ttl manifest.ttl sdlmeter.lv2
		cp -R sdlmeter.lv2 ~/.lv2
