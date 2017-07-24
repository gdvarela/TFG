from flask import Flask
from flask import request

app = Flask(__name__)


@app.route('/', methods=['GET', 'POST'])
def receive_data():
    if request.method == 'POST':
        if 'message' in request.form:
            print request.form['message']
            return "Mensagem recebida!"
        else:
            print "error"
            return "Mensagem nao recebida!"
    else:
        if 'message' in request.args:
            print request.args['message']
            return 'Mensagem Recebida!'
        else:
            return 'Mensagem nao recebida!'


if __name__ == '__main__':
    app.run(host='0.0.0.0')
