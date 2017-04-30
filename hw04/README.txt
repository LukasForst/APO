Zadání domácího úkolu číslo 4
=============================

V souborech program-x86.list a program-mips.list naleznete
výpis programu z paměti počítače.

Program byl nejdříve zkompilovaný překladačem jazyka C pro cílovou platformu
x86, 32-bitový režim a operační systém GNU/Linux. Poté byl stejný program
zkompilovaný překladačem pro little-endian variantu architektury MIPS32.

Výpis obsahuje funkci toplevel_fnc(), která volá funkci subroutine_fnc().
Dále se v programu vyskytují systémová volání.

Vaším úkolem je analyzovat funkci programu.

V souboru syscalls.lst naleznete jména systémových volání,
která se mohou v programu vyskytovat. Upravte seznam
tak, aby obsahoval jen ta volání, která skutečně program
používá. Dodržte formát jedno jméno volání na jeden řádek.
Nepoužívejte žádné oddělovače.
(správně vyplněný seznam 1 bod)


Na základě analýzy funkce subroutine_fnc() vyplňte seznam
parametrů funkce v hlavičkovém souboru subroutine.h.
Parametry budou obvykle celá čísla (int), znaky (char)
nebo ukazatele na tyto typy.
(správný počet a typ argumentů 2 body)

V souboru subroutine.c implementujte funkci subroutine_fnc()
s ekvivalentním chováním, jako měla původní funkce v assembleru.

V souboru toplevel.c implementujte zbytek funkcionality
programu s tím, že tato funkce bude vracet stejnou číselnou
hodnotu jako původní program.

Odevzdaný program musí být možné zkompilovat kompilátorem.
Ekvivalent volání make v adresáři. Instrukce v Makefile
pro sestavovací program neupravujte.


README.txt - tento soubor
program-mips.list - výpis paměti ze systému s architekturou MIPS,
                    do souboru si můžete dělat poznámky
program-x86.list - výpis paměti ze systému s architekturou x86
                    do souboru si můžete dělat poznámky
Makefile - instrukce pro sestavovací program, neupravujte
main.c - vstupní bod programu, volání funkce toplevel_fnc(), neupravujte
subroutine.h - prototyp funkce subroutine_fnc(), vyplňte typ návratové hodnoty
                    a paremtry
subroutine.c - implementace funkce, vyplňte
toplevel.c - implementace nejvyšší úrovně programu
syscalls.lst - seznam jen těch systémových volání, která jsou
               v programu využitá, upravte do této podoby
