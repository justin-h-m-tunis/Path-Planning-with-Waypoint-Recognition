Demo.exe is the demo
the output is:
A map of the paths, waypoints and heatmap
-values of the heatmap are megenta
-waypoints are yellow circles
-paths are blue lines
A graph
-range of Y is -1.0 to 1.0
-magenta points are proportional time saved
-yellow is average of last 26 magenta points
A console output
-#WP is number of waypoints
-total time lost is the total time lost by WWA* (negative is time saved)

*NOTE*
Waypoint generation stops at 15, and after that it only compares
the planning times. I suggest letting it run till then to observe
the appreciable difference between the algorithms
