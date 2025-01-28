## test (1).c
All : test.elf
test.elf : test (1).o
	h8300-elf-gcc -Wl,--section-start=.stack=0x400 -B C:/wingcc/app/../local/h8300-elf -LC:/wingcc/app/ -mint32 -mh -T C:/wingcc/app/h8pic.x -nostartfiles C:/wingcc/app/h8piccrt0.S -o test.elf  test (1).o -lmes_h8e
test (1).o : test (1).c
	h8300-elf-gcc -B C:/wingcc/app/../local/h8300-elf -IC:/wingcc/app/. -mint32 -mh -fPIC -O2 -c test (1).c
