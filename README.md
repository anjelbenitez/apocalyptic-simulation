# apocalyptic-simulation
An apocalyptic simulation using functional decomposition
Overview:
This project was run on OSU’s flip1 server. It is a month-to-month simulation consisting of four agents: Deer, Grain, Watcher, and MeteorShower, with each agent executed in its own thread. Aside from the interaction between the three main agents (Deer, Grain, and MeteorShower), external factors such as Temperature and Precipitation affect grain height, while an artificial factor measuring the gods’ temperament affects the probability of a meteor shower occurring.
Chosen Agent (Q #1):
I chose an apocalyptic meteor shower (MeteorShower) as a third agent to interact with the existing Deer and Grain agents. The probability of a meteor shower is a float (0. – 1.) that is randomly generated during each iteration. That factor is multiplied by an added artificial factor, also a float (0. – 4.) measuring the temperament of the gods. If the resulting computed probability is greater than or equal to 1, then a meteor shower is imminent. At that point, whether or not a meteor shower is destructive, that is to say, wipes out the entire Deer population and destroys all the existing Grain, is decided by a function ( rand( ) % 2 ).
Simulation Results (Q #2):
Note: For the Meteor Destruction measurement: a value of 1 == True and a value of 0 == False
![image](https://user-images.githubusercontent.com/55768082/136715791-5eacfd12-81e0-40e4-8168-b3585dbdf944.png)
![image](https://user-images.githubusercontent.com/55768082/136715796-348613d7-65f2-4852-aa55-c389631246ac.png)

Simulation Graph (Q #3):
A graph showing temperature, precipitation, grain height, number of deer, and the occurrence of a destructive meteor shower, as a function of month number.
![image](https://user-images.githubusercontent.com/55768082/136715803-0ded4583-bc29-4c59-a01c-d07bb22842d6.png)

Analysis of Patterns and Behaviors (Q #4):
We can see in the graph that Grain thrives under ideal conditions. If the conditions are extreme (i.e., too hot, or low precipitation), the Grain does not thrive. Consequently, if there is not enough Grain to feed the entire Deer population, the Deer population suffers (is decremented). On the other hand, if there is an abundance of Grain, more Deer can move into the environment (population is incremented). This relationship can be interrupted by an apocalyptic meteor shower (MeteorDestruction) at any given month. If a destructive meteor shower occurs, the entire Deer population is wiped out and all the Grain is destroyed. Evidence of this phenomenon can be seen below. The highlighted points show that when MeteorDestruction has a value of 1 (True), we see both the Deer population and Grain height drop to 0. Only when MeteorDestruction is 0 (False), can we see both the Grain and the Deer thrive.

![image](https://user-images.githubusercontent.com/55768082/136715810-62817b2d-ed7e-4a8a-94bf-dd84fbaf6f7c.png)
