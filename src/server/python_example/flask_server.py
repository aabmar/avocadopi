from flask import Flask
from flask_cors import CORS
import requests
from bs4 import BeautifulSoup
import re
from flask import request
import os

UPLOAD_FOLDER = './uploads'

app = Flask(__name__)
CORS(app)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

@app.route('/', methods=["GET"])
def hello_world():
    return 'Hello, World!'

@app.route('/file', methods=['POST'])
def recieveFile():
    file = request.files['file']
    filename = file.filename
    file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
    return "Test"



if __name__ == "__main__":
    app.run(debug=True)


#http://flask.pocoo.org/docs/1.0/patterns/fileuploads/