# pwqRenderer

## Introduction

This repo is a toy renderer implemented using C++ and OpenGL. 

The following features has been implemented:
```
Cube
Texture: Diffusion Map; Normal Map; Specular Map; Emissive Map
Material
Light: Point Light; Directional Light; Spot Light;
Shadow
Skybox
Camera
Generate land from height map
Bone based animation
FBX/ADE format mesh and animation loading
```
The following features will be completed in the future:
```
Collision: AABB; OBB; Sphere
Partile
```
The UML is shown below:
<img width="1058" alt="ClassDiagram2" src="https://github.com/user-attachments/assets/15b732d7-ff4d-4c9c-adcd-d4c526b34f81" />
When you run it, you can see:
![image](https://github.com/user-attachments/assets/cecb2683-4f7c-4032-a53b-3e31713dc4e0)

All resources such as meshes and textures were collected from websites. 

## Installation

Currently, only provides a brief dependency list. You can obtain executable files from Releases. The following libraries are used.
```
GLFW 
GLM 0.9.8.5
GLAD
ASSIMP
STB_IMAGE
```
## Run

Use `W` `A` `S` `D` to move around, use mouse to change camera direction, `left mouse button` to caputure cursor and `Esc` to escape.
