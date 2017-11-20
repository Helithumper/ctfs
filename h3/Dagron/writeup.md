This was an extremely simple flag.

Upon receiving a file, we can see that it is an ELF

~~~
./dagron: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=ca0c3ed544945ed136599b23183a480f0badcff2, not stripped
~~~

So, we do what we always do with ELFs, run it
~~~
                                  /   \       
 _                        )      ((   ))     (
(@)                      /|\      ))_((     /|\
|-|                     / | \    (/\|/\)   / | \                      (@)
| | -------------------/--|-voV---\`|'/--Vov-|--\---------------------|-|
|-|                         '^`   (o o)  '^`                          | |
| |                               `\Y/'                               |-|
|-|                                                                   | |
| |                Welcome to Hungry Hungry Dragons!                  |-|
|-|                                                                   | |
| |                                                                   |-|
|_|___________________________________________________________________| |
(@)              l   /\ /         ( (       \ /\   l                `\|-|
                 l /   V           \ \       V   \ l                  (@)
                 l/                _) )_          \I
                                   `\ /'

~~~

Nothing really here, so let's do some investigation.

How about strings

~~~
$ strings ./dagron 
~~~

If we look through the strings we see the following line

~~~
flag{31b9307b77418383}
~~~
Simple!