# robot
OS mindstorm project fall 2017 for the best OS team

## MAINFRAME
`kinda everyone`  
Links most of the modules and gives directions.  
This is a crude estimate of how it should work :
```
Wait for the start signal given by the server
Do the following while the map is not complete or non-existant (on the first iteration)
	send position to the server
	map the surroundings and save it in ~/map/...
	find a new spot in order to map optimaly
	create a path to it and save the path's nodes in ~/path
	while ~/path is not empty
		pop the next coordinate to which you need to move from ~/path
		move towards it
		if it's not the last node:
			send position to server
```

## Guidance system
`Natalia`  
Mainly mechanical and physical test heavy.
Gets directions, tells motors to rotate accordingly.
Stop movement if a robot suddently arrives.


## Mapping
`Axel`  
Turn the robot on itself and calculate the distance from the walls.
After each abrupt change in distance to robot measured, check if it is not due to the other robot and
add a new node and vertice to the memorised map.

## Pathfinding
`?`  
Which path on the map should the robot take to join a coordinate? Look up for the dijkstra or the A\* algorithms.

## Next spot to map
`?`  
Figures out which spot is optimal for a new round of mapping.
Reads the map info and determines which nodes are missing vertices to complete an object of the map. 
What spot should the robot be in to see the most of these nodes?
Careful : find a spot that we know exists, i.e. a spot which the robot has seen before, on the 
'interior' of the incomplete map.
Tells the system if the map is complete or not.

## Server communications
`Vemund`  
Self explanatory? Still, could you write down here how it is we should use this hypothetical communication function? 

## Detect an other robot
`Axel`  
Usually a problem during the mapping. because the robots are kinda boney, scanning through them creates noise. If we compare the scan to it's own low pass filter value we can detect hectic behavior synonym to noise i.e. an other robot.  
Same goes for when the robot goes in a straight line : if an other robot passes in front of it, it will be able to measure

## Collision handler (secondary)
`?`  
Detect collision :
 - Unpredicted sensor measures (sudden change in acceleration/gyroscope compared to what the robot 
 - Pressure sensors activated?  
 
Act accordingly :
 - wait for the end of the disturbance
 - map the surroundings
 - compare it to the memorised map
 - determine the new position & direction
 - start the process anew

## Memory

```bash
$ cat ~/map/nodes 
#vertices parsed followingly : ID,posX,posY
1,5,856
2,-89,6557
3,256,8766
...
```
```bash
$ cat ~/map/vertices 
#vertices parsed followingly : ID,node1ID,node2ID
1,3,2
2,5,7
3,1,2
...
```
```bash
$ cat ~/position.log 
#visited positions parsed followingly : date,posX,posY
000000,0,0
1579,1264,751
...
```
```bash
$ cat ~/path 
#lists the nodes to visit without mapping. The last node listed is the first one to visit 
#They are parsed followingly : posX,posY
1134,548 #<- newPosX,newPosY
1264,751
1895,756
1895,965 #<- first node to go towards
```

## Next modules
Include new modules for the following deadline (addition of movable obstacles, throw an object at a robot if it's detected ^^...)

Diagram:  
https://drive.google.com/file/d/1H9doWuWqNe3DVBzsik5Cms2MYLRC_kVg/view?usp=sharing

Project link :  
http://soc.eurecom.fr/OS/projects_fall2017.html
