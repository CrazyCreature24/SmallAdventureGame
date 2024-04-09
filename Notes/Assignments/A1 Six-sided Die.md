
**Due Date**
Thursday, Jan 26, 6pm

**Git Setup**
- Create a branch called "Cube"
    - It has to be this exact branch name, I'm scripting pulling repos and switching branches

**Setup for 3D vertex positions**
- Make a_position a vec3
	- Attribute properties are defined in "varying.def.sc" in the DataRaw/Shaders folder
	- Changing the attribute type to vec3 won't prevent our meshes from rendering, the graphics pipeline will just fill the 'z' values with 0 for each 2d vertex
- Adapt all .vert files to use a vec3 a_position
	- Also update them to use World, View, and Proj matrices

**Vertex Format**
- Create a new vertex format with 3D positions
	- or adapt one of the vertex format structs to have 3D positions

**Die**
- Create or download a texture with 6 faces of a die on it
	- Load it, create a material, etc
- Make a create cube function in Shapes.cpp
    - You'll need 24 vertices and 36 indices
	    - 4 verts and 6 indices per side of the cube
    - Create a cube the requested size centered around 0,0,0
	    - Pass in a size, so we can make any size of rectangle in the future
	    - Setup the UVs so it shows the 6 faces of a die
- Render the die rotating so we see all 6 sides over time

**Submit**
- Commit to your branch and push your branch to your origin
- 
- Don't send pull requests
