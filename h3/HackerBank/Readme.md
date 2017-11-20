# Hacker Bank
This challenge looked daunting, but ended up being a simple client-side validation problem

After creating an account, the user was greeted with the following page:
![Original Webpage](https://raw.githubusercontent.com/Helithumper/ctfs/master/h3/HackerBank/original_hacker_bank.png)

You are given 10 credits and have the ability to send credits. 

For a while we thought this was going to be a bruteforce question until we looked at the code behind the text box after `Amount to send:`

![Vulnerability](https://raw.githubusercontent.com/Helithumper/ctfs/master/h3/HackerBank/vuln_hacker_bank.png)

Here, we can change the minimum amount to send. So what if we send -1,000,000 credits to someone? 

![Fin](https://raw.githubusercontent.com/Helithumper/ctfs/master/h3/HackerBank/after_pwn_hacker_bank.png)

The flag is `flag{bMQOcrqNAXF5}`