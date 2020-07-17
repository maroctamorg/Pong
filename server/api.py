import json
from flask import Flask, request, jsonify
from flask_restful import Resource, Api

app = Flask(__name__)
api = Api(app)

class Establish(Resource):
	def get(self):
		with open("sessionData.json", "r+") as sessionData:
			session = json.load(sessionData)

			if (session["client1IP"] == " " and session["client2IP"] != request.remote_addr):
				session["client1IP"] = request.remote_addr
				sessionData.seek(0)
				json.dump(session, sessionData)
				sessionData.truncate()

				with open("client1.json", "w") as clientData:
					client_data = {"done":False,"lcl_cPos":{"x":80,"y":50},"rmt_cPos":{"x":720,"y":50}}
					json.dump(client_data, clientData)

				return jsonify(sss1wEst=True, sss2wEst=False, v=5)

			elif (session["client2IP"] == " " and session["client1IP"] != request.remote_addr):
				session["client2IP"] = request.remote_addr
				sessionData.seek(0)
				json.dump(session, sessionData)
				sessionData.truncate()

				with open("client2.json", "w") as clientData:
					client = {"done":False,"lcl_cPos":{"x":80,"y":50},"rmt_cPos":{"x":720,"y":50}}
					json.dump(client, clientData)
					
				return jsonify(sss1wEst=True, sss2wEst=True, v=-5)

			elif ((session["client1IP"] != session["client2IP"]) and (session["client1IP"] != " " and session["client2IP"] != " ")):
				with open("client1.json", "w") as clientData:
					client_data = {"done":False,"lcl_cPos":{"x":80,"y":50},"rmt_cPos":{"x":720,"y":50}}
					json.dump(client_data, clientData)
				with open("client2.json", "w") as clientData:
					client_data = {"done":False,"lcl_cPos":{"x":80,"y":50},"rmt_cPos":{"x":720,"y":50}}
					json.dump(client_data, clientData)
					if (request.remote_addr == session["client1IP"]):
						return jsonify(sss1wEst=True, sss2wEst=True, v=5)
					elif (request.remote_addr == session["client2IP"]):
						return jsonify(sss1wEst=True, sss2wEst=True, v=-5)

			elif (session["client1IP"] == request.remote_addr):
					return jsonify(sss1wEst = True, sss2wEst = False, v = 5)

			elif (session["client2IP"] == request.remote_addr):
					return jsonify(sss1wEst = True, sss2wEst = False, v = -5)
					
			else:
				return "Unhandled Server Error!"


class Data(Resource):
	def get(self):

		with open("sessionData.json", "r") as sessionData:
			session = json.load(sessionData)

			if (request.remote_addr == session["client1IP"]):
				with open("client1.json", "r") as dataFile:
					data = json.load(dataFile)
					with open("client2.json", "r") as dataFile2:
						data2 = json.load(dataFile2)
						response = data
						response["rmt_cPos"]["x"] = 800 - data2["lcl_cPos"]["x"]
						response["rmt_cPos"]["y"] =  data2["lcl_cPos"]["y"]
						#response["ball"]["pos"][0] = 800 - data2["ball"]["pos"][0]
						#response["ball"]["vel"][0] = -data2["ball"]["vel"][0]
						return response

			elif (request.remote_addr == session["client2IP"]):
				with open("client2.json", "r") as dataFile:
					data = json.load(dataFile)
					with open("client1.json", "r") as dataFile2:
						data2 = json.load(dataFile2)
						response = data
						response["rmt_cPos"]["x"] = 800 - data2["lcl_cPos"]["x"]
						response["rmt_cPos"]["y"] =  data2["lcl_cPos"]["y"]
						#response["ball"]["pos"][0] = 800 - data2["ball"]["pos"][0]
						#response["ball"]["vel"][0] = -data2["ball"]["vel"][0]
						return response

			else:
				return 'Unhandled IP:' + session["client1IP"] + '!=' + request.remote_addr + '.\nUndefined game session\n', 300

	def post(self):

		with open("sessionData.json", "r") as sessionData:
			session = json.load(sessionData)
			dataIn = request.get_json()
			print(dataIn)

			if (request.remote_addr == session["client1IP"]):
				with open("client1.json", "w") as dataFile:
					json.dump(dataIn, dataFile)
				return 202

			elif (request.remote_addr == session["client2IP"]):
				with open("client2.json", "w") as dataFile:
					json.dump(dataIn, dataFile)

					return 202
		
			else:
				return 'Unhandled IP: undefined game session', 300


api.add_resource(Establish, '/establish')
api.add_resource(Data, '/data')


if __name__ == "__main__":
	app.run(host='0.0.0.0')
