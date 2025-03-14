from flask import Flask, jsonify, send_file, request
import os
import subprocess

app = Flask(__name__)

# Define the folder where JSON files are stored
JSON_FOLDER = "/home/ubuntu/DewaterIQ/json_output_files"
PROGRAM_PATH = "/home/ubuntu/DewaterIQ/DewaterIQ"  # Update this with your actual program name

@app.route('/list-json', methods=['GET'])
def list_json_files():
    """List all JSON files in the directory"""
    try:
        files = [f for f in os.listdir(JSON_FOLDER) if f.endswith('.json')]
        return jsonify({"files": files})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/get-json/<filename>', methods=['GET'])
def get_json_file(filename):
    """Serve a JSON file"""
    file_path = os.path.join(JSON_FOLDER, filename)
    if os.path.exists(file_path) and filename.endswith('.json'):
        return send_file(file_path, mimetype='application/json')
    else:
        return jsonify({"error": "File not found"}), 404

@app.route('/run-program', methods=['POST'])
def run_program():
    """Execute the C++ program and return its output"""
    try:
        result = subprocess.run([PROGRAM_PATH], capture_output=True, text=True)
        return jsonify({"status": "success", "output": result.stdout or result.stderr})
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

