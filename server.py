from flask import Flask, jsonify, send_file, request
import os
import subprocess

app = Flask(__name__)

# Define the folder where JSON files are stored
JSON_FOLDER = "/home/ubuntu/DewaterIQ/json_output_files"
PROGRAM_PATH = "/home/ubuntu/DewaterIQ/DewaterIQ"  # Update this with your actual program path

@app.route('/run-program', methods=['POST'])
def run_program():
    """Execute the C++ program and return its output"""
    try:
        print(f"Executing: {PROGRAM_PATH}")  # Debugging print
        result = subprocess.run([PROGRAM_PATH], capture_output=True, text=True)

        print("STDOUT:", result.stdout)  # Debugging print
        print("STDERR:", result.stderr)  # Debugging print

        return jsonify({
            "status": "success",
            "stdout": result.stdout,
            "stderr": result.stderr,
            "return_code": result.returncode
        })
    except Exception as e:
        print("Execution Error:", str(e))  # Debugging print
        return jsonify({"status": "error", "message": str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

