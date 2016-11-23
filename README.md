# asm2c #
converts assembler source code into C source code

## Usage ##
Example usage (hardcoding optimizations):
```bash
echo 'main(){puts("Hello World!");}' | gcc -x c -S -masm=intel -O2 -o - - | asm2c -o c_code.c
```
