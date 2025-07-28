import bpy
from mathutils import Vector

def activate_and_select(obj):
    view_layer = bpy.context.view_layer
    with bpy.context.temp_override(active_object=obj, selected_objects=[obj], object=obj, view_layer=view_layer):
        if bpy.ops.object.mode_set.poll():
            bpy.ops.object.mode_set(mode='OBJECT')
        bpy.ops.object.select_all(action='DESELECT')
        obj.select_set(True)
        bpy.context.view_layer.objects.active = obj
# המרת 2 נקודות תלת ממד לקואטרניון
def calc_bone_direction_quaternion(start, end):
    direction = Vector(end) - Vector(start)
    #וקטור הכיוון ונירמול- רק כיוון ומבאל את ה אורך
    direction.normalize()
    up = Vector((0, 1, 0))
    # חישוב הסיבוב
    quat = up.rotation_difference(direction)
    return quat
#IK_Target רשימת שמות של עצמות שיש להן
def ensure_ik_target(obj, target_bone, location):
    # לחפש אם קיימת עצם כזו ואם לא ליצור אותה חדשה
    if target_bone not in obj.data.edit_bones:
        bpy.ops.object.mode_set(mode='EDIT')
        arm = obj.data
        new_bone = arm.edit_bones.new(target_bone)
        new_bone.head = location
        new_bone.tail = (location[0], location[1] + 0.1, location[2])
        bpy.ops.object.mode_set(mode='POSE')
# מוסיפים איבר מסוים לתהליך IK , שמים אותו היעד, ואורך שרשרת.
def add_ik_constraint(obj, bone_name, target_bone, chain_length=2):
    pb = obj.pose.bones.get(bone_name)
    if not pb:
        print(f"Bone '{bone_name}' not found")
        return
    ik = pb.constraints.new('IK')
    ik.target = obj
    ik.subtarget = target_bone
    ik.chain_count = chain_length

def animate_movement_from_points(armature_name, motion_data, start_frame, frames_per_step, action_name, use_ik=False, ik_targets=None):

#האם להפעיל קינמטיקה
    obj = bpy.data.objects.get(armature_name)
    if not obj or obj.type != 'ARMATURE':
        print(f"Armature '{armature_name}' not found")
        return
    activate_and_select(obj)
    bpy.ops.object.mode_set(mode='POSE')

    action = bpy.data.actions.new(name=action_name)
    if not obj.animation_data:
        obj.animation_data_create()
    obj.animation_data.action = action


    # אם רוצים IK, לוודא שה-target קיים ואם לא – יוצרים אותו, ואז להוסיף constraint ולהנפיש את ה-target
    #כדי להבדיל בין ה-Bone שמזיזים  לבין העצם־יעד שתנוע מוסיפים 0
    if use_ik and ik_targets:
        for bone_name in ik_targets:
            target_bone = bone_name + "0"
            target_points = motion_data.get(target_bone)
            if target_points:
                ensure_ik_target(obj, target_bone, target_points[0])
                add_ik_constraint(obj, bone_name, target_bone, chain_length=4)
                # הנפשת ה-target לפי נקודות הקלט
                pb = obj.pose.bones.get(target_bone)
                if pb:
                    frame = start_frame
                    for pt in target_points:
                        pb.location = Vector(pt)
                        pb.keyframe_insert(data_path="location", frame=frame)
                        frame += frames_per_step

    # אחרת, FK רגיל עם quaternion
    for bone_name, points in motion_data.items():
        if use_ik and ik_targets and bone_name in ik_targets:
            continue  
        pb = obj.pose.bones.get(bone_name)
        if not pb:
            print(f"Bone '{bone_name}' not found")
            continue
        pb.rotation_mode = 'QUATERNION'
        frame = start_frame   
        for i in range(len(points) - 1):
            quat = calc_bone_direction_quaternion(points[i], points[i+1])
            pb.rotation_quaternion = quat
            pb.keyframe_insert(data_path="rotation_quaternion", frame=frame)
            frame += frames_per_step

    bpy.ops.object.mode_set(mode='OBJECT')
    print("Animation done!")