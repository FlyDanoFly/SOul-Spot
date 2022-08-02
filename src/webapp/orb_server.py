import datetime
import json
import os
import struct
import sqlite3
from http import HTTPStatus

from flask import Flask, render_template, request, send_from_directory
# from flask_caching import Cache

from database import init_database
from programs import PROGRAMS


DB_FILENAME = '/tmp/penguin.sqlite3'


app = Flask(__name__)
app.config['SECRET_KEY'] = '11235eureka'
app.config['CACHE_TYPE'] = 'SimpleCache'
# cache = Cache(app)


@app.route('/favicon.ico') 
def favicon(): 
    return send_from_directory(os.path.join(app.root_path, 'static'), 'favicon.ico', mimetype='image/vnd.microsoft.icon')


@app.route('/ajax', methods=['POST'])
def ajax():
    thingy = request.get_json()
    print(thingy)
    program = thingy['program_mode']
    mymap = PROGRAMS[program]
    mymap['process'](thingy)
    return ('', HTTPStatus.NO_CONTENT)


@app.route('/')
def hello():
   now = datetime.datetime.now()
   timeString = now.strftime("%Y-%m-%d %H:%M")
   templateData = {
      'title' : 'HELLO!',
      'time': timeString,
      'PROGRAMS': PROGRAMS,
      }
   return render_template('penguin.jinja', **templateData)


if __name__ == '__main__':
    init_database(PROGRAMS)
    app.run(debug=True, port=8008, host='0.0.0.0')