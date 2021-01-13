# Cloth Simulation

Cloth Simulation Visual Studio C++ Project. 

Implementation of **Baraff, D. and Witkin, Large Steps in Cloth Simulation**<sup>[[1]](#reference1)</sup> using OpenGL

## Overview

- Particle based model using triangle mesh
- In each step, calculate positions of particles 
- Implemented stretch, shear, bending force (+gravity)
- Constraints are implemented using **mass modification**
- Used **modified conjugate gradient method** to solve the system 



## Equations

### 1. Implicit Integration

<p align="center">
    <img src="./images/Implicit-Integration.PNG" width="400px">
</p>

### 2. Forces

#### 1) Basic Formula

<p align="center">
    <img src="./images/forces.PNG" width="400px">
</p>

#### 2) Stretch/Shear Force

<p align="center">
    <img src="./images/stretch-shear.PNG" width="500px">
</p>

**[Stretch]**

<p align="center">
    <img src="./images/stretch.PNG" width="400px">
</p>

**[Shear]**

<p align="center">
    <img src="./images/shear.PNG" width="450px">
</p>

#### 3) Bend Force

<p align="center">
    <img src="./images/bend1.PNG" width="700px">
</p>

<p align="center">
    <img src="./images/bend2.PNG" width="650px">
</p>

### 3. Mass Modification

<p align="center">
    <img src="./images/mass-modification.PNG" width="400px">
</p>

### 4. Modified Conjugate Gradient

**[pseudo-code]**

<p align="center">
    <img src="./images/modified-CG.PNG" width="500px">
</p>

## Results

<p align="center">
    <img src="./images/result.gif">
</p>

## Reference

<a name="reference1">[1]</a> Baraff, D. and Witkin, A. 1998. Large Steps in Cloth Simulation. In *Proceedings of SIGGRAPH 98*, Annual Conference Series, 43–54.

[2] Prichard, D. 2003. Implementing Baraff & Witkin's Cloth Simulation

[3] Shewchuk, J. 1994. An introduction to the conjugate gradient method without the agonizing pain. Technical Report CMUCS-TR-94-125, Carnegie Mellon University.



