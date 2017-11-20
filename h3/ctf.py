
from pwn import *;

import random
import time
random.seed(time.time())

while(True):
	#Establish the connection
	target = remote('10.10.10.83', 38123);

	print(target.recvline());
	print(target.recvline());
	print(target.recvline());
	target.sendline(str(random.randint(1,65535)));
	print(target.recvline());
	print(target.recvline());