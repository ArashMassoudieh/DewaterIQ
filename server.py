from flask import Flask, request, jsonify
from flask_cors import CORS
import subprocess

app = Flask(__name__)
CORS(app)  # Enable CORS for cross-origin requests

@app.route('/run-program', methods=['POST'])
def run_program():
    try:
        result = subprocess.run(["/home/ubuntu/DewaterIQ/DewaterIQ"], capture_output=True, text=True)
        response = {"status": "success", "output": result.stdout}
        print("Response:", response)  # Debug print
        return jsonify(response)  # Ensure the response is in JSON format
    except Exception as e:
        error_response = {"status": "error", "message": str(e)}
        print("Error:", error_response)  # Debug print
        return jsonify(error_response), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

