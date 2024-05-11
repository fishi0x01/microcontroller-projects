CLI_VERSION := 0.35.3
ESP32_VERSION := 1.0.6
DHT_LIB_VERSION := 1.4.6
LCD_LIB_VERSION := 1.1.2

CLI := ./bin/arduino-cli

venv:
	@rm -rf .venv
	@python3 -m venv .venv
	@. .venv/bin/activate && pip3 install --upgrade pip
	@. .venv/bin/activate && pip3 install -r requirements.txt

cli:
	@rm -rf bin/
	@mkdir -p bin/
	@curl -sL https://downloads.arduino.cc/arduino-cli/arduino-cli_$(CLI_VERSION)_Linux_64bit.tar.gz | tar -xz -C bin/
	@rm bin/LICENSE.txt

boards:
	@$(CLI) core update-index --additional-urls https://dl.espressif.com/dl/package_esp32_index.json
	@$(CLI) core install esp32:esp32@$(ESP32_VERSION)
	
libs:
	@$(CLI) lib update-index
	# https://github.com/adafruit/DHT-sensor-library
	@$(CLI) lib install "DHT sensor library@$(DHT_LIB_VERSION)"
	# https://github.com/johnrickman/LiquidCrystal_I2C
	@$(CLI) lib install "LiquidCrystal I2C@$(LCD_LIB_VERSION)"

install: venv cli boards libs

