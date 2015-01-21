# 
# Copyright (C) 2015 Emmanuel Durand
# 
# This file is part of Splash (http://github.com/paperManu/splash)
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# blobserver is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with blobserver.  If not, see <http://www.gnu.org/licenses/>.
# 

bl_info = {
    "name": "Splash output",
    "author": "Emmanuel Durand",
    "version": (0, 1, 9),
    "blender": (2, 72, 0),
    "location": "3D View > Toolbox",
    "description": "Utility tools to connect Blender to the Splash videomapper",
    "category": "Object",
}


if "bpy" in locals():
    import imp
    imp.reload(ui)
    imp.reload(operators)
else:
    import bpy
    from bpy.props import (StringProperty,
                           BoolProperty,
                           EnumProperty,
                           IntProperty,
                           FloatProperty,
                           PointerProperty,
                           )
    from bpy.types import (Operator,
                           AddonPreferences,
                           PropertyGroup,
                           )
    from . import ui
    from . import operators

def getImageList(scene, context):
    images = []
    for image in bpy.data.images:
        images.append((image.name, image.name, ""))
    return images

class SplashSettings(PropertyGroup):
    outputActive = BoolProperty(
        name="Splash output active",
        description="True if the data is being sent to Splash",
        options={'SKIP_SAVE'},
        default=False
        )
    sendTexture = BoolProperty(
        name="Texture is sent if active",
        description="Set to true to send the texture along with the mesh",
        default=False
        )
    targetObject = StringProperty(
        name="Object name",
        description="Name of the object being sent",
        default="", maxlen=1024,
        )
    outputPathPrefix = StringProperty(
        name="Splash output path prefix",
        description="Path prefix to the shared memory where the selected object is sent",
        default="/tmp/blenderToSplash", maxlen=1024, subtype="FILE_PATH",
        )
    updatePeriodObject = FloatProperty(
        name="Update period in Object mode",
        description="Time between updates of the shared memory while in Object mode",
        subtype='TIME',
        default=0.5,
        min=0.01, max=1.0,
        )
    updatePeriodEdit = FloatProperty(
        name="Update period in Edit mode",
        description="Time between updates of the shared memory while in Edit mode",
        subtype='TIME',
        default=0.05,
        min=0.01, max=1.0,
        )
    textureName = EnumProperty(
        name="Images",
        description="Available images to send",
        items=getImageList
        )
    updatePeriodTexture = FloatProperty(
        name="Update period for the texture",
        description="Time between updates of the texture",
        subtype='TIME',
        default=1.0,
        min=0.1, max=100.0,
        )

classes = (
    ui.SplashToolbarObject,
    ui.SplashToolbarMesh,

    operators.SplashActivateSendMesh,
    operators.SplashSendTexture,

    SplashSettings
    )


def register():
    for cls in classes:
        bpy.utils.register_class(cls)

    bpy.types.Scene.splash = PointerProperty(type=SplashSettings)


def unregister():
    for cls in classes:
        bpy.utils.unregister_class(cls)

    del bpy.types.Scene.splash