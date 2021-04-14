# Lehasit
Team Fortress 2 injection-based dodgeball bot

Dodgeball is a custom gamemode in Team Fortress 2 that involves rallying a homing projectile back and forth. This bot automates the reflecting of the projectile at the right time and providing configurable options to "spike" the projectile.    

The challenge of developing such a bot is compensating for network conditions. Normally, in the Source engine, entities like the homing projectile are properly lag compensated for clients, so lag has less of an effect on gameplay. However, since the dodgeball gamemode(mod) is run on the server only, it becomes impossible for clients to properly simulate the homing projectile. This bot attempts to correct for this by simulating the projectile by an amount relative to the client's current latency.

## Demonstration Video    
[![TF2:DB; Slightly Improved Prediction @ 80 ping](https://img.youtube.com/vi/QWc3oT6t_Rg/0.jpg)](https://www.youtube.com/watch?v=QWc3oT6t_Rg)
