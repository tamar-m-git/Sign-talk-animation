import sys
import json
import bpy
import os
script_dir = os.path.dirname(__file__)
sys.path.append(script_dir)
from blender_animator import animate_movement_from_points

# קבלת נתיב ה־JSON מהשורת פקודה
json_path = sys.argv[-1]


with open(json_path, "r", encoding="utf-8") as fp:
    data = json.load(fp)

motion_data = data["motion_data"]
action_name = data.get("action_name", "Sign")

# הרצת האנימציה
animate_movement_from_points(
    armature_name="Armature",
    motion_data=motion_data,
    start_frame=1,
    frames_per_step=20,
    action_name=action_name,
    use_ik=True,
    ik_targets=[
    "mixamorig6:RightHand",
    "mixamorig6:RightHandIndex1",
    "mixamorig6:RightHandMiddle1",
    "mixamorig6:RightHandRing1",
    "mixamorig6:RightHandPinky1",
    "mixamorig6:RightHandThumb1",
]

)

# ייצוא קבצים
output_dir = os.path.abspath("OUTPUT")
#blend_output = os.path.join(output_dir, f"{action_name}.blend")
#glb_output = os.path.join(output_dir, f"{action_name}.glb")
gltf_output = os.path.join(output_dir, f"{action_name}.gltf")

# שמירת BLEND
#bpy.ops.wm.save_as_mainfile(filepath=blend_output)

# ייצוא GLB
"""""
bpy.ops.export_scene.gltf(
    filepath=glb_output,
    export_format='GLB',
    use_selection=False,
    export_apply=True
)
"""

# ייצוא GLTF
bpy.ops.export_scene.gltf(
    filepath=gltf_output,
    export_format='GLTF_EMBEDDED',
    use_selection=False,
    export_apply=True
)

print(f"[✅] Done exporting to {output_dir}")
