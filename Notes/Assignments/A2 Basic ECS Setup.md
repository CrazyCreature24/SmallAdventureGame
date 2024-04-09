
**Due Date**
Wednesday, Feb 1, 6pm

**Git Setup**
- Create a branch called "project"
	- It has to be this exact branch name, I'm scripting pulling repos and switching branches
	- This is going to be your new "main" with all your branches merged into this one
	- Merge in your "Cube" branch
- Create a branch called "components" or similar
	- When you're done, you'll merge this into your "project" branch

**Component classes**
- Create a Component base class
	- Give it a pure virtual GetType method
	- Give it a GameObject*
- Create a few subclasses for:
	- Transform
	- Mesh Rendering
	- Physics
- Each component subclass will store a unique type (see Event class for an example of how to make this efficient and fairly user friendly)

**ComponentManager**
- You need one ComponentManager instance for the game
	- In the future, we'll add scenes, and you'll need one ComponentManager for each Scene
- The manager will need to store a list of component pointers for each component type
- Drop in methods to do the following:
	- Add a component
	- Remove a component
	- Get a specific component vector (see Systems section below)

**Systems**
- These functions can be part of the ComponentManager, but we'll eventually need to make Game specific ones
	- Add a method to ComponentManager to retrieve a single vector of a specific component type
		- Don't return it by value, that will make a full copy of the vector
- You need at least the following systems:
	- One to update all the transforms
	- One to sync all the physics positions and rotations
	- One to render all the meshes

**GameObject**
- GameObjects will hold a vector of components
- Allow for GameObjects that have no components
	- i.e. don't create components in the GameObject constructor
- Creating a new component of any given type will need to add it to the GameObject list as well as the ComponentManager list
- Create helper functions to enable/disable a specific GameObject
	- i.e. Remove all of the GameObject components from the ComponentManager and readd them

**All of the above**
- Add any members, methods, etc, that will make the Game code simpler and easier to use when making new GameObjects with a variety of components or when writing new "systems"

**Make it all work**
- Have the Game class call each of your systems from the right place and in the right order to keep the game working

**Cleanup**
- Remove the old GameObject::Draw method
- Remove any physics object sync code from GameObject
	- It should now be done by the "system"
- Remove the old unused uniforms from the last batch of changes related to setting up matrices
- Make sure you delete all allocations when shutting down

**Submit**
- Commit to your "components" branch and push your branch to your origin
- Switch to your "project" branch and merge "components" into it
	- Commit and push that to your origin as well
- Don't send pull requests
