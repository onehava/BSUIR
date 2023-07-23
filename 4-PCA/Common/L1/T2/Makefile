.PHONY: build clean

build: main.vfd

main.vfd: main.bin
	rm -f main.vfd
	head -c 1474560 /dev/zero > main.vfd
	dd status=noxfer conv=notrunc if=main.bin of=main.vfd

main.bin: main.asm
	nasm -f bin -o main.bin main.asm

clean:
	rm main.vfd main.bin