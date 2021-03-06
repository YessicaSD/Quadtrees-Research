I am 
[Joan Marín](https://github.com/X0KA)
, student of the Bachelor’s Degree in Video Games by UPC at CITM. This content is generated for the second year’s subject Project 2, under supervision of lecturer Ricard Pillosu.
 
# Introduction

Something I like about coding, is that every task can be approached and solved in different ways.
 
In video games, when we want to check collisions, render tile maps, render 3D objects, etc. the first solutions that comes to our mind will usually be an exhaustive search, also called brute force search.
 
As its name says, this algorithms are generally problem-solving, but they scale with the size of candidates they have to check., working OK with a low amount of them but making our game unplayable if the amount is high.
 
For example, in order to check the collisions of the particles of the system in the image, if we used a brute force algorithm, we would check each particlie colliding with the others, no matter their position.

If we are working with a low amount of particles like in this image, we would iterate 72 (8x8) times each frame. But let’s say we are working with 100 particles, we would need 10.000 (100x100) iterations each frame.

<img src="images/low_particles.png" ><br>
 
Now imagine how many iterations we would need in a scene like this one:

<img src="images/lots_particles.png" ><br>

And that’s where spacial partitioning appears and saves us.

# Space partitioning

In order to avoid iterations that are not needed (colliders too far from each other, tiles / polygons out of the screen), we can divide the space into different subsets.
 
That process is called space partition. There are a lot of ways to divide the space, and depending on our needs we will choose one or another.
 
* **Quadtrees**: divides the space into 4 subsets, each of them might be divided into 4 subsets, etc.

<img src="images/quadtree.png" ><br>

* **Octrees**: similar to the quadtrees, but used in 3D instead of 2D. Each node will have 8 children instead of 4.

<img src="images/octree.png" ><br>

* **k-d trees**: the space is divided into 2 subspaces, which might not be equal. The “partition lines” are always perpendicular to the coordinates axis.

<img src="images/kdtree.png" ><br>

* **AABB Trees**: Creates subspaces for each group of elements we need to check. Mostly used in dynamic entities like collisions.
 
 <img src="images/aabbtree.png" ><br>
 
As you cans see in the images above, these data structures translate the space into nodes, which have more subnodes. This structure can be represented in tree diagrams, that's why they are called trees.
 
This are just a **few** ways to part the space explained vrey superficially. There are **a lot more** of them, and honestly, we could spend an entire semester talking about different space partition algorithms, but in this article I’m going to focus on Quadtrees.
 
 
# Quadtrees

As I said before, quadtrees are a data structure that divide the space into 4 sub regions. Each node will have four children, which will have four children each, etc. 

Even though I'm going to use them from a "video game" aproach, they can be used in a lot of fields, such as image compression.

 <img src="images/quadtree-image-compression.gif" ><br>
 *Compression of an image using quadtrees*
 
There are different types of quadtrees, but I will focus on the "Region Quadtrees", which are the most common and I think will be the most usefull in a 2D game.

This quadtree divide the total space into four equal regions, which will be divided into four equal regions and so on until we reach the nodes that are at the bottom of the tree. 

<img src="images/quadtree-1.png" ><br>

This type of structure works mostly with recursive fucntions, which the user will call to the main quadtree but then will be called to all its subnodes. 

OK, nice, now we know how quadtrees divide the space, but how can they be usefull in our game?

Let's go back to **particle** systems. Take a look at this images.

<img src="images/particles-gif.gif" ><br>

In the first frame we see how the space is not divided, therefore, we check collisions between all the particles (brute force). And in a system for only 20 particles, we need 400 iterations for each frame. It's important to know that the checks we make increase exponentially as we add more particles: with 10 particles we need 100 iterations, with 20 particles, 400 iterations, with 30 particles, 900 iterations, and so on.

In the second frame the space is divided into four subspaces, and each particle only checks its collision with the other particles in its own subspace. As you can see, it reduces the number of iterations a lot.

And in the third frame, we divide all the previous subspaces that had more than 3 particles. As you can see, there are some subspaces which only have one particle, so we won't even need to check their collision. 

Only by dividing the space twice, improved the performance of our system in a 1279%, by going from 400 iterations each frame to 29. Amazing, right?

### Quadtree structure ###

So, now let me explain superficially which I think are the core elements of a quadtree, and then explain more deeply how I used them in my project.

<img src="images/quadtree-core.png" ><br>

We will find this elements in any quadtree, no matter what we use them for, some functions might change, but the overall purpose will be the same. Obviously, we will need more methods and variables depending on what we use our quadtrees for, but this would be a nice parent class. 

The fact that some methods are virtual in this header does not mean that they have to be, this is how I made it, but as I said at the beggining, you can find your own way of coding them.

Basically, all we need is a rectangle that tells us the area the node is occupying (**section**).

The **level** and **max_levels** variables tell us in which level the node is in the tree and how much levels can the tree have respectively, so when a node's level is equal to max_levels, it won't be divided under any circumstance.

<img src="images/levels.png" ><br>

And obviously, an array with all the **subnodes**.

When it comes to the methods, the most important one is the **Split()**, which will divide the node into 4 subnodes.

<img src="images/split-function.png" ><br>

The **CheckTouch()** tells us if a rectangle fits or not to the current node (I know the name is pretty bad, I apologize).

<img src="images/checktouch.png" ><br>

Then we have the **DrawQuadtree()** which, guess what, is going to draw the quadtrees.

<img src="images/draw-qt.png" ><br>

And obviously the constructor and the cleanup.

### Camera culling with quadtrees ###

So, even thought all my examples were about particles and collisions, I used quadtrees for the camera culling of my game. I didn't mention it because i'll talk about it a little bit more deeply here.

When we are playing a video game, we don't need to render all the map, in fact, we must not, because it's a waste of time. As I said at the beggining, if the map isn't really big, it's not a big deal, but most of the cases it won't be like that, and especially in tiled maps we need to optimize the render process.

Let me put into situation. Let's say I'm developing an RTS with a map of 256x256 tiles. That's a total amount of 65.536 tiles, from which only 500 will appear in the screen, more or less. So, how do we print only the tiles that appear in the screen?

<img src="images/camera-culling.png" ><br>

The first solution that might come to our minds is to put a condition before drawing each tile to make sure they appear in the screen, right? **WRONG** We don't want to do that, that's brute force, we're still checking all the tiles. 

What we want to do is to only go across those tiles that appear int the screen, instead of going across all of them and only printing those that are in the screen. How do we do that?

I will create a quadtree in which i will store all the tiles distributed among all the nodes. Unlike particles, tiles are static, so I will only need to place them once. I will store the tiles in the nodes that are at the **bottom of the tree**, so I don't need to create more nodes and alocate data during the update phase.

Before, I showed you a screenshot of what the header of a quadtre would look like. To do this i will create a children class from that one. The header looks like this:

<img src="images/tile-quadtree-header.png" ><br>

First of all, I've created a struct called **TileData** which will store the position and ID of a tile. There will be one of them for each tile in the map. Each node will have a dinamic array in order to store them, but as I said before, I will only store the tiles in the nodes located at the bottom of the tree, so the arrays of the nodes that are not at the bottom will be **NULL**.

The **size** of the node will tell us how many tiles it can store, and **tiles_contained** how many it actually stores.

I've also added two functions:

**CheckVisibility()** will return true if the node is in the screen, and false if it does not.

**DrawMap()** if the node has children, what means it is not at the bottom of the tree, it will check if they are on the screen. each node in the screen will call it, if they are not they won't. When a node located at the bottom of the tree calls it, it will draw the tiles stored in his array.

#### Try to do it by yourself! ####

This is where the fun begins.

**TODO 0: WARMING UP**
Draw the quadtrees! Actually showed it before, but this is a pretty easy one, but since recursive functions are not the most common thing, we might need to start with a easy one.

<img src="images/todo-0.png" ><br>

**TODO 0: WARMING UP - SOLUTION**
As I said, pretty easy. There are different ways of doing it, this is the one I liked the most.

<img src="images/todo-0-solution.png" ><br>

**TODO 1 & 2 - Store the tiles** 
Time to store the tiles in the quadtree. Check if the node we are working with is located at the bottom of the tree. If it is, store the tile in it, if it's not, insert the tile to the pertinent subnode.

<img src="images/todo-1-2.png" ><br>

**TODO 1 - Solution**

Pretty easy. In this case i don't take care of the size of the array because I take care of it in the todo 2.
<img src="images/todo-1-solution.png" ><br>

**TODO 2 - Solution**

<img src="images/todo-2-solution.png" ><br>

**TODO 3 - Draw the map**

Before we start with the todo, I'll explain how I've been printing the map before implementing quadtrees, because you will probably need to get a bit familiar with it.

<img src="images/todo-3-introduction.png" ><br>

The way I've been storing tiled maps is the next: Each map has different layers, which store all the tiles in an array. So I added a quadtree for every layer of the map. Knowing this, I think we can start:

<img src="images/todo-3.png" ><br>

**TODO 3 - Solution**

<img src="images/todo-3-solution.png" ><br>

If you made it here, congratulations. Try pressing F1 once the application is running in order to compare the performance of the App when we're using quadtrees VS when we're using brute force.

## Results ##

Time to see how our application is doing. Let me remind you that I'm printing a 256 x 256 tiles map.

<img src="images/tiles_drawn.png" ><br>

As you can see here, it's really important to optimize the drawing methods of our games. We've gone from drawing **65.536 tiles each frame**, to drawing only those that are on the screen, which are 231. This value might change a bit depending on the position of our camera, but it wouldn't make a big difference.

And how does this affect the performance of our game? Well, let's see:

<img src="images/map_drawing_ms.png" ><br>

The numbers at the left, show the time spent drawing the map with quadtrees, and the ones at the right show the time spent using brute force. From spending more than 230 ms each frame to draw the map, we now spend 2 /4 seconds. So we improved the performance of our game in a 11.500%, not bad at all, huh?

## Things to improve ## 

In isometric maps, I need to create some extra room to store the tiles, like the double needed. That's because each node distributes the space equally to all its subnodes when it splits. Problem about it is that in the first quadtrees, there will be a lot of nodes that are located out of the map, which means that will store not a single tile, but still have the same size as the ones that will, and it needs to be solved.

Even so, I think it's worth it to make a bit of extra room in exchange for the improvement in performance we get. 

I'll work to solve it and I will update this article as soon as I find a solution.

If you made it down here, I hope this article was useful :)

## Webgraphy ##

[Quick Tip: Use Quadtrees to Detect Likely Collisions in 2D Space](https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374)

[JavaScript QuadTree Implementation](http://www.mikechambers.com/blog/2011/03/21/javascript-quadtree-implementation/)

[Pyramid Panic - Feature - QuadTree Optimizations](https://steemit.com/utopian-io/@carsonroscoe/pyramid-panic-feature-quadtree-optimizations)

[Examining Quadtrees, k-d Trees, and Tile Arrays](https://pdfs.semanticscholar.org/422f/63b62aaa6c8209b0dcbe8a53e360ad90514d.pdf)

[Teoría de colisiones 2D: QuadTree](https://www.genbeta.com/desarrollo/teoria-de-colisiones-2d-quadtree)

[AABB Trees for Collision Detection](https://goharsha.com/blog/aabb-trees-for-collision-detection/)

[Quadtrees – Hierarchical Grids](http://www.cs.tau.ac.il/~haimk/seminar12b/Quadtrees.pdf)
