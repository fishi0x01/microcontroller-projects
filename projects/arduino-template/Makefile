CLI := ../../bin/arduino-cli
VENV := ../../.venv/bin/activate

compile:
	@. $(VENV) && $(CLI) compile --fqbn esp32:esp32:esp32 $(shell pwd)/src/main --build-property build.extra_flags='-DCORE_DEBUG_LEVEL=5 -DESP32' --build-property build.partitions=huge_app --build-property upload.maximum_size=3145729 

compile-release:
	@. $(VENV) && $(CLI) compile --fqbn esp32:esp32:esp32 $(shell pwd)/src/main --build-property build.extra_flags='-DCORE_DEBUG_LEVEL=0 -DESP32 -DDEVICE_ID=$(shell date +%s)' --build-property build.partitions=huge_app --build-property upload.maximum_size=3145728 

upload: compile
	@. $(VENV) && $(CLI) upload --port /dev/ttyUSB0 --fqbn esp32:esp32:esp32 src/main
	@stty 115200 -F /dev/ttyUSB0 raw -echo
	@cat /dev/ttyUSB0

upload-release: compile-release
	@. $(VENV) && $(CLI) upload --port /dev/ttyUSB0 --fqbn esp32:esp32:esp32 src/main

log: 
	@stty 115200 -F /dev/ttyUSB0 raw -echo
	@cat /dev/ttyUSB0

