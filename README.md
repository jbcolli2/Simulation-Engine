# Simulation-Engine
This is a WIP project for learning OpenGL, rendering techniques, simulation algorithms, and improving my C++ skills.  It is a basic rendering engine, implemented with a components based model, that is aiming to be a framework that is as extensible as possible.  My purpose for creating this engine is to have a framework to be able to quickly create various physics simulations such as cloth or fluid simulation.  I would also like to explore various rendering techniques like particle effects, shadow creation, and splines.  I also wanted to better understand how the component model worked, and I figured the best way would be to create it myself from the ground up.


# Features
Here is a list of the some of the simulations and techniques I hope to explore with this engine:

- [x] Basic rendering with point and directional lights (diffuse and specular lighting) 
- [x] Cloth simulation using rods between point masses
- [ ] Cloth simulation using springs
- [ ] Self-collision within the cloth
- [ ] Cloth-object collision
- [ ] Fluid simulation of water
- [ ] Various shadow rendering techniques (to explore the differences in results between them)
- [ ] Particle effects
- [ ] Splines

# Controls

The camera can be controled with the mouse and WASD keys.  The main point light can be moved around in a similar way using the IJKL keys.

# Building
**Dependencies** 
- GLFW
- GLM
- ASSIMP

**Building**
Using CMake.
