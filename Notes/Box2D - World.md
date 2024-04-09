#Box2D[[Box2D - Overview]]

##### Include the library header
```c++
#include "../Libraries/box2d/include/box2d/box2d.h"
```

##### Create a world
```c++
	b2Vec2 gravity = b2Vec2(0,-10);
	m_pWorld = new b2World( gravity );
```

Next Steps:
Creating bodies: [[Box2D - Bodies]]
