import sqlite3
import random
import string
import time
import datetime

visits_db = '__HOME__/request_handler/game_data.db'
new_code_db = '__HOME__/request_handler/new_code.db'


def request_handler(request):
    # if (("values" in request and request["values"]["name"] == name) or ("form" in request and request["form"]["game_name"])):
        # can't figure out what the next number is, will just have to figure out what the id is then

    
    if ((request["method"] == 'POST') and ("reset" in request["form"])):#have it reset when you start a new game
        conn = sqlite3.connect(visits_db)
        host = request["form"]["host"]
        c = conn.cursor()
        c.execute('''DELETE FROM visits_db WHERE host == ?;''',(str(host),))
        conn.commit()
        conn.close()
        return "uhhhh"
    elif ((request["method"] == 'POST') and ("game_name" in request["form"])):
        conn = sqlite3.connect(new_code_db)
        c = conn.cursor()
        game_name = request["form"]["game_name"]
        game_code = request["form"]["game_code"]
        host = request["form"]["host"]
        c.execute('''CREATE TABLE IF NOT EXISTS real_code_db (game_name text, game_code text, host text, timing timestamp);''')
        c.execute('''INSERT INTO real_code_db VALUES (?, ?, ?, ?);''', (str(game_name), str(game_code), str(host), datetime.datetime.now()))
        things = c.execute('''SELECT * FROM real_code_db ORDER BY timing DESC;''').fetchall()
        conn.commit()
        conn.close()
        return things[0]
    elif ((request["method"] == 'POST') and ("game_name" not in request["form"])):
        conn = sqlite3.connect(visits_db)
        c = conn.cursor()
        out = ""
        variable_1 = request["form"]["variable_1"]
        variable_2 = request["form"]["variable_2"]
        variable_3 = request["form"]["variable_3"]
        variable_4 = request["form"]["variable_4"]
        variable_5 = request["form"]["variable_5"]
        name = request["form"]["name"]
        host = request["form"]["host"]
        c.execute('''CREATE TABLE IF NOT EXISTS visits_db (name text, variable_1 text, variable_2 text, variable_3 text, variable_4 text, variable_5 text, host text, timing timestamp);''')
        c.execute('''INSERT INTO visits_db VALUES (?, ?, ?, ?, ?, ?, ?, ?);''',(str(name), str(variable_1), str(variable_2),str(variable_3), str(variable_4), str(variable_5), str(host), datetime.datetime.now()))
        things = c.execute('''SELECT * FROM visits_db WHERE host == ? AND name == ? ORDER BY timing DESC;''', (str(host), str(name))).fetchall()
        conn.commit()
        conn.close()
        return things[0]
    elif ((request["method"] == 'GET') and ("game_name" in request["values"])):
        conn = sqlite3.connect(new_code_db)
        c = conn.cursor()
        game_name = request["values"]["game_name"]
        host = request["values"]["host"]
        c.execute('''CREATE TABLE IF NOT EXISTS real_code_db (game_name text, game_code text, host text, timing timestamp);''')
        things = c.execute('''SELECT * FROM real_code_db WHERE game_name == ? AND host == ? ORDER BY timing DESC;''', (str(game_name), str(host))).fetchall()
        game_code = things[0][1]
        conn.commit()
        conn.close()
        return (game_code)
    elif ((request["method"] == 'GET') and ("game_name" not in request["values"])):
        conn = sqlite3.connect(visits_db)
        c = conn.cursor()
        returned = ""
        name = request["values"]["name"]
        host = request["values"]["host"]
        thirty_seconds_ago = datetime.datetime.now() - datetime.timedelta(minutes=5)
        c.execute('''CREATE TABLE IF NOT EXISTS real_code_db (game_name text, game_code text, host text, timing timestamp);''')
        things = c.execute('''SELECT * FROM visits_db WHERE host == ? AND name == ? ORDER BY timing DESC;''', (str(host), str(name))).fetchall()
        conn.commit()
        conn.close()
        if (len(things) == 0):
            return "0"
        else:
            returned += "1"
            for i in range(0, len(things[0])):
                if (i == 0 or i == 6 or i == 7):
                    returned += ";"
                else:
                    try:
                        returned += ";" + str(i) + "=" + str(int(things[0][i]))
                    except ValueError:
                        returned += ";" + "Nope" + ";"
            return returned


