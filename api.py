from flask import Flask, request
from flask_restful import Resource, Api

app = Flask(__name__)
api = Api(app)

class Data(Resource):
	def get(self):
		return {'data': 3.1415}
	def post(self):
		incoming = request.get_json()
		return 'received data: ' + str(incoming["data"]), 201

api.add_resource(Data, '/')

if __name__ == "__main__":
	app.run(debug=True)
