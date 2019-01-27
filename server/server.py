import os

from flask import Flask
from flask import jsonify
from flask import request

FLASK_DEBUG = os.environ.get('FLASK_DEBUG', True)
SUPPORTED_EXTENSIONS = ('.png', '.jpg', '.jpeg')

app = Flask(__name__)

def id_to_move(argument): 
    switcher = { 
        0: "compost", 
        1: "recyclage", 
        2: "ordures", 
        3: "dangereux", 
        4: "materiaux",
    } 
  
    return switcher.get(argument, "err") 

def allowed_file(filename):
    return filename.lower().endswith(SUPPORTED_EXTENSIONS)

@app.route("/ping")
def ping():
    return "ping"

# @app.route('/move/<type_id>', methods=['GET'])
# def move_servo(type_id):
#     id_to_move(type_id)

if __name__ == "__main__":
    app.run(debug=FLASK_DEBUG, host='0.0.0.0', port=5005)