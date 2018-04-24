This is a adaption of Fire2012 to work on a 2D-LED-Array.
The original 1-dimensional version "Fire2012" was created by Mark Kriegsman.

This basic one-dimensional 'fire' simulation works roughly as follows:
There's a underlying array of 'heat' cells, that model the temperature
at each point along the line.  Every cycle through the simulation, 
four steps are performed:
 1) All cells cool down a little bit, losing heat to the air
 2) The heat from each cell drifts 'up' and diffuses a little
 3) Sometimes randomly new 'sparks' of heat are added at the bottom
 4) The heat from each cell is rendered as a color into the leds array
    The heat-to-color mapping uses a black-body radiation approximation.

Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).

This simulation scales it self a bit depending on NUM_LEDS; it should look
"OK" on anywhere from 20 to 100 LEDs without too much tweaking. 

I recommend running this simulation at anywhere from 30-100 frames per second,
meaning an interframe delay of about 10-35 milliseconds.

Looks best on a high-density LED setup (60+ pixels/meter).

The 2D-Version just uses multiple of these heat-columns and puts them side by side.