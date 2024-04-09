#Box2D[[Box2D - Overview]]

##### Create a body
When creating a body, rather than passing in 13 different parameters, the box2d library uses a pattern of making local "definition" objects
In this case a *b2BodyDef* is simply a struct with 13 public members
```c++
	// Initialize a body definition object
	b2BodyDef bodyDef;
	// TODO: Set other b2BodyDef members here.
	m_pBody = m_pWorld->CreateBody( &bodyDef );
```

Bodies need fixtures and shapes before they can be affected by physics

##### Create a shape
Box2D physics shapes need to be convex
There's a helper function to make a box, but look at it's implementation to see how other shapes could be made, such as a hexagon
There's also a *b2CircleShape* which is a special case for circles and is very useful
```c++
	b2PolygonShape boxShape;
	boxShape.SetAsBox( 1, 1 );
```

##### Create a fixture
A fixture holds a shape along with other properties like density
In 3D physics engines, shapes often have many more vertices, so sharing "physics meshes" between physics bodies with different properties (density, bounciness, etc) is common. That's why we have this pattern of a *Body* holding multiple *fixtures*, each referencing a *shape/physics mesh*
```c++
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	// TODO: Set other b2FixtureDef members here.
	m_pBody->CreateFixture( &fixtureDef );
```

##### Step the world
```c++
	m_pWorld->Step( deltaTime, 8, 3 );
```

##### Display object position (see [[Formatting in printf statements]])
```c++
	ImGui::Text( "%f", m_pBody->GetPosition().y );
```
