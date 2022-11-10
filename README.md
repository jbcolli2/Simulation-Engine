# Simulation-Engine
This is a rendering engine written in C++ using the OpenGL graphics API.  I've implemented it with a components based model, aiming for a framework that is as extensible as possible.  My purpose for creating this engine is to have a framework to be able to quickly create various physics simulations such as cloth or fluid simulation.  I would also like to explore various rendering techniques like particle effects and improvements on shadow creation.  I also wanted to better understand how the component model worked, and I figured the best way would be to create it myself from the ground up.

I enjoy understanding how things work at their most base level.  I usually want to go beyond just taking something that works and using it, I want to understand why it works.  I figure the best way to do that is to implement it myself.  Hence, while I know how to turn on shadows in a game engine like Unity or Unreal, I wanted to know what the engine was actually doing when I click that button.  This is the main reason for starting this project.

Here is a list of the some of the simulations and techniques I hope to explore with this engine:

- Basic rendering with point and directional lights (diffuse and specular lighting) (Implemented)
- Cloth simulation using rods between point masses (Implemented)
- Cloth simulation using springs
- Self-collision within the cloth
- Cloth-object collision
- Fluid simulation of water
- Various shadow rendering techniques (to explore the differences in results between them)
- Particle effects
- Many more ...

# Controls

The camera can be controled with the mouse and WASD keys.  The main point light can be moved around in a similar way using the IJKL keys.
