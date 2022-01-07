<strong>2D Simple Animations by using C++ OpenGL (including glfw libraries).</strong>

When the user presses specific keyboard keys, one of the following objects will be drawn at random points in space (visible on the screen). Only one object should be visible at a time.
 
  *	Square
  *	Hexagon
  *	Octagon
 
![Alt Text](https://media.giphy.com/media/z3EsHg3pl51P4skzJC/giphy.gif)

When the user presses specific keyboard key (“T”), <strong>translation</strong> will be applied to the object over time (create the illusion of continuous movement from left to right and back) – the object should start the back movement when it reached the (right) end of the window

![Alt Text](https://media.giphy.com/media/dZJvKjR23o3lXNu8qu/giphy.gif)

When the user presses specific keyboard keys (“R”), <strong>rotation</strong> will be applied to the object over time (create the illusion of making the object bigger and 
then smaller) – the object should start shrinking after reaching a size that fills the whole window

![Alt Text](https://media.giphy.com/media/NFwNBWLj1vfxD6KGxf/giphy.gif)

When the user presses specific keyboard keys (“S”), <strong>scalation</strong> will be applied to the object over time around its own center

![Alt Text](https://media.giphy.com/media/5HxUtCnAPm8FrojDuB/giphy.gif)

When the user presses a different transformation key, the current one should stop (e.g. If we press T and then S, we won’t get a continuous translation + scaling at the same time)

![Alt Text](https://media.giphy.com/media/XFvDLatGnScYK4VUOO/giphy.gif)

When the user presses "O" will completely clear the screen displaying only the background

![Alt Text](https://media.giphy.com/media/rNFtX91A8uXmaITRUc/giphy.gif)
