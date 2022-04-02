#CLI_VERSION := 0.13.0
CLI_VERSION := 0.21.1
ESP32_VERSION := 1.0.4
DHT_LIB_VERSION := 1.4.3

CLI := ./bin/arduino-cli

venv:
	@rm -rf .venv
	@python3 -m venv .venv
	@. .venv/bin/activate && pip3 install --upgrade pip
	@. .venv/bin/activate && pip3 install -r requirements.txt

deps:
	@$(CLI) core update-index --additional-urls https://dl.espressif.com/dl/package_esp32_index.json
	@$(CLI) core install esp32:esp32@$(ESP32_VERSION) --additional-urls https://dl.espressif.com/dl/package_esp32_index.json
	@$(CLI) lib install "DHT sensor library@$(DHT_LIB_VERSION)"
	@cp patch/esp32-hal-bt.c ~/.arduino15/packages/esp32/hardware/esp32/1.0.4/cores/esp32/esp32-hal-bt.c

cli:
	@rm -rf bin/
	@mkdir -p bin/
	@curl -sL https://downloads.arduino.cc/arduino-cli/arduino-cli_$(CLI_VERSION)_Linux_64bit.tar.gz | tar -xz -C bin/
	@rm bin/LICENSE.txt

compile:
	@. .venv/bin/activate && $(CLI) compile --fqbn esp32:esp32:esp32 --libraries $(shell pwd)/libs $(shell pwd)/src/main --build-property build.extra_flags='-DCORE_DEBUG_LEVEL=5 -DESP32' --build-property build.partitions=huge_app --build-property upload.maximum_size=3145729 

compile-release:
	@. .venv/bin/activate && $(CLI) compile --fqbn esp32:esp32:esp32 --libraries $(shell pwd)/libs $(shell pwd)/src/main --build-property build.extra_flags='-DCORE_DEBUG_LEVEL=0 -DESP32 -DDEVICE_ID=$(shell date +%s)' --build-property build.partitions=huge_app --build-property upload.maximum_size=3145728 

upload: compile
	@. .venv/bin/activate && bin/arduino-cli upload --port /dev/ttyUSB0 --fqbn esp32:esp32:esp32 src/main
	@stty 115200 -F /dev/ttyUSB0 raw -echo
	@cat /dev/ttyUSB0

upload-release: compile-release
	@. .venv/bin/activate && bin/arduino-cli upload --port /dev/ttyUSB0 --fqbn esp32:esp32:esp32 src/main

log: 
	@stty 115200 -F /dev/ttyUSB0 raw -echo
	@cat /dev/ttyUSB0

detect-lcd-address:
	@. .venv/bin/activate && $(CLI) compile --fqbn esp32:esp32:esp32 --libraries $(shell pwd)/libs $(shell pwd)/src/detect-lcd-address --build-properties build.extra_flags='-DCORE_DEBUG_LEVEL=5 -DESP32' --build-properties build.partitions=huge_app,upload.maximum_size=3145729
	@. .venv/bin/activate && bin/arduino-cli upload --port /dev/ttyUSB0 --fqbn esp32:esp32:esp32 src/detect-lcd-address
	@stty 115200 -F /dev/ttyUSB0 raw -echo
	@cat /dev/ttyUSB0
