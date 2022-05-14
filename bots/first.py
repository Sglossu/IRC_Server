import asyncio, aiohttp
from configs import tits, conf, weather_keys



def parce_line(msg: str):
    msg = msg.decode('utf-8').rstrip('\r\n')
    prefix = None
    last_arg = None

    ind_rest = msg.find(" :")
    if ind_rest != -1:
        last_arg = msg[ind_rest + 2:]
        msg = msg[:ind_rest]

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

async def handler_tits(recv, msg):
    return f"NOTICE {recv} :(.)(.)"

async def handler_weater(recv, msg):
    msg = msg.replace(',', ' ').rstrip('?')
    msg = msg.split()
    if len(msg) >= 2:
        city_name = msg[1].lower()
    url_w = f"https://api.openweathermap.org/data/2.5/weather?q={city_name}&appid={conf['k']}&units=metric&lang=ru"
    async with aiohttp.ClientSession(connector=aiohttp.TCPConnector(ssl=False)) as session:
        async with session.get(url_w) as resp:
            if resp.status != 200:
                return f"NOTICE {recv} :cannot get weather for city_name"
            data = await resp.json()
            # print()
            tip = data["weather"][0]["description"]
            tmp = data["main"]["temp"]
            wind = data["wind"]["speed"]
            pogoda = f"Сейчас в {city_name} {tip}, {tmp} градусов. Сила ветра {wind}."

    return f"NOTICE {recv} :{pogoda}"
        
async def handlers(prefix, args):
    match args:
        case "PRIVMSG"|"NOTICE", recv, msg if set(msg.split()) & tits:
            return await handler_tits(recv, msg)
        case "PRIVMSG"|"NOTICE", recv, msg if set(msg.split()) & weather_keys:
            return await handler_weater(recv, msg)
        
    return None

async def main(conf: dict):
    reader, writer = await asyncio.open_connection(conf["host"], conf["port"])

    def send(msg: str):
        writer.write(msg.encode() + b'\r\n')
    
    send("PASS " + conf['pasw'])
    send("NICK " + conf['nick'])    
    send("USER " + conf['name'] + " " + conf['host'] + " " + conf['host'] + " " + conf['name'])
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

    print("Succes connect")
    while True:
        line = await reader.readline()
        if not line:
            break   
        prefix, args = parce_line(line)
        if len(args):
            msg = await handlers(prefix, args)
            if msg is not None:
                send(msg)
    writer.close()


asyncio.run(main(conf))