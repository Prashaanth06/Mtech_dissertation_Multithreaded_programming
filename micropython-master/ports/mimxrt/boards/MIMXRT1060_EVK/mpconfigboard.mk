MCU_SERIES = MIMXRT1062
MCU_VARIANT = MIMXRT1062DVJ6A

JLINK_PATH ?= /media/RT1060-EVK/

deploy: $(BUILD)/firmware.bin
	cp $< $(JLINK_PATH)
