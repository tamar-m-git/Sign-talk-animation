import json
import os
import sys
from flask import jsonify, Flask
from flask import Flask, request, jsonify
from flask_cors import CORS
import traceback
from threading import Lock
import subprocess
#  תיקיית ייצוא
output_dir = os.path.abspath("OUTPUT")
os.makedirs(output_dir, exist_ok=True)
if not os.access(output_dir, os.W_OK):
    raise PermissionError(f"there is no output directory: {output_dir}")


sys.path.append(os.path.dirname(__file__))

# 🌐 Flask app
app = Flask(__name__, static_folder="OUTPUT")
CORS(app)
from flask import send_from_directory

@app.route('/<filename>.gltf')
def serve_gltf_file(filename):
    output_folder = r"C:\CodesProject\blender_server\OUTPUT"
    return send_from_directory(output_folder, f"{filename}.gltf", mimetype="model/gltf+json")

anim_lock = Lock()
# ודא שהתיקייה TmpFiles קיימת
tmp_folder = r"C:\CodesProject\TmpFiles"
os.makedirs(tmp_folder, exist_ok=True)
@app.route("/clear_output", methods=["POST"])
def clear_output():
    output_folder = r"C:\CodesProject\blender_server\OUTPUT"
    for filename in os.listdir(output_folder):
        file_path = os.path.join(output_folder, filename)
        try:
            if os.path.isfile(file_path):
                os.unlink(file_path)
        except Exception as e:
            print(f"Failed to delete {file_path}: {e}")
    return jsonify({"status": "ok"})
@app.route("/create_animation", methods=["POST"])
def create_animation():
    if not anim_lock.acquire(blocking=False):
        return jsonify({"error": "Server is busy"}), 429

    try:
        # 1⃣ קבלת הנתונים מהלקוח
        data = request.get_json()
        if not data or "motion_data" not in data:
            return jsonify({"error": "Missing motion_data"}), 400

        motion_data = data["motion_data"]
        action_name = data.get("action_name", "Sign")

        # TmpFiles תיקייה שבה יש את הקובץ של הנתונים מה לקוח- גייסון
        json_path = os.path.join(tmp_folder, "TEMPJSON.json")
        with open(json_path, "w", encoding="utf-8") as fp:
            json.dump({
                "motion_data": motion_data,
                "action_name": action_name
            }, fp, ensure_ascii=False, indent=2)

        #  הרצת Blender עם serve_animation.py והעברת הנתיב ל־JSON
        blender_exe = r"C:\Program Files\Blender Foundation\Blender 4.4\blender.exe"
        blend_file = r"C:\CodesProject\blender_server\base.blend"
        serve_script = r"C:\CodesProject\blender_server\serve_animation.py"

        subprocess.run([
            blender_exe,
            blend_file,
            "--background",
            "--python", serve_script,
            "--",
            json_path
        ], check=True)

        # החזרת URL של הקובץ ללקוח
        return jsonify({
            "status": "ok",
            "url": f"http://localhost:5005/{action_name}.gltf"

        })


    except Exception as e:
        print("[ERROR] שגיאה במהלך יצירת האנימציה:", e)
        traceback.print_exc()
        return jsonify({"status": "error", "message": str(e)}), 500

    finally:
        anim_lock.release()



if __name__ == "__main__":
    app.run(port=5005)

