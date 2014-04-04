Assembly Interpreter
=============

A simple interpreter of assembly language, supporting commands as follows:

- mov reg reg (assignment)
- mov reg data（assignment）
- add reg reg (addition)
- add reg data (addition)
- sub reg reg (subtraction)
- sub reg data (subtraction)
- mul reg reg (multiplication)
- mul reg data (multiplication)
- div reg reg (division)
- div reg data (division)
- mod reg reg (modulo)
- mod reg data (modulo)
- chg reg reg（value exchange）
- max reg reg (max operation)
- max reg data (max operation)
- min reg reg (min operation)
- min reg data (min operation)
- clr (clear and reset)
- shw (show values of registers)
- inc reg (auto-increasing operation)
- dec reg (auto-decreasing operation)
- int 21 (ah = 1, 2, 3 mean to input character, output character, output value respectively)
- loop label (loop operation)
- jmp label (jump operation)
- jz, je, jnz, jne, jl, jg (conditional jump)
- label: (label)
- **;** (comment)

## Usage

##### On Windows

	cd Windows
	make
	asm.exe ../test/ex1.txt
