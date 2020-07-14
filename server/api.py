import json
from flask import Flask, request, jsonify
from flask_restful import Resource, Api

app = Flask(__name__)
api = Api(app)

class Establish(Resource):
	def get(self):
		with open("sessionData.json", "r+") as sessData:
			session = json.load(sessData)

			if (session["client1IP"] == " "):
				session["client1IP"] = request.remote_addr
				sessData.seek(0)
				json.dump(session, sessData)
				sessData.truncate()


			elif (session["client2IP"] == " "):
				session["client2IP"] = request.remote_addr
				sessData.seek(0)
				json.dump(session, sessData)
				sessData.truncate()

			else:
				return "Server session is full!"

class Data(Resource):
	def get(self):

		with open("sessionData.json", "r") as sessData:
			session = json.load(sessData)

			if (request.remote_addr == session["client1IP"]):
				with open("client1.json", "r") as dataFile:
					data = json.load(dataFile)
	
					return data, 201

			elif (request.remote_addr == ["client2IP"]):
				with open("client2.json", "r") as dataFile:
					data = json.load(dataFile)
					return data, 201
		
			else:
				return 'Unhandled IP:' + session["client1IP"] + '!=' + request.remote_addr + '.\nUndefined game session\n', 300

	def post(self):

		with open("sessionData.json", "r") as sessData:
			session = json.load(sessData)

			dataIn = request.get_json()

			if (request.remote_addr == session["client1IP"]):
				with open("client1.json", "w") as dataFile:
					json.dump(dataIn, dataFile)
	
				return 202

			elif (request.remote_addr == Establish.client2IP):
				with open("client2.json", "w") as dataFile:
					json.dump(dataIn, dataFile)
	
					return 202
		
			else:
				return 'Unhandled IP: undefined game session', 300


api.add_resource(Establish, '/')
api.add_resource(Data, '/session')


if __name__ == "__main__":
	app.run(host='0.0.0.0')
