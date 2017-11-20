import socket;

TCP_IP = '10.10.10.33'
TCP_PORT = 80
BUFFER_SIZE = 1024

message = "POST /register HTTP/1.1Host: 10.10.10.33Content-Length: 178Cache-Control: max-age=0Origin: http://10.10.10.33Upgrade-Insecure-Requests: 1Content-Type: application/x-www-form-urlencodedUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8DNT: 1Referer: http://10.10.10.33/registerAccept-Encoding: gzip, deflateAccept-Language: en-US,en;q=0.8Cookie: session=.eJwNjkEOAjEIAP_SswcoUIqf2VAKiTHRRL0Z_-7eZzLzbcdtt2sb2hWSqnRJVi9TiwVODsrUa5cbrmQkPImBWj1G2hSUQGbO09EK2A5s1lMc0KcEieBce6_JTIIWkltpbc_BAhPLzLWHebu0eL_q-Dzv-Th_OjlaR1CoSrSpwbBt5fQBOnU5lcpZar8_iK01uQ.DN-uFA.6n_cUAcq1RNKUabnhmQDGGoKvW8Connection: closenext=&csrf_token=IjIzYTE5MjEwNzBmZmUxOTg3YzQwZDliZThhNjA3ODdiYTNmNzU5OTIi.DN-uFQ.-Obnd6f0_QF196YU5JNXUQsRHoE&email=peyton@king.com&password=asdfasdf&password_confirm=asdfasdf&submit=Register"

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((TCP_IP,TCP_PORT));
s.send(message)

data = s.recv(BUFFER_SIZE);

s.close()

print("Data Reveived: "+data);