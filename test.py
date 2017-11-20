"""
Use this script in order to reset a Maya scene and allow re-testing after a
change has been made to the plugin.
"""

import maya.cmds
maya.cmds.file(new=True, force=True)

maya.cmds.unloadPlugin('OpenEXRLoader')
maya.cmds.unloadPlugin('tiffFloatReader')
maya.cmds.unloadPlugin('ddsFloatReader')

maya.cmds.unloadPlugin('oiioImageReader')
maya.cmds.loadPlugin('oiioImageReader')