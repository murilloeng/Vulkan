#imports
import os

#shader files
for root, dirs, files in os.walk("shd"):
	for file in files:
		src_path = os.path.join(root, file)
		if src_path.endswith(".vert"):
			obj_path = src_path.replace(".vert", "_vert.spv")
			if not os.path.isfile(obj_path) or os.path.getmtime(src_path) > os.path.getmtime(obj_path):
				os.system("glslc %s -o %s" % (src_path, obj_path))
		if src_path.endswith(".frag"):
			obj_path = src_path.replace(".frag", "_frag.spv")
			if not os.path.isfile(obj_path) or os.path.getmtime(src_path) > os.path.getmtime(obj_path):
				os.system("glslc %s -o %s" % (src_path, obj_path))