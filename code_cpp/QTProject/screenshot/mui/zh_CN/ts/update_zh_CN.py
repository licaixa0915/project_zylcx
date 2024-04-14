import os
import sys
cwd = os.path.dirname(__file__)
sys.path.append(os.path.join(cwd, '../../../../../mui/bin'))
from update_plugin_ts import update_plugin_ts
update_plugin_ts(cwd + os.sep + '../../../', cwd + os.sep + '../../../mui', 
    'kscreengrab.ts', ['zh_CN'], True)