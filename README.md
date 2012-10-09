# JRK

*A 3D real time strategy game, with network support.*

This project was done for the class on Videogame Programming. In its two month development it was 
basically my life, and I am quite impressed of the amount of features that we were able to develop in
such a short period.

This is a 3D strategy game done in OpenGL. It shows a heightfield scenario loaded from file and the players
can build buildings and and units (and do researchs) like in Starcraft. Loots of goodies were implemented, like
hotkeys, units smart actions, units following other units, immersive vision from a unit -like in a first person
shooter-, group selection, a minimap, voice feedback, and so on.

On the graphic front the scenario had day and night transitions, and after a while, you would see it change
to winter textures.

We also designed for network support from the beginning, and so the logic was decoupled of the graphic part, so
that at the final stage we implemented a network proxy for the logic interface and we tested it in a three
player game.

The game is not playable, but rather a tech showoff. We implemented the headquarters and the barracon 
buildings, and the engineer and soldier units. Of the logic, the combat part is missing which will allow
for some stub combat at this point.
