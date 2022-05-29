
from functools import partial
from flask import Flask, request, jsonify
from flask_pymongo import PyMongo
from itsdangerous import json 
from marshmallow import Schema, fields, ValidationError
from bson.json_util import dumps
from json import loads
from datetime import datetime
import os
from dotenv import load_dotenv
load_dotenv()

app = Flask(__name__)
app.config["MONGO_URI"] = os.getenv("MONGO_CONNECTION_STRING")
mongo = PyMongo(app) 
# datetime object containing current date and time
date_time = datetime.now()#gets day and time
datetime_string = date_time.strftime("%d/%m/%Y, %H:%M:%S")#formats day and time

FAKE_DATABASE = {} # data base for user objects 
id_count = 0
count = 0



"""TANKS"""

class Level(Schema):
    tank_id= fields.String(required=True)
    water_level = fields.Integer(required=True)

@app.route("/tank",methods = ["POST"])
def add_new_tank():    
    request_dict = request.json

    store_water_level = request.json["water_level"]
    if store_water_level > 79 and store_water_level <101:
            led_status = True
    else:
        led_status = False
    
    try:

        new_tank =  Level().load(request_dict)
    except ValidationError as err:
        return (err.messages,400)
    tank_document = mongo.db.levels.insert_one(new_tank)
    tank_id = tank_document.inserted_id

    tank = mongo.db.levels.find_one({"_id":tank_id})

    tank_json = loads(dumps(tank))
  
   
    message_profile = { #dictionary containing user message data  
        "msg": "data saved in database sucesfully",
        "date":datetime_string,
        "led": led_status
    
    }
    
    global FAKE_DATABASE    # makes fakedatabase a global variable
    FAKE_DATABASE = message_profile  #assigns the user profile dictionary to the fake database
    return jsonify(FAKE_DATABASE)


if __name__ == '__main__':
    app.run(debug=True, port=3000, host="0.0.0.0")
