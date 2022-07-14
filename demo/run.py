from flask import Flask, render_template, jsonify
from datetime import timedelta
import judge
app = Flask(__name__)
app.config['SEND_FILE_MAX_AGE_DEFAULT'] = timedelta(seconds=1)
global ai
@app.route('/')
def cover():
    return render_template('cover.html')
@app.route('/index')
def index():
    return render_template('index.html')
@app.route('/human')
def human():
    return render_template('11.html')
@app.route('/setting/<x>', methods=['GET'])
def setting(x):
    global ai
    ai=judge.AI("./sample",int(x))
    ai.init()
    return jsonify([1,1])

@app.route('/ez_setting/<x>', methods=['GET'])
def ez_setting(x):
    global ai
    ai=judge.AI("./easy",int(x))
    ai.init()
    return jsonify([1,1])


@app.route('/action/<x>+<y>', methods=['GET'])
def get_data(x,y):
    global ai
    a,b = ai.action(int(y),int(x))
    return jsonify([int(b), int(a)])

if __name__ == '__main__':
    # app.run()
    app.run(debug=True,use_reloader=False) 



