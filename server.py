from flask import Flask, jsonify, send_file
import os

app = Flask(__name__)

# Define the folder where JSON files are stored
JSON_FOLDER = "/home/ubuntu/DewaterIQ/json_output_files"

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

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

