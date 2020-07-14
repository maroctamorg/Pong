import json
from flask import Flask, request, jsonify
from flask_restful import Resource, Api

app = Flask(__name__)
api = Api(app)

class Establish(Resource):
	client1IP = 0
	client2IP = 0
	def get(self):
		if (client1IP == 0):
			client1IP = request.remote_addr
		
		elif (client2IP):
			client2IP = request.remote_addr

		else:
			return "Server session is full!"

class Data(Resource):
	def get(self):
		if (request.remote_addr == Establish.client1IP):
			dataFile = open("client1.json", "r")
			data = json.load(dataFile)
			return data

		if (request.remote_addr == Establish.client2IP):
			dataFile = open("client2.json", "r")
			data = json.load(dataFile)
			return data

	def post(self):
		dataIn = request.get_json()

		if (request.remote_addr == Establish.client1IP):
			dataFile = open("client1.json", "w")
			json.dump(dataIn, dataFile)

		if (request.remote_addr == Establish.client2IP):
			dataFile = open("client2.json", "w")
			json.dump(dataIn, dataFile)

		return 201

api.add_resource(Establish, '/')
api.add_resource(Data, '/session')

if __name__ == "__main__":
	app.run(debug=True)
