#! /usr/bin/python3
import numpy as np
import sys
import os
import importlib
import matplotlib.pyplot as plt

## 这是c++导出库的python签名封装，使用python时import此文件即可

# to import mesh_slicer
# dummy declare, implemented in cpp
class mesh_slicer:
    def __init__(self, filename :str) -> None:
        assert(False)
        pass
    def get_intersection_polylines(self, a: float, b: float, c: float, d: float) -> list:
        assert(False)
        pass
    def get_original_vertices(self) -> np.ndarray:
        assert(False)
        pass
    def get_original_faces(self) -> np.ndarray:
        assert(False)
        pass
    def save_intersection_polylines(self, a: float, b: float, c: float, d: float, filename: str, sep: str=',' ) -> None:
        assert(False)
        pass


script_path = sys.path[0] # 脚本路径
lib_path = os.path.abspath(script_path + "/../build/src/")
sys.path.append(script_path)
sys.path.append(lib_path) # 增加导入库时的系统搜索路径
# 导入库Way1
mesh_slicer = importlib.import_module('libmesh_slicer').mesh_slicer
# 导入库Way2
# from libstlread import stlreader
