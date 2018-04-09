PI_VER := RPIBPLUS
CFLAGS := -O0 -D$(PI_VER) -I. -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -nostartfiles -g -Wl,-T,rpi.x
SRC    := $(wildcard kernel/*.c kernel/*.S arm/*.c arm/*.S)

all: build/kernel.img

build/kernel.elf: $(SRC)
	arm-none-eabi-gcc $(CFLAGS) $(SRC) -o $@

%.img: %.elf
	arm-none-eabi-objcopy $< -O binary $@

clean:
	rm -f build/*.elf build/*.img