Para compilar este programa solo se requiere:
- GNU Arm Embedded Toolchain: encargado del cross-compiling y linking
- stm32.ld(linker): donde se indica el layout del dispositivo a programar(RAM, ROM...)
- Makefile: indica los pasos a seguir para la generación de los archivos intermedios como los object files y su combinacion
	para crear el archivo a flashear (.bin/.elf/.hex). En este se indican además las opciones con las que se compila
	por ejemplo punto flotante, processador, optimizaciones y archivos fuente.

El proceso que sigue el toolchain es el siguiente:
Se compilan los archivos fuente con el cross-compiler(o cross assembler en caso de que se programe en ensabladero), generando los object files. 
Posteriormente de mandan al linker el cual se encarga de "pegar" esa informacion con el layout del dispositivo y asi se generan los ejecutables.
El archivo ejecutable se divide en secciones que separan simbolos, datos, instrucciones ejecutables, etc... en un formato estandar .