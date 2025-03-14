from flask import Flask, jsonify, send_file, request
import os
import subprocess

from flask_cors import CORS
app = Flask(__name__)
CORS(app)  # Allow cross-origin requests

# Define the folder where JSON files are stored
JSON_FOLDER = "/home/ubuntu/DewaterIQ/json_output_files"
PROGRAM_PATH = "/home/ubuntu/DewaterIQ/DewaterIQ"  # Update this with your actual program path

@app.route('/run-program', methods=['POST'])
def run_program():
    """Execute the C++ program and return its output"""
    try:
        print(f"Executing program: {PROGRAM_PATH}")  # Log execution
        result = subprocess.run([PROGRAM_PATH], capture_output=True, text=True)

        print("STDOUT:", result.stdout)  # Log standard output
        print("STDERR:", result.stderr)  # Log standard error
        print("Return Code:", result.returncode)  # Log exit code

        response = {
            "status": "success",
            "stdout": result.stdout.strip(),
            "stderr": result.stderr.strip(),
            "return_code": result.returncode
        }
        print("Response:", response)  # Log what we return
        return jsonify(response)
    except Exception as e:
        error_response = {"status": "error", "message": str(e)}
        print("Execution Error:", error_response)  # Log error
        return jsonify(error_response), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

