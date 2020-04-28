import sqlite3

hostsdb = '__HOME__/request_handler/map.db'

def request_handler(request):

    if(request["method"] == "GET"):
        abshost = request["values"]["abshost"] 

        conn = sqlite3.connect(hostsdb)
        c = conn.cursor()
        queryresults = c.execute('''SELECT * FROM hostsdb WHERE abshost = ?;''', (abshost,)).fetchone()
        conn.commit(); conn.close()

        try: rtv = queryresults[1]
        except KeyError: rtv = abshost
        return rtv

    abshost = request["form"]["abshost"]
    relhost = request["form"]["relhost"]

    conn = sqlite3.connect(hostsdb)
    c = conn.cursor()
    c.execute('''CREATE TABLE IF NOT EXISTS hostsdb (abshost text, relhost text);''')   
    c.execute('''DELETE FROM hostsdb WHERE abshost = ?;''', (abshost,))
    c.execute('''INSERT INTO hostsdb VALUES (?, ?);''', (abshost, relhost))

    conn.commit(); conn.close(); return "Complete"


