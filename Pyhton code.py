from flask import Flask, request
import time

app = Flask(__name__)

dict_list_data = {
    "time": [],
    "value": []
}

@app.route('/data',  methods=['GET'])
def sensor_data():
    
    return dict_list_data, 200

@app.route('/create',  methods=['POST'])
def post_data():
    data = request.json
    data['time'] = time.time()
    dict_list_data['time'].append(data['time'])
    dict_list_data['value'].append(data['value'])
    
    return f"time: {data['time']}, data: {data['value']}", 300

if __name__ == '_main_':
    app.run(debug=True, port=8000, host="192.129.90.30")