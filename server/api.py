import json
from flask import Flask, request, jsonify
from flask_restful import Resource, Api

app = Flask(__name__)
api = Api(app)

class Establish(Resource):
	def get(self):
		with open("sessionData.json", "r+") as sessData:
			session = json.load(sessData)

			if (session["client1IP"] == " " or session["client1IP"] == request.remote_addr):
				session["client1IP"] = request.remote_addr
				sessData.seek(0)
				json.dump(session, sessData)
				sessData.truncate()

				with open("client1.json", "w") as clientData:
					client_data = {'lcl_cPos':{'x':80, 'y':50}, 'rmt_cPos':{'x':720, 'y':50}}
					json.dump(client_data, clientData)

				return jsonify(sss1wEst=True, sss2wEst=False)

			elif (session["client2IP"] == " " and session["client1IP"] != request.remote_addr):
				session["client2IP"] = request.remote_addr
				sessData.seek(0)
				json.dump(session, sessData)
				sessData.truncate()

				with open("client2.json", "w") as clientData:
					client_data = {'lcl_cPos':{'x':80, 'y':50}, 'rmt_cPos':{'x':720, 'y':50}}
					json.dump(client_data, clientData)
					
				return jsonify(sss1wEst=True, sss2wEst=True)

			elif (session["client1IP"] != session["client2IP"] and (session["client1IP"] != " " and session["client2IP"] != " ")):
				return jsonify(sss1wEst=True, sss2wEst=True)
			
			else:
				return jsonify(sss1wEst=False, sss2wEst=False)


class Data(Resource):
	def get(self):

		with open("sessionData.json", "r") as sessData:
			session = json.load(sessData)

			if (request.remote_addr == session["client1IP"]):
				with open("client1.json", "r") as dataFile:
					data = json.load(dataFile)
					return data

			elif (request.remote_addr == ["client2IP"]):
				with open("client2.json", "r") as dataFile:
					data = json.load(dataFile)
					return data

			else:
				return 'Unhandled IP:' + session["client1IP"] + '!=' + request.remote_addr + '.\nUndefined game session\n', 300

	def post(self):

		with open("sessionData.json", "r") as sessData:
			session = json.load(sessData)
			dataIn = request.get_json()

			if (request.remote_addr == session["client1IP"]):
				with open("client1.json", "w") as dataFile:
					json.dump(dataIn, dataFile)
				with open("client2.json", "w") as dataFile2:
					dataOut = json.load(dataFile2)
					dataOut["rmt_cPos"]["x"] = 800 - dataIn["lcl_cPos"]["x"]
					dataOut["rmt_cPos"]["y"] = 500 - dataIn["lcl_cPos"]["y"]
					json.dump(dataOut, dataFile2)
				return 202

			elif (request.remote_addr == session["client2IP"]):
				with open("client2.json", "w") as dataFile:
					json.dump(dataIn, dataFile)
				with open("client1.json", "w") as dataFile2:
					dataOut = json.load(dataFile2)
					dataOut["rmt_cPos"]["x"] = 800 - dataIn["lcl_cPos"]["x"]
					dataOut["rmt_cPos"]["y"] = 500 - dataIn["lcl_cPos"]["y"]
					json.dump(dataOut, dataFile2)

					return 202
		
			else:
				return 'Unhandled IP: undefined game session', 300


api.add_resource(Establish, '/')
api.add_resource(Data, '/session')


if __name__ == "__main__":
	app.run(host='0.0.0.0')
