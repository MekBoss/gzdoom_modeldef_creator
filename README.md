# gzdoom_modeldef_creator
Really straightforward script which read md3 model file and create, almost, ready to use MODELDEF definition from it.
Only small editing and its ready to go. 

# How to use
1. Put executable and model in the same folder.
2. Open cmd in that folder and type "executable_name md3_file_name sprite_name" and press enter.

It create modeldef.model_name.txt file with all frames defined in model exported to modeldef file with defined in comand line sprite name.

# Current limitations.
It can create definitions for md3 files only. md2/unreal model support would be added after I find this formats specifications.