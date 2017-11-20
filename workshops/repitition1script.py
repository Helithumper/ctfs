#first, import pwntools
from pwn import *;

#Establish the connection
target = remote('ctf.hackucf.org', 10102);

print(target.recvline());

text = ""

print(target.recvline());

print("######")

num = 1;
holding = 0;
while(True):
	#Scan in the input text line
	text = target.recvline();

	print(text)

	print("---")
	if(len(text.split("Value:"))>1):
		text = text.replace("Value: ", "");
		text = text.replace("Repeat: ", "");
	else:
		text = holding;
	if(num==1):
		holding = text;

	print("sending: " + text);

	target.sendline(text);

	num = num +1;

	print("######")

