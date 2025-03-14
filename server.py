from flask import Flask, request
import subprocess

app = Flask(__name__)

@app.route('/run-program', methods=['POST'])
def run_program():
    try:
        result = subprocess.run(["/home/ubuntu/your-program"], capture_output=True, text=True)
        return {"status": "success", "output": result.stdout}
    except Exception as e:
        return {"status": "error", "message": str(e)}, 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

