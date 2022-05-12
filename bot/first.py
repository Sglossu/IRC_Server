# import socket
# import os, sys
import asyncio

conf = {
    "host": "localhost",
    "port": 6667,
    "pasw": "1234",
    "nick": "ded",
    "name": "Maksim"
}

def parce_line(msg: str):
    msg = msg.decode('utf-8').rstrip('\r\n')
    prefix = None
    last_arg = None

    ind_rest = msg.find(" :")
    if ind_rest != -1:
        last_arg = msg[ind_rest + 2:]
        msg = msg[:ind_rest]
    else:
        last_arg = None

    if msg.startswith(":"):
        ind_space = msg.find(' ')
        if ind_space != -1:
            prefix = msg[1:ind_space]
            msg = msg[ind_space + 1:]
        else:
            prefix = msg[1:]
    
    args = msg.split(' ')
    if last_arg is not None:
        args.append(last_arg)
    
    return prefix, args



async def main(conf: dict):
    reader, writer = await asyncio.open_connection(conf["host"], conf["port"])

    def send(msg: str):
        writer.write(msg.encode() + b'\r\n')
    
    send("PASS " + conf['pasw'])
    send("NICK " + conf['nick'])
    send("USER " + conf['name'] + " " + conf['host'] + " " + conf['host'] + " A")
    while True:
        line = await reader.readline()
        prefix, args = parce_line(line)
        #search end MOD
        if (args and args[0] == "372"):
            continue
        if (args and args[0] == "376"):
            break
        print('cant connect to server', prefix, args)
        writer.close()
        exit(1)

    print("succes connect")
    while True:
        line = await reader.readline()
        prefix, args = parce_line(line)
        print(line)


asyncio.run(main(conf))